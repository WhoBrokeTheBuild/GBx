#ifndef VIDEO_H
#define VIDEO_H

#include "log.h"
#include <stdint.h>
#include <stdbool.h>

typedef union {
    struct {
        bool TileDisplayEnable:1;
        bool SpriteDisplayEnable:1;
        uint8_t SpriteSize:1;
        uint8_t TileMapSelect:1;
        uint8_t TileDataSelect:1;
        bool WindowDisplayEnable:1;
        uint8_t WindowTileMapSelect:1;
        bool LCDEnable:1;
    };
    uint8_t data;
} LCDC_t;

#define SPRITE_SIZE_8X8  0
#define SPRITE_SIZE_8X16 1

#define TILE_MAP_9800_9BFF 0
#define TILE_MAP_9C00_9FFF 1

#define TILE_DATA_8800_97FF 0
#define TILE_DATA_8000_8FFF 1

typedef union {
    struct {
        uint8_t Mode:2;
        bool Coincidence:1;
        bool IntHBlank:1;
        bool IntVBlank:1;
        bool IntSearchSprite:1;
        bool IntLYCLY:1;
        uint8_t _:1;
    };
    uint8_t data;
} STAT_t;

#define MODE_HBLANK         0b00
#define MODE_VBLANK         0b01
#define MODE_SEARCH_SPRITE  0b10
#define MODE_DATA_TRANSFER  0b11

typedef union {
    struct {
        uint8_t Color00:2;
        uint8_t Color01:2;
        uint8_t Color10:2;
        uint8_t Color11:2;
    };
    uint8_t data;
} palette_t;

typedef union {
    struct {
        uint8_t Y;
        uint8_t X;
        uint8_t Pattern;
        union {
            struct {
                uint8_t Priority:1;
                uint8_t YFlip:1;
                uint8_t XFlip:1;
                uint8_t Palette:1;
                uint8_t _:4;
            };
            uint8_t Attributes;
        };
    };
    uint32_t data;
} sprite_t;

extern LCDC_t LCDC;
extern STAT_t STAT;
extern uint8_t SCY;
extern uint8_t SCX;
extern uint8_t LY;
extern uint8_t LYC;
extern palette_t BGP;
extern palette_t OBP0;
extern palette_t OBP1;
extern uint8_t WX;
extern uint8_t WY;

extern uint8_t CharacterRAM[0x1800];
extern uint8_t BGMapData1[0x400];
extern uint8_t BGMapData2[0x400];
extern uint8_t OAM[0xA0];

extern uint64_t LCDTicks;

static void printLCDC()
{
    LogDebug("BGWinDisp=%d OBJDisp=%d OBJSize=%d BGTileMap=%s TileData=%s WinDisp=%d WinTileMap=%s LCDEnab=%d",
        LCDC.TileDisplayEnable, LCDC.SpriteDisplayEnable, LCDC.SpriteSize,
        (LCDC.TileMapSelect == 0 ? "9800h-9BFFh" : "9C00h-9FFFh"),
        (LCDC.TileDataSelect == 0 ? "8800h-97FFh" : "8000h-8FFFh"),
        LCDC.WindowDisplayEnable,
        (LCDC.WindowTileMapSelect == 0 ? "9C00h-9FFFh" : "9800h-9BFFh"),
        LCDC.LCDEnable);
}

static void printSTAT() 
{
    const char * mode[4] = {
        "HBlank",
        "VBlank",
        "SearchSprite",
        "DataTransfer",
    };

    LogDebug("Mode=%s Coinc=%d IntHBlank=%d IntVBlank=%d IntSearchSprite=%d IntLYCLY=%d",
        mode[STAT.Mode], STAT.Coincidence, STAT.IntHBlank, STAT.IntVBlank, STAT.IntSearchSprite, STAT.IntLYCLY);
}

static void printLCDInfo()
{
    LogDebug("SCY=%d SCX=%d LY=%d LYC=%d WX=%d WY=%d",
        SCY, SCX, LY, LYC, WX, WY);
}

void lcdInit();
void lcdTerm();

void lcdTick(unsigned cycles);

#endif // VIDEO_H