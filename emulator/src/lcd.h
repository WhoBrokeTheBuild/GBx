#ifndef LCD_H
#define LCD_H

#include <stdbool.h>
#include <stdint.h>

typedef union {
    struct {
        bool TileDisplayEnable:1;
        bool SpriteDisplayEnable:1;
        bool SpriteSize:1;
        bool TileMapSelect:1;
        bool TileDataSelect:1;
        bool WindowDisplayEnable:1;
        bool WindowTileMapSelect:1;
        bool LCDEnable:1;
    };
    uint8_t data;
} LCDC_t;

typedef enum {
    STAT_MODE_HBLANK         = 0b00,
    STAT_MODE_VBLANK         = 0b01,
    STAT_MODE_SEARCH_SPRITE  = 0b10,
    STAT_MODE_DATA_TRANSFER  = 0b11,
} stat_mode_t;

typedef union {
    struct {
        stat_mode_t Mode:2;
        bool        IntCoincidence:1;
        bool        IntHBlank:1;
        bool        IntVBlank:1;
        bool        IntSearchSprite:1;
        bool        LYCLY:1;
        uint8_t     _:1;
    };
    uint8_t data;
} STAT_t;

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
                bool Priority:1;
                bool YFlip:1;
                bool XFlip:1;
                bool Palette:1;
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

extern bool FPSLimit;

void lcdInit();
void lcdTerm();

void lcdTick(unsigned cycles);

void printLCDC();
void printSTAT();
void printLCDInfo();
void printBGP();
void printOBP0();
void printOBP1();

#endif // LCD_H