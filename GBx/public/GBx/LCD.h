#ifndef LCD_H
#define LCD_H

#include <GBx/Types.h>

#define LCD_WIDTH  (160)
#define LCD_HEIGHT (144)
#define LCD_MAX_LY (LCD_HEIGHT + 10)

extern bool LCDUseOriginalColors;

typedef union
{
    struct {
        bool TileDisplayEnabled:1;
        bool SpriteDisplayEnabled:1;
        byte SpriteSize:1;
        byte BGTileMapSelect:1;
        byte TileDataSelect:1;
        bool WindowDisplayEnabled:1;
        byte WindowTileMapSelect:1;
        bool Enabled:1;
    };
    byte raw;

} lcd_control_t;

typedef union
{
    struct {
        byte Mode:2;
        bool Coincidence:1;
        bool IntHBlank:1;
        bool IntVBlank:1;
        bool IntSearchSprite:1;
        bool IntCoincidence:1;
        byte :1;
    };
    byte raw;

} lcd_status_t;

#define STAT_WRITE_MASK (0b01111000)

#define STAT_MODE_HBLANK        (0)
#define STAT_MODE_VBLANK        (1)
#define STAT_MODE_SEARCH_SPRITE (2)
#define STAT_MODE_DATA_TRANSFER (3)

typedef union
{
    struct {
        byte Color0:2;
        byte Color1:2;
        byte Color2:2;
        byte Color3:2;
    };
    byte raw;

} palette_t;

typedef union
{
    struct {
        bool Priority:1;
        bool YFlip:1;
        bool XFlip:1;
        bool Palette:1;
        byte :4;
    };
    byte raw;

} sprite_attrib_t;

// $FF40 - LCD Control
extern lcd_control_t LCDC;

// $FF41 - LCD Status
extern lcd_status_t STAT;

// $FF42 - Scroll Y
extern byte SCY;

// $FF43 - Scroll X
extern byte SCX;

// $FF44 - LCD Y Coordinate
extern byte LY;

// $FF45 - LY Compare
extern byte LYC;

// $FF4A - Window Y Position
extern byte WX;

// $FF4B - Window X Position
extern byte WY;

// $FF47 - BG Palette Data
extern palette_t BGP;

// $FF48 - Object Palette 0 Data
extern palette_t OBP0;

// $FF49 - Object Palette 1 Data
extern palette_t OBP1;

#define VRAM_BANK_COUNT (2)
#define VRAM_BANK_SIZE  (0x2000)

extern byte VRAM[VRAM_BANK_COUNT][VRAM_BANK_SIZE];

extern unsigned VRAMBank;

extern byte OAM[0xA0];

#define LCD_BUFFER_WIDTH      (256)
#define LCD_BUFFER_HEIGHT     (256)
#define LCD_BUFFER_COMPONENTS (3) // RGB

#define LCD_BUFFER_SIZE \
    (LCD_BUFFER_WIDTH * LCD_BUFFER_HEIGHT * LCD_BUFFER_COMPONENTS)

extern byte LCDBuffer[LCD_BUFFER_SIZE];

void ResetLCD();

const byte * GetColor(palette_t * pal, int bit, byte d1, byte d2);

void OAMDMATransfer(byte data);

void DrawTiles();
void DrawSprites();

void LCDTick(unsigned cycles);

word GetTileMapAddress(unsigned index);
word GetTileDataAddress(unsigned index);

const char * GetLCDModeString(unsigned mode);
const char * GetTileMapRangeString(unsigned index);
const char * GetTileDataRangeString(unsigned index);
const char * GetSpriteSizeString(unsigned index);

void PrintLCDC();
void PrintSTAT();
void PrintPalette(const char * name, palette_t * pal);
void PrintLCDCoordinates();

#endif // LCD_H