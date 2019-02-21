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
        uint8_t Coincidence:1;
        uint8_t Mode00:1;
        uint8_t Mode01:1;
        uint8_t Mode10:1;
        uint8_t LYCLY:1;
        uint8_t _:1;
    };
    uint8_t data;
} STAT_t;

extern bool LCDEnabled;

extern LCDC_t LCDC;
extern STAT_t STAT;
extern uint8_t SCY;
extern uint8_t SCX;
extern uint8_t LY;
extern uint8_t LYC;

extern uint8_t CharacterRAM[0x1800];
extern uint8_t BGMapData1[0x400];
extern uint8_t BGMapData2[0x400];

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

void lcdTick();

#endif // VIDEO_H