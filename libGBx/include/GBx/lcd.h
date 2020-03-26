#ifndef LCD_H
#define LCD_H

#include "types.h"

#define LCD_WIDTH  (160)
#define LCD_HEIGHT (144)
#define LCD_MAX_LY (LCD_HEIGHT + 10)

extern bool LCDOriginalColors;

#define LCD_BUFFER_WIDTH      (256)
#define LCD_BUFFER_HEIGHT     (256)
#define LCD_BUFFER_COMPONENTS (3) // RGB

#define LCD_BUFFER_SIZE \
    (LCD_BUFFER_WIDTH * LCD_BUFFER_HEIGHT * LCD_BUFFER_COMPONENTS)

#define HBLANK_TICK_COUNT        (204)
#define SEARCH_SPRITE_TICK_COUNT (80)
#define DATA_TRANSFER_TICK_COUNT (172)

// 1/10th of VBlank period, or one line worth of ticks
#define VBLANK_TICK_COUNT \
    (HBLANK_TICK_COUNT + SEARCH_SPRITE_TICK_COUNT + DATA_TRANSFER_TICK_COUNT)

#define TILE_WIDTH     (8)
#define TILE_HEIGHT    (8)
#define TILES_PER_ROW  (32)
#define TILE_DATA_SIZE (16)

extern byte LCDBuffer[LCD_BUFFER_SIZE];

static const word TILE_MAP_ADDR[2] = {
    0x9800, // 9800-9BFF
    0x9C00, // 9C00-9FFF
};

static const word TILE_DATA_ADDR[2] = {
    0x9000, // 8800-97FF
    0x8000, // 8000-8FFF
};

typedef union
{
    struct {
        bool TileDisplayEnabled:1;
        bool SpriteDisplayEnabled:1;
        uint SpriteSize:1;
        uint BGTileMapSelect:1;
        uint TileDataSelect:1;
        bool WindowDisplayEnabled:1;
        uint WindowTileMapSelect:1;
        bool Enabled:1;
    };
    byte raw;

} lcd_control;

typedef union
{
    struct {
        uint Mode:2;
        bool Coincidence:1;
        bool IntHBlank:1;
        bool IntVBlank:1;
        bool IntSearchSprite:1;
        bool IntCoincidence:1;
        uint :1;
    };
    byte raw;

} lcd_status;

typedef union
{
    struct {
        uint Color0:2;
        uint Color1:2;
        uint Color2:2;
        uint Color3:2;
    };
    byte raw;

} palette;

typedef union
{
    struct {
        byte Y;
        byte X;
        byte Pattern;
        union {
            struct {
                bool Priority:1;
                bool YFlip:1;
                bool XFlip:1;
                bool Palette:1;
                uint :4;
            };
            byte Attributes;
        };
    };
    dword raw;

} sprite;

// FF40 - LCD Control

extern lcd_control LCDC;

// FF41 - LCD Status

extern lcd_status STAT;

#define STAT_WRITE_MASK (0b01111000)

#define STAT_MODE_HBLANK         (0)
#define STAT_MODE_VBLANK         (1)
#define STAT_MODE_SEARCH_SPRITE  (2)
#define STAT_MODE_DATA_TRANSFER  (3)

// FF42 - Scroll Y

extern byte SCY;

// FF43 - Scroll X

extern byte SCX;

// FF44 - LCD Y Coordinate

extern byte LY;

// FF45 - LY Compare

extern byte LYC;

// FF4A - Window Y Position

extern byte WX;

// FF4B - Window X Position

extern byte WY;

// FF47 - BG Palette Data

extern palette BGP;

// FF48 - Object Palette 0 Data

extern palette OBP0;

// FF49 - Object Palette 1 Data

extern palette OBP1;

// VRAM Switchable Bank - 8000-9FFF

#define VRAM_BANK_COUNT (2)
#define VRAM_BANK_SIZE  (0x2000)

extern byte VRAM[VRAM_BANK_COUNT][VRAM_BANK_SIZE];

extern uint VRAMBank;

// Object Attribute Memory - FE00-FE9F

extern byte OAM[0xA0];

void ResetLCD();

byte * GetColor(palette * pal, int bit, byte d1, byte d2);

const char * GetLCDModeString(uint mode);

void DrawTiles();
void DrawSprites();

void LCDTick(uint cycles);

void PrintLCDC();
void PrintSTAT();
void PrintPalettes();
void PrintLCDCoordinates();

#endif // LCD_H