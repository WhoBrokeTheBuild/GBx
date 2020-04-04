#ifndef GBX_PPU_H
#define GBX_PPU_H

#include <GBx/Types.h>

#define LCD_WIDTH  (160)
#define LCD_HEIGHT (144)
#define LCD_MAX_LY (LCD_HEIGHT + 10)

extern bool LCDUseOriginalColors;


#define VRAM_BANK_COUNT (2)
#define VRAM_BANK_SIZE  (0x2000)

extern uint8_t VRAM[VRAM_BANK_COUNT][VRAM_BANK_SIZE];

extern unsigned VRAMBank;

extern uint8_t OAM[0xA0];

#define LCD_BUFFER_WIDTH      (256)
#define LCD_BUFFER_HEIGHT     (256)
#define LCD_BUFFER_COMPONENTS (3) // RGB

#define LCD_BUFFER_SIZE \
    (LCD_BUFFER_WIDTH * LCD_BUFFER_HEIGHT * LCD_BUFFER_COMPONENTS)

extern uint8_t LCDBuffer[LCD_BUFFER_SIZE];

lcd_control_t LCDC;
lcd_status_t STAT;

uint8_t SCY;
uint8_t SCX;
uint8_t LY;
uint8_t LYC;
uint8_t WX;
uint8_t WY;

palette_t BGP;
palette_t OBP0;
palette_t OBP1;


void ResetLCD();

const uint8_t * GetColor(palette_t * pal, int bit, uint8_t d1, uint8_t d2);

void OAMDMATransfer(uint8_t data);

void DrawTiles();
void DrawSprites();

void LCDTick(unsigned cycles);

uint16_t GetTileMapAddress(unsigned index);
uint16_t GetTileDataAddress(unsigned index);

const char * GetLCDModeString(unsigned mode);
const char * GetTileMapRangeString(unsigned index);
const char * GetTileDataRangeString(unsigned index);
const char * GetSpriteSizeString(unsigned index);

void PrintLCDC();
void PrintSTAT();
void PrintPalette(const char * name, palette_t * pal);
void PrintLCDCoordinates();

#endif // GBX_PPU_H