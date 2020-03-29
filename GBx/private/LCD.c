#include <GBx/LCD.h>

#include <GBx/Cartridge.h>
#include <GBx/CPU.h>
#include <GBx/Interrupts.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>
#include <GBx/Util.h>

#include <time.h>
#include <string.h>

bool LCDUseOriginalColors = false;

byte LCDBuffer[LCD_BUFFER_SIZE];

lcd_control_t LCDC;
lcd_status_t STAT;

byte SCY;
byte SCX;
byte LY;
byte LYC;
byte WX;
byte WY;

palette_t BGP;
palette_t OBP0;
palette_t OBP1;

byte VRAM[2][0x2000];
uint VRAMBank;

byte OAM[0xA0];

void ResetLCD()
{
    VRAMBank = 0;
    
    for (uint i = 0; i < VRAM_BANK_COUNT; ++i) {
        memset(VRAM[i], 0, sizeof(VRAM[i]));
    }

    memset(OAM, 0, sizeof(OAM));

    LCDC.raw = 0x91;
    STAT.Mode = STAT_MODE_HBLANK;

    SCY = 0x00;
    SCX = 0x00;
    LY = 0x00;
    LYC = 0x00;
    WY = 0x00;
    WX = 0x00;

    BGP.raw = 0b11100100;
    OBP0.raw = 0b11100100;
    OBP1.raw = 0b11100100;
}

const byte * GetColor(palette_t * pal, int bit, byte d1, byte d2)
{
    static const byte COLORS_ORIG[][3] = {
        { 0x9B, 0xBC, 0x0F },
        { 0x8B, 0xAC, 0x0F },
        { 0x30, 0x62, 0x30 },
        { 0x0F, 0x38, 0x0F },
    };

    static const byte COLORS_CLEAN[][3] = {
        { 0xFF, 0xFF, 0xFF },
        { 0xCC, 0xCC, 0xCC },
        { 0x77, 0x77, 0x77 },
        { 0x00, 0x00, 0x00 },
    };

    bit = 0x80 >> bit;
    bool high = (d2 & bit);
    bool low  = (d1 & bit);
    byte colorIndex = 
        (high ? 0b10 : 0b00) | 
        (low  ? 0b01 : 0b00);
    
    switch (colorIndex) {
    case 0b00:
        colorIndex = pal->Color0;
        break;
    case 0b01:
        colorIndex = pal->Color1;
        break;
    case 0b10:
        colorIndex = pal->Color2;
        break;
    case 0b11:
        colorIndex = pal->Color3;
        break;
    }

    return (
        LCDUseOriginalColors
        ? COLORS_ORIG[colorIndex]
        : COLORS_CLEAN[colorIndex]
    );
}

void OAMDMATransfer(byte data)
{
    CPUEnabled = false;

    word addr = data << 8;
    for (unsigned i = 0; i < 0xA0; ++i) {
        OAM[i] = ReadByte(addr + i);
        Tick(4);
    }

    CPUEnabled = true;
}

void DrawTiles()
{
    const int TILE_WIDTH     = 8;
    const int TILE_HEIGHT    = 8;
    const int TILE_DATA_SIZE = 16;
    const int TILES_PER_ROW  = 32;

    bool usingWindow = (LCDC.WindowDisplayEnabled && WY <= LY);

    uint mapSelect = (usingWindow 
        ? LCDC.WindowTileMapSelect 
        : LCDC.BGTileMapSelect);

    word mapBaseAddress = GetTileMapAddress(mapSelect);
    word dataBaseAddress = GetTileDataAddress(LCDC.TileDataSelect);

    byte WXM7 = WX - 7;

    byte yPos = 0;
    if (usingWindow) {
        yPos = LY - WY;
    }
    else {
        yPos = SCY + LY;
    }

    int tileRow = (byte)(yPos / TILE_HEIGHT) * TILES_PER_ROW;

    for (int pixel = 0; pixel < LCD_WIDTH; ++pixel) {
        byte xPos = pixel + SCX;
        if (usingWindow && pixel >= WXM7) {
            xPos = pixel - WXM7;
        }

        int tileCol = xPos / TILE_WIDTH;

        int tileIndex = ReadByte(mapBaseAddress + tileRow + tileCol);
        if (LCDC.TileDataSelect == 0) {
            tileIndex = (sbyte)tileIndex; // Convert to [-128, 127]
        }

        word dataOffset = dataBaseAddress + (tileIndex * TILE_DATA_SIZE);

        byte line = (yPos % TILE_HEIGHT) * 2;
        byte data1 = ReadByte(dataOffset + line);
        byte data2 = ReadByte(dataOffset + line + 1);

        int bit = (xPos % TILE_WIDTH);
        const byte * color = GetColor(&BGP, bit, data1, data2);

        uint off = (LY * LCD_BUFFER_WIDTH * LCD_BUFFER_COMPONENTS) + 
            (pixel * LCD_BUFFER_COMPONENTS);
        
        LCDBuffer[off + 0] = color[0];
        LCDBuffer[off + 1] = color[1];
        LCDBuffer[off + 2] = color[2];
    }
}

void DrawSprites()
{
    const int OAM_ADDRESS = 0xFE00;
    const int OAM_ENTRY_DATA_SIZE = 4;
    const int SPRITE_Y_OFFSET = 16;
    const int SPRITE_X_OFFSET = 8; 

    sprite_attrib_t attrib;
    for (uint sprite = 0; sprite < 40; ++sprite) {
        uint offset = sprite * OAM_ENTRY_DATA_SIZE;

        int y = ReadWord(OAM_ADDRESS + offset) - SPRITE_Y_OFFSET;
        
        ++offset;
        int x = ReadWord(OAM_ADDRESS + offset) - SPRITE_X_OFFSET;

        ++offset;
        int pattern = ReadWord(OAM_ADDRESS + offset);

        ++offset;
        attrib.raw = ReadWord(OAM_ADDRESS + offset);

        int height = 8;
        if (LCDC.SpriteSize == 1) {
            height = 16;
        }

        if (LY >= y && LY < y + height) {
            int line = LY - y;
            if (attrib.YFlip) {
                line -= height;
                line *= -1;
            }
            line *= 2;
            
            word tileAddress = 0x8000 + (pattern * 16);
            byte data1 = ReadByte(tileAddress + line);
            byte data2 = ReadByte(tileAddress + line + 1);

            for (int pixel = 7; pixel >= 0; --pixel) {
                int bit = (1 << pixel);
                if (attrib.XFlip) {
                    bit = (0x80 >> pixel);
                }

                palette_t * pal = (attrib.Palette ? &OBP1 : &OBP0);
                const byte * color = GetColor(pal, bit, data1, data2);

                int x = -pixel + 7;

                uint off = (LY * LCD_BUFFER_WIDTH * LCD_BUFFER_COMPONENTS) 
                    + (x * LCD_BUFFER_COMPONENTS);
                LCDBuffer[off + 0] = color[0];
                LCDBuffer[off + 1] = color[1];
                LCDBuffer[off + 2] = color[2];
            }
        }
    }
}

struct timespec timediff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec) < 0) {
		temp.tv_sec = end.tv_sec-start.tv_sec - 1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void updateCoincidence()
{
    STAT.Coincidence = (LY == LYC);
    if (STAT.IntCoincidence && STAT.Coincidence) {
        IF.STAT = true;
    }
}

void LCDTick(uint cycles)
{
    // Convert Ticks from M-Cycles to T-Cycles
    const int HBLANK_TICKS        = 204 * 4;
    const int SEARCH_SPRITE_TICKS = 80  * 4;
    const int DATA_TRANSFER_TICKS = 172 * 4;

    // 1/10th of VBlank period, or one line worth of ticks
    const int VBLANK_TICKS = (HBLANK_TICKS 
        + SEARCH_SPRITE_TICKS 
        + DATA_TRANSFER_TICKS);

    static uint modeTicks = 0;
    modeTicks += cycles;

    switch (STAT.Mode) {
        case STAT_MODE_SEARCH_SPRITE:
            if (modeTicks >= SEARCH_SPRITE_TICKS) {
                modeTicks -= SEARCH_SPRITE_TICKS;

                STAT.Mode = STAT_MODE_DATA_TRANSFER;
            }
        break;
        case STAT_MODE_DATA_TRANSFER:
            if (modeTicks >= DATA_TRANSFER_TICKS) {
                modeTicks -= DATA_TRANSFER_TICKS;

                if (LCDC.TileDisplayEnabled) {
                    DrawTiles();
                }

                if (LCDC.SpriteDisplayEnabled) {
                    DrawSprites();
                }

                STAT.Mode = STAT_MODE_HBLANK;
            }
        break;
        case STAT_MODE_HBLANK:
            if (modeTicks >= HBLANK_TICKS) {
                modeTicks -= HBLANK_TICKS;

                ++LY;
                updateCoincidence();

                if (LY == LCD_HEIGHT) {
                    STAT.Mode = STAT_MODE_VBLANK;

                    IF.VBlank = true;
                    if (STAT.IntVBlank) {
                        IF.STAT = true;
                    }

                    // static struct timespec last;
                    // static double fpsTotal = 0.0;
                    // static long fpsCount = 0;
                    
                    // if (last.tv_nsec == 0) {
                    //     clock_gettime(CLOCK_MONOTONIC, &last);
                    // }
                    // else {
                        // struct timespec now;
                    
                        // clock_gettime(CLOCK_MONOTONIC, &now);
                        // struct timespec delta = timediff(last, now);
                        // last = now;
                        
                    //     float fps = 1.0 / (delta.tv_nsec / 1000000000.0);
                    //     // LogInfo("VBL %ld %f", delta.tv_nsec, fps);
                        
                    //     fpsTotal += fps;
                    //     ++fpsCount;
                        
                    //     if (fpsCount == 386) {
                    //         LogInfo("Average FPS %f", fpsTotal / fpsCount);
                    //     }
                    // }
                    
                    // struct timespec wait;
                    // wait.tv_sec = 0;
                    // wait.tv_nsec = 16750419 - delta.tv_nsec;
                    // clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, NULL);
                }
                else {
                    STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                    if (STAT.IntSearchSprite) {
                        IF.STAT = true;
                    }
                }
            }
        break;
        case STAT_MODE_VBLANK:
            if (modeTicks >= VBLANK_TICKS) {
                modeTicks -= VBLANK_TICKS;
                
                LY = (LY + 1) % LCD_MAX_LY;
                updateCoincidence();

                if (LY == 0) {
                    STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                    if (STAT.IntSearchSprite) {
                        IF.STAT = true;
                    }
                }
            }
        break;
    }
}

word GetTileMapAddress(uint index)
{
    static const word ADDR[] = {
        0x9800,
        0x9C00,
    };

    return ADDR[index];
}

word GetTileDataAddress(uint index)
{
    static const word ADDR[] = {
        0x9000,
        0x8000,
    };

    return ADDR[index];
}

const char * GetLCDModeString(uint mode)
{
    static const char * STR[] = {
        "HBlank",
        "VBlank",
        "SearchSprite",
        "DataTransfer",
    };

    return STR[mode];
}

const char * GetTileMapRangeString(uint index)
{
    static const char * STR[] = {
        "$9800-$9BFF",
        "$9C00-$9FFF",
    };

    return STR[index];
}

const char * GetTileDataRangeString(uint index)
{
    static const char * STR[] = {
        "$8800-$97FF",
        "$8000-$8FFF",
    };

    return STR[index];
}

const char * GetSpriteSizeString(uint index)
{
    static const char * STR[] = {
        "8x8",
        "8x16",
    };

    return STR[index];
}

void PrintLCDC()
{
    LogInfo("LCDC %s OBJ=%s OBJSize=%s BG=%s BGTileMap=%s TileData=%s Win=%s WinTileMap=%s",
        GetEnabledString(LCDC.Enabled),
        GetEnabledString(LCDC.SpriteDisplayEnabled),
        GetSpriteSizeString(LCDC.SpriteSize),
        GetEnabledString(LCDC.TileDisplayEnabled),
        GetTileMapRangeString(LCDC.BGTileMapSelect),
        GetTileDataRangeString(LCDC.TileDataSelect),
        GetEnabledString(LCDC.WindowDisplayEnabled),
        GetTileMapRangeString(LCDC.WindowTileMapSelect));
}

void PrintSTAT() 
{
    LogInfo("STAT Mode=%d (%s) Coincidence=%d IntHBlank=%d IntVBlank=%d IntSearchSprite=%d IntCoincidence=%d",
        STAT.Mode, 
        GetLCDModeString(STAT.Mode), 
        STAT.Coincidence, 
        STAT.IntHBlank, 
        STAT.IntVBlank, 
        STAT.IntSearchSprite, 
        STAT.IntCoincidence);
}

void PrintPalette(const char * name, palette_t * pal)
{
    LogInfo("%s Color0=%d Color1=%d Color2=%d Color3=%d",
        name, pal->Color0, pal->Color1, pal->Color2, pal->Color3);
}

void PrintLCDCoordinates()
{
    LogInfo("SCY=%d SCX=%d LY=%d LYC=%d WX=%d WY=%d",
        SCY, SCX, LY, LYC, WX, WY);
}