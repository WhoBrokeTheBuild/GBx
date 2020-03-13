#include "lcd.h"

#include "cartridge.h"
#include "clock.h"
#include "cpu.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"

#include <time.h>
#include <string.h>

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

const char * LCD_MODE_STR[4] = {
    "HBlank",
    "VBlank",
    "SearchSprite",
    "DataTransfer",
};

const word TILE_MAP_ADDR[2] = {
    0x9800, // 9800-9BFF
    0x9C00, // 9C00-9FFF
};

const word TILE_DATA_ADDR[2] = {
    0x9000, // 8800-97FF
    0x8000, // 8000-8FFF
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

void DrawTiles()
{    
    const uint TILE_WIDTH     = 8;
    const uint TILE_HEIGHT    = 8;
    const uint TILES_PER_ROW  = 32;
    const uint TILE_DATA_SIZE = 16;

    byte WXM7 = WX - 7;

    bool usingWindow = (LCDC.WindowDisplayEnabled && WY <= LY);
    uint mapSelect = (usingWindow ? LCDC.WindowTileMapSelect : LCDC.TileMapSelect);
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

        byte line = (yPos % 8) * 2;
        byte data1 = ReadByte(dataOffset + line);
        byte data2 = ReadByte(dataOffset + line + 1);

        int bit = 0x80 >> (xPos % TILE_WIDTH);
        bool high = (data2 & bit);
        bool low  = (data1 & bit);
        byte colorIndex = 
            (high ? 0b10 : 0b00) | 
            (low  ? 0b01 : 0b00);
        
        switch (colorIndex) {
        case 0b00:
            colorIndex = BGP.Color0;
            break;
        case 0b01:
            colorIndex = BGP.Color1;
            break;
        case 0b10:
            colorIndex = BGP.Color2;
            break;
        case 0b11:
            colorIndex = BGP.Color3;
            break;
        }

        byte color = 0;
        switch (colorIndex) {
        case 0b00:
            color = LCD_COLOR_WHITE;
            break;
        case 0b01:
            color = LCD_COLOR_LIGHT_GRAY;
            break;
        case 0b10:
            color = LCD_COLOR_DARK_GRAY;
            break;
        case 0b11:
            color = LCD_COLOR_BLACK;
            break;
        }

        // if (LY < 0 || LY > (LCD_HEIGHT - 1) || 
        //     pixel > 0 || pixel < (LCD_WIDTH - 1)) {
        //     LogWarn("Drawing out of bounds");
        //     continue;
        // }

        uint off = (LY * LCD_BUFFER_WIDTH * LCD_BUFFER_COMPONENTS) + 
            (pixel * LCD_BUFFER_COMPONENTS);
        
        LCDBuffer[off + 0] = color;
        LCDBuffer[off + 1] = color;
        LCDBuffer[off + 2] = color;
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

                bool high = (data2 & bit);
                bool low  = (data1 & bit);
                byte colorIndex = 
                    (high ? 0b10 : 0b00) | 
                    (low  ? 0b01 : 0b00);

                palette * pal = (spr.Palette ? &OBP1 : &OBP0);

                byte color = 0;
                switch (colorIndex) {
                case 0b00:
                    colorIndex = BGP.Color0;
                    break;
                case 0b01:
                    colorIndex = BGP.Color1;
                    break;
                case 0b10:
                    colorIndex = BGP.Color2;
                    break;
                case 0b11:
                    colorIndex = BGP.Color3;
                    break;
                }

                switch (colorIndex) {
                case 0b00:
                    color = LCD_COLOR_WHITE;
                    break;
                case 0b01:
                    color = LCD_COLOR_LIGHT_GRAY;
                    break;
                case 0b10:
                    color = LCD_COLOR_DARK_GRAY;
                    break;
                case 0b11:
                    color = LCD_COLOR_BLACK;
                    break;
                }

                int x = -pixel + 7;

                uint off = (LY * LCD_BUFFER_WIDTH * LCD_BUFFER_COMPONENTS) 
                    + (x * LCD_BUFFER_COMPONENTS);
                LCDBuffer[off + 0] = color;
                LCDBuffer[off + 1] = color;
                LCDBuffer[off + 2] = color;
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
                // printSTAT();
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
                // printSTAT();
            }
        break;
        case STAT_MODE_HBLANK:
            if (modeTicks >= HBLANK_TICK_COUNT) {
                modeTicks -= HBLANK_TICK_COUNT;

                ++LY;
                updateCoincidence();

                if (LY == LCD_HEIGHT) {
                    STAT.Mode = STAT_MODE_VBLANK;
                    // printSTAT();
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
                    // clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, NULL);
                }
                else {
                    STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                    // printSTAT();
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
                    // printSTAT();
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
        (LCDC.SpriteSize ? "8x16" : "8x8"),
        (LCDC.TileMapSelect ? "9C00h-9FFFh" : "9800h-9BFFh"),
        (LCDC.TileDataSelect ? "8000h-8FFFh" : "8800h-97FFh"),
        LCDC.WindowDisplayEnabled,
        (LCDC.WindowTileMapSelect ? "9800h-9BFFh" : "9C00h-9FFFh"),
        LCDC.LCDEnabled);
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
