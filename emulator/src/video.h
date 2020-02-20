#ifndef VIDEO_H
#define VIDEO_H

#include "log.h"
#include <stdint.h>
#include <stdbool.h>

typedef union {
    struct {
        bool    TileDisplayEnable:1;
        bool    SpriteDisplayEnable:1;
        uint8_t SpriteSize:1;
        uint8_t TileMapSelect:1;
        uint8_t TileDataSelect:1;
        bool    WindowDisplayEnable:1;
        uint8_t WindowTileMapSelect:1;
        bool    LCDEnable:1;
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
        bool    IntCoincidence:1;
        bool    IntHBlank:1;
        bool    IntVBlank:1;
        bool    IntSearchSprite:1;
        bool    LYCLY:1;
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

extern uint8_t VideoRAM0[0x1FFF];
extern uint8_t VideoRAM1[0x1FFF];
extern uint8_t OAM[0xA0];

extern uint64_t LCDTicks;

extern bool FPSLimit;

void videoInit();
void videoTerm();

void videoTick(unsigned cycles);

void printLCDC();
void printSTAT();
void printLCDInfo();

#endif // VIDEO_H