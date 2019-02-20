#ifndef VIDEO_H
#define VIDEO_H

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

extern LCDC_t LCDC;
extern STAT_t STAT;
extern uint8_t SCY;
extern uint8_t SCX;
extern uint8_t LY;
extern uint8_t LYC;

enum {
    BLACK       = 0xFF000000,
    DARK_GREY   = 0xFF555555,
    LIGHT_GREY  = 0xFFAAAAAA,
    WHITE       = 0xFFFFFFFF,
};

void LCDTick();

#endif // VIDEO_H