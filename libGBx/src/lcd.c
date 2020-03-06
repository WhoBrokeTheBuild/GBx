#include "lcd.h"

#include "cartridge.h"
#include "clock.h"
#include "cpu.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"

#include <time.h>

LCDC_t LCDC;

STAT_t STAT;

uint8_t SCY = 0x00;
uint8_t SCX = 0x00;
uint8_t LY  = 0x00;
uint8_t LYC = 0x00;
uint8_t WX  = 0x00;
uint8_t WY  = 0x00;

palette_t BGP  = { .raw = 0b11100100 };
palette_t OBP0 = { .raw = 0b11100100 };
palette_t OBP1 = { .raw = 0b11100100 };

uint8_t VRAM0[0x1FFF];
uint8_t VRAM1[0x1FFF];
uint8_t * VRAM = VRAM0;

uint8_t OAM[0xA0];

uint8_t Backbuffer[BACKBUFFER_WIDTH * BACKBUFFER_HEIGHT * BACKBUFFER_COMP];

const char * LCD_MODE_STR[4] = {
    "HBlank",
    "VBlank",
    "SearchSprite",
    "DataTransfer",
};

uint16_t TILE_MAP_ADDR[2] = {
    0x9800, // 9800-9BFF
    0x9C00, // 9C00-9FFF
};

uint16_t TILE_DATA_ADDR[2] = {
    0x9000, // 8800-97FF
    0x8000, // 8000-8FFF
};

#define HBLANK_TICK_COUNT        (204)
#define VBLANK_TICK_COUNT        (456) // 1/10 of VBlank Period
#define SEARCH_SPRITE_TICK_COUNT (80)
#define DATA_TRANSFER_TICK_COUNT (172)

unsigned lcdModeTotalTicks = 0;

#define MAX_LY (154)

#define TILE_WIDTH     (8)
#define TILE_HEIGHT    (8)
#define TILES_PER_ROW  (32)
#define TILE_DATA_SIZE (16)

#define COLOR_WHITE      (0xFF)
#define COLOR_LIGHT_GRAY (0xCC)
#define COLOR_DARK_GRAY  (0x77)
#define COLOR_BLACK      (0x00)

struct timespec diff(struct timespec start, struct timespec end)
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

void drawTiles()
{
    int wx7 = WX - 7;

    unsigned mapSelect = (LCDC.WindowDisplayEnable ? LCDC.WindowTileMapSelect : LCDC.TileMapSelect);
    uint16_t mapBaseAddress = TILE_MAP_ADDR[mapSelect];
    uint16_t dataBaseAddress = TILE_DATA_ADDR[LCDC.TileDataSelect];

    int srcY = LY;
    int dstY = LY + SCY;
    if (LCDC.WindowDisplayEnable) {
        dstY = LY - WY;
    }

    uint16_t yOffset = (dstY / TILE_HEIGHT) * TILES_PER_ROW;
    uint16_t lineOffset = (dstY % TILE_HEIGHT) * 2;

    for (unsigned srcX = 0; srcX < SCREEN_WIDTH; ++srcX) {

        int dstX = srcX + SCX;
        if (LCDC.WindowDisplayEnable && srcX >= wx7) {
            dstX = srcX - wx7;
        }

        uint16_t xOffset = dstX / TILE_WIDTH;

        int tileIndex = readByte(mapBaseAddress + yOffset + xOffset);
        if (LCDC.TileDataSelect == 0) {
            tileIndex = (int8_t)tileIndex; // Convert to [-128, 127]
        }

        uint16_t dataOffset = dataBaseAddress + (tileIndex * TILE_DATA_SIZE);

        uint8_t data1 = readByte(dataOffset + lineOffset);
        uint8_t data2 = readByte(dataOffset + lineOffset + 1);

        int bit = 0x80 >> (srcX % TILE_WIDTH);
        bool high = (data2 & bit);
        bool low  = (data1 & bit);
        uint8_t colorIndex = 
            (high ? 0b10 : 0b00) | 
            (low  ? 0b01 : 0b00);
        
        uint8_t color = 0;
        switch (colorIndex) {
        case 0b00:
            colorIndex = BGP.Color00;
            break;
        case 0b01:
            colorIndex = BGP.Color01;
            break;
        case 0b10:
            colorIndex = BGP.Color10;
            break;
        case 0b11:
            colorIndex = BGP.Color11;
            break;
        }

        switch (colorIndex) {
        case 0b00:
            color = COLOR_WHITE;
            break;
        case 0b01:
            color = COLOR_LIGHT_GRAY;
            break;
        case 0b10:
            color = COLOR_DARK_GRAY;
            break;
        case 0b11:
            color = COLOR_BLACK;
            break;
        }

        unsigned off = (srcY * BACKBUFFER_WIDTH * 3) + (srcX * 3);
        Backbuffer[off + 0] = color;
        Backbuffer[off + 1] = color;
        Backbuffer[off + 2] = color;
    }
}

void drawSprites()
{
    sprite_t s;
    for (int sprite = 0; sprite < 40; ++sprite) {
        uint8_t index = sprite * 4;
        s.Y = readWord(0xFE00 + index) - 16;
        s.X = readWord(0xFE00 + index + 1) - 8;
        s.Pattern = readWord(0xFE00 + index + 2);
        s.Attributes = readWord(0xFE00 + index + 3);

        int height = 8;
        if (LCDC.SpriteSize == 1) {
            height = 16;
        }

        if (LY >= s.Y && LY < s.Y + height) {
            int line = LY - s.Y;
            if (s.YFlip) {
                line -= height;
                line *= -1;
            }
            line *= 2;
            
            uint16_t tileAddress = 0x8000 + (s.Pattern * 16);
            uint8_t data1 = readByte(tileAddress + line);
            uint8_t data2 = readByte(tileAddress + line + 1);

            for (int pixel = 7; pixel >= 0; --pixel) {
                int bit = pixel;
                if (s.XFlip) {
                    bit -= 7;
                    bit *= -1;
                }

                uint8_t colorIndex = ((data2 & (1 << bit)) ? 2 : 0) | 
                                    ((data1 & (1 << bit)) ? 1 : 0);
                
                palette_t * pal = (s.Palette ? &OBP1 : &OBP0);

                uint8_t color = 0;
                switch (colorIndex) {
                case 0b00:
                    colorIndex = pal->Color00;
                    break;
                case 0b01:
                    colorIndex = pal->Color01;
                    break;
                case 0b10:
                    colorIndex = pal->Color10;
                    break;
                case 0b11:
                    colorIndex = pal->Color11;
                    break;
                }

                switch (colorIndex) {
                case 0b00:
                    color = COLOR_WHITE;
                    break;
                case 0b01:
                    color = COLOR_LIGHT_GRAY;
                    break;
                case 0b10:
                    color = COLOR_DARK_GRAY;
                    break;
                case 0b11:
                    color = COLOR_BLACK;
                    break;
                }

                int x = -pixel + 7;

                unsigned off = (LY * 256*3) + (x * 3);
                Backbuffer[off + 0] = color;
                Backbuffer[off + 1] = color;
                Backbuffer[off + 2] = color;
            }
        }
    }
}

void updateCoincidence()
{
    STAT.Coincidence = (LY == LYC);
    if (STAT.IntCoincidence && STAT.Coincidence) {
        IF.STAT = true;
    }
}

void lcdTick(unsigned cycles)
{
    lcdModeTotalTicks += cycles;

    switch (STAT.Mode) {
        case STAT_MODE_SEARCH_SPRITE:
            if (lcdModeTotalTicks >= SEARCH_SPRITE_TICK_COUNT) {
                lcdModeTotalTicks -= SEARCH_SPRITE_TICK_COUNT;

                STAT.Mode = STAT_MODE_DATA_TRANSFER;
            }
        break;
        case STAT_MODE_DATA_TRANSFER:
            if (lcdModeTotalTicks >= DATA_TRANSFER_TICK_COUNT) {
                lcdModeTotalTicks -= DATA_TRANSFER_TICK_COUNT;

                if (LCDC.TileDisplayEnable) {
                    drawTiles();
                }

                if (LCDC.SpriteDisplayEnable) {
                    drawSprites();
                }

                STAT.Mode = STAT_MODE_HBLANK;
            }
        break;
        case STAT_MODE_HBLANK:
            if (lcdModeTotalTicks >= HBLANK_TICK_COUNT) {
                lcdModeTotalTicks -= HBLANK_TICK_COUNT;

                ++LY;
                updateCoincidence();

                if (LY == SCREEN_HEIGHT) {
                    STAT.Mode = STAT_MODE_VBLANK;
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
                        struct timespec delta = diff(last, now);
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
            if (lcdModeTotalTicks >= VBLANK_TICK_COUNT) {
                lcdModeTotalTicks -= VBLANK_TICK_COUNT;
                
                LY = (LY + 1) % MAX_LY;
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

void printLCDC()
{
    LogInfo("BGWinDisp=%d OBJDisp=%d OBJSize=%s BGTileMap=%s TileData=%s WinDisp=%d WinTileMap=%s LCDEnab=%d",
        LCDC.TileDisplayEnable, LCDC.SpriteDisplayEnable, 
        (LCDC.SpriteSize ? "8x16" : "8x8"),
        (LCDC.TileMapSelect ? "9C00h-9FFFh" : "9800h-9BFFh"),
        (LCDC.TileDataSelect ? "8000h-8FFFh" : "8800h-97FFh"),
        LCDC.WindowDisplayEnable,
        (LCDC.WindowTileMapSelect ? "9800h-9BFFh" : "9C00h-9FFFh"),
        LCDC.LCDEnable);
}

void printSTAT() 
{
    LogInfo("Mode=%s IntCoinc=%d IntHBlank=%d IntVBlank=%d IntSearchSprite=%d Coincidence=%d",
        LCD_MODE_STR[STAT.Mode], STAT.IntCoincidence, STAT.IntHBlank, STAT.IntVBlank, STAT.IntSearchSprite, STAT.Coincidence);
}

void printLCDInfo()
{
    LogInfo("SCY=%d SCX=%d LY=%d LYC=%d WX=%d WY=%d",
        SCY, SCX, LY, LYC, WX, WY);
}

void printBGP()
{
    LogInfo("BGP Color00=%d Color01=%d Color10=%d Color11=%d",
        BGP.Color00, BGP.Color01, BGP.Color10, BGP.Color11);
}

void printOBP0()
{
    LogInfo("OBP0 Color00=%d Color01=%d Color10=%d Color11=%d",
        OBP0.Color00, OBP0.Color01, OBP0.Color10, OBP0.Color11);
}

void printOBP1()
{
    LogInfo("OBP1 Color00=%d Color01=%d Color10=%d Color11=%d",
        OBP1.Color00, OBP1.Color01, OBP1.Color10, OBP1.Color11);
}
