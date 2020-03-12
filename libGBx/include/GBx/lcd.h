#ifndef LCD_H
#define LCD_H

#include <stdbool.h>
#include <stdint.h>

typedef union
{
    struct {
        bool     TileDisplayEnable:1;
        bool     SpriteDisplayEnable:1;
        unsigned SpriteSize:1;
        unsigned TileMapSelect:1;
        unsigned TileDataSelect:1;
        bool     WindowDisplayEnable:1;
        unsigned WindowTileMapSelect:1;
        bool     LCDEnable:1;
    };
    uint8_t raw;

} lcd_control_t;

typedef union
{
    struct {
        unsigned Mode:2;
        bool     Coincidence:1;
        bool     IntHBlank:1;
        bool     IntVBlank:1;
        bool     IntSearchSprite:1;
        bool     IntCoincidence:1;
        unsigned :1;
    };
    uint8_t raw;

} lcd_status_t;

#define STAT_WRITE_MASK (0b01111000)

#define STAT_MODE_HBLANK         (0b00)
#define STAT_MODE_VBLANK         (0b01)
#define STAT_MODE_SEARCH_SPRITE  (0b10)
#define STAT_MODE_DATA_TRANSFER  (0b11)

typedef union
{
    struct {
        unsigned Color0:2;
        unsigned Color1:2;
        unsigned Color2:2;
        unsigned Color3:2;
    };
    uint8_t raw;

} palette_t;

typedef union
{
    struct {
        uint8_t Y;
        uint8_t X;
        uint8_t Pattern;
        union {
            struct {
                bool     Priority:1;
                bool     YFlip:1;
                bool     XFlip:1;
                bool     Palette:1;
                unsigned :4;
            };
            uint8_t Attributes;
        };
    };
    uint32_t raw;

} sprite_t;

// FF40 - LCD Control

extern lcd_control_t LCDC;

// FF41 - LCD Status

extern lcd_status_t STAT;

// FF42 - Scroll Y

extern uint8_t SCY;

// FF43 - Scroll X

extern uint8_t SCX;

// FF44 - LCD Y Coordinate

extern uint8_t LY;

// FF45 - LY Compare

extern uint8_t LYC;

// FF4A - Window Y Position

extern uint8_t WX;

// FF4B - Window X Position

extern uint8_t WY;

// FF47 - BG Palette Data

extern palette_t BGP;

// FF48 - Object Palette 0 Data

extern palette_t OBP0;

// FF49 - Object Palette 1 Data

extern palette_t OBP1;

// VRAM Bank 0

extern uint8_t VRAM0[0x1FFF];

// VRAM Bank 1

extern uint8_t VRAM1[0x1FFF];

// VRAM Switchable Bank - 8000-9FFF

extern uint8_t * VRAM;

// Object Attribute Memory - FE00-FE9F

extern uint8_t OAM[0xA0];

#define BACKBUFFER_WIDTH  (256)
#define BACKBUFFER_HEIGHT (256)
#define BACKBUFFER_COMP   (3)

#define SCREEN_WIDTH  (160)
#define SCREEN_HEIGHT (144)

extern uint8_t Backbuffer[BACKBUFFER_WIDTH * BACKBUFFER_HEIGHT * BACKBUFFER_COMP];

void lcdTick(unsigned cycles);

void printLCDC();
void printSTAT();
void printLCDInfo();
void printBGP();
void printOBP0();
void printOBP1();

#endif // LCD_H