#ifndef VIDEO_H
#define VIDEO_H

#include "log.h"
#include <stdint.h>
#include <stdbool.h>

typedef union {
    struct {
        bool BGWindowDisplay:1;
        bool SpriteDisplay:1;
        uint8_t SpriteSize:1;
        uint8_t BGTileMapSelect:1;
        uint8_t BGTileDataSelect:1;
        bool WindowDisplay:1;
        uint8_t WindowTileMapSelect:1;
        bool LCDEnable:1;
    };
    uint8_t data;
} LCDC_t;

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

extern LCDC_t LCDC;
extern STAT_t STAT;
extern uint8_t SCY;
extern uint8_t SCX;
extern uint8_t LY;
extern uint8_t LYC;

extern uint8_t CharacterRAM[0x1800];
extern uint8_t BGMapData1[0x400];
extern uint8_t BGMapData2[0x400];

extern uint64_t LCDTicks;

enum {
    BLACK       = 0xFF000000,
    DARK_GREY   = 0xFF555555,
    LIGHT_GREY  = 0xFFAAAAAA,
    WHITE       = 0xFFFFFFFF,
};

static void printLCDC()
{
    LogDebug("BGWinDisp=%d OBJDisp=%d OBJSize=%d BGTileMap=%s TileData=%s WinDisp=%d WinTileMap=%s LCDEnab=%d",
        LCDC.BGWindowDisplay, LCDC.SpriteDisplay, LCDC.SpriteSize,
        (LCDC.BGTileMapSelect ? "9C00h-9FFFh" : "9800h-9BFFh"),
        (LCDC.BGTileDataSelect ? "8000h-8FFFh" : "8800h-97FFh"),
        LCDC.WindowDisplay,
        (LCDC.WindowTileMapSelect ? "9C00h-9FFFh" : "9800h-9BFFh"),
        LCDC.LCDEnable);
}

void lcdInit();
void lcdTerm();

void lcdTick(unsigned cycles);

#endif // VIDEO_H