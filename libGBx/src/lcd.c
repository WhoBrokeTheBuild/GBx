#include "lcd.h"

#include "cartridge.h"
#include "clock.h"
#include "cpu.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"

#include <time.h>
#include <string.h>

bool LCDOriginalColors = false;

byte LCDBuffer[LCD_BUFFER_SIZE];

lcd_control LCDC;
lcd_status STAT;

byte SCY;
byte SCX;
byte LY;
byte LYC;
byte WX;
byte WY;

palette BGP;
palette OBP0;
palette OBP1;

byte VRAM[2][0x2000];
uint VRAMBank;

byte OAM[0xA0];

const byte LCD_COLORS_ORIG[][3] = {
    { 0x9B, 0xBC, 0x0F },
    { 0x8B, 0xAC, 0x0F },
    { 0x30, 0x62, 0x30 },
    { 0x0F, 0x38, 0x0F },
};

const byte LCD_COLORS_CLEAN[][3] = {
    { 0xFF, 0xFF, 0xFF },
    { 0xCC, 0xCC, 0xCC },
    { 0x77, 0x77, 0x77 },
    { 0x00, 0x00, 0x00 },
};

const char * LCD_MODE_STR[4] = {
    "HBlank",
    "VBlank",
    "SearchSprite",
    "DataTransfer",
};

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

const byte * GetColor(palette * pal, int bit, byte d1, byte d2)
{
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
        LCDOriginalColors
        ? LCD_COLORS_ORIG[colorIndex]
        : LCD_COLORS_CLEAN[colorIndex]
    );
}

const char * GetLCDModeString(uint mode)
{
    switch (mode) {
    case STAT_MODE_HBLANK:
        return "HBlank";
    case STAT_MODE_VBLANK:
        return "VBlank";
    case STAT_MODE_SEARCH_SPRITE:
        return "Search Sprite";
    case STAT_MODE_DATA_TRANSFER:
        return "Data Transfer";
    default:
        break;
    }

    return "";
}

void DrawTiles()
{
    byte WXM7 = WX - 7;

    bool usingWindow = (LCDC.WindowDisplayEnabled && WY <= LY);
    uint mapSelect = (usingWindow ? LCDC.WindowTileMapSelect : LCDC.BGTileMapSelect);
    word mapBaseAddress = TILE_MAP_ADDR[mapSelect];
    word dataBaseAddress = TILE_DATA_ADDR[LCDC.TileDataSelect];

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

        // if (LY < 0 || LY > (LCD_HEIGHT - 1) || 
        //     pixel > 0 || pixel < (LCD_WIDTH - 1)) {
        //     LogWarn("Drawing out of bounds");
        //     continue;
        // }

        uint off = (LY * LCD_BUFFER_WIDTH * LCD_BUFFER_COMPONENTS) + 
            (pixel * LCD_BUFFER_COMPONENTS);
        
        LCDBuffer[off + 0] = color[0];
        LCDBuffer[off + 1] = color[1];
        LCDBuffer[off + 2] = color[2];
    }
}

void DrawSprites()
{
    sprite spr;

    for (uint sprite = 0; sprite < 40; ++sprite) {
        uint index = sprite * 4;
        spr.Y = ReadWord(0xFE00 + index) - 16;
        spr.X = ReadWord(0xFE00 + index + 1) - 8;
        spr.Pattern = ReadWord(0xFE00 + index + 2);
        spr.Attributes = ReadWord(0xFE00 + index + 3);

        int height = 8;
        if (LCDC.SpriteSize == 1) {
            height = 16;
        }

        if (LY >= spr.Y && LY < spr.Y + height) {
            int line = LY - spr.Y;
            if (spr.YFlip) {
                line -= height;
                line *= -1;
            }
            line *= 2;
            
            word tileAddress = 0x8000 + (spr.Pattern * 16);
            byte data1 = ReadByte(tileAddress + line);
            byte data2 = ReadByte(tileAddress + line + 1);

            for (int pixel = 7; pixel >= 0; --pixel) {
                int bit = (1 << pixel);
                if (spr.XFlip) {
                    bit = (0x80 >> pixel);
                }

                palette * pal = (spr.Palette ? &OBP1 : &OBP0);
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
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
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
    static uint modeTicks = 0;
    modeTicks += cycles;

    switch (STAT.Mode) {
        case STAT_MODE_SEARCH_SPRITE:
            if (modeTicks >= SEARCH_SPRITE_TICK_COUNT) {
                modeTicks -= SEARCH_SPRITE_TICK_COUNT;

                STAT.Mode = STAT_MODE_DATA_TRANSFER;
            }
        break;
        case STAT_MODE_DATA_TRANSFER:
            if (modeTicks >= DATA_TRANSFER_TICK_COUNT) {
                modeTicks -= DATA_TRANSFER_TICK_COUNT;

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
            if (modeTicks >= HBLANK_TICK_COUNT) {
                modeTicks -= HBLANK_TICK_COUNT;

                ++LY;
                updateCoincidence();

                if (LY == LCD_HEIGHT + 1) {
                    STAT.Mode = STAT_MODE_VBLANK;

                    IF.VBlank = true;
                    if (STAT.IntVBlank) {
                        IF.STAT = true;
                    }

                    static struct timespec last;
                    // static double fpsTotal = 0.0;
                    // static long fpsCount = 0;
                    
                    // if (last.tv_nsec == 0) {
                    //     clock_gettime(CLOCK_MONOTONIC, &last);
                    // }
                    // else {
                        struct timespec now;
                    
                        clock_gettime(CLOCK_MONOTONIC, &now);
                        struct timespec delta = timediff(last, now);
                        last = now;
                        
                    //     float fps = 1.0 / (delta.tv_nsec / 1000000000.0);
                    //     // LogInfo("VBL %ld %f", delta.tv_nsec, fps);
                        
                    //     fpsTotal += fps;
                    //     ++fpsCount;
                        
                    //     if (fpsCount == 386) {
                    //         LogInfo("Average FPS %f", fpsTotal / fpsCount);
                    //     }
                    // }
                    
                    struct timespec wait;
                    wait.tv_sec = 0;
                    wait.tv_nsec = 16750419 - delta.tv_nsec;
                    clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, NULL);
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
            if (modeTicks >= VBLANK_TICK_COUNT) {
                modeTicks -= VBLANK_TICK_COUNT;
                
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

void PrintLCDC()
{
    LogInfo("BGWinDisp=%d OBJDisp=%d OBJSize=%s BGTileMap=%s TileData=%s WinDisp=%d WinTileMap=%s LCDEnab=%d",
        LCDC.TileDisplayEnabled,
        LCDC.SpriteDisplayEnabled,
        (LCDC.SpriteSize == 0 ? "8x8" : "8x16"),
        (LCDC.BGTileMapSelect == 0 ? "9800h-9BFFh" : "9C00h-9FFFh"),
        (LCDC.TileDataSelect == 0 ? "8800h-97FFh" : "8000h-8FFFh"),
        LCDC.WindowDisplayEnabled,
        (LCDC.WindowTileMapSelect == 0 ? "9800h-9BFFh" : "9C00h-9FFFh"),
        LCDC.Enabled);
}

void PrintSTAT() 
{
    LogInfo("Mode=%d (%s) Coincidence=%d IntHBlank=%d IntVBlank=%d IntSearchSprite=%d IntCoincidence=%d",
        STAT.Mode, LCD_MODE_STR[STAT.Mode], 
        STAT.Coincidence, 
        STAT.IntHBlank, 
        STAT.IntVBlank, 
        STAT.IntSearchSprite, 
        STAT.IntCoincidence);
}

void PrintPalettes()
{
    LogInfo("BGP Color0=%d Color1=%d Color2=%d Color3=%d",
        BGP.Color0, BGP.Color1, BGP.Color2, BGP.Color3);

    LogInfo("BGP Color0=%d Color1=%d Color2=%d Color3=%d",
        OBP0.Color0, OBP0.Color1, OBP0.Color2, OBP0.Color3);

    LogInfo("BGP Color0=%d Color1=%d Color2=%d Color3=%d",
        OBP1.Color0, OBP1.Color1, OBP1.Color2, OBP1.Color3);
}

void PrintLCDCoordinates()
{
    LogInfo("SCY=%d SCX=%d LY=%d LYC=%d WX=%d WY=%d",
        SCY, SCX, LY, LYC, WX, WY);
}
