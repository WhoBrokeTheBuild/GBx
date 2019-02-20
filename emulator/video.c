#include "video.h"

LCDC_t LCDC = { .data = 0x91 };
STAT_t STAT;
uint8_t SCY = 0x00;
uint8_t SCX = 0x00;
uint8_t LY  = 0x00;
uint8_t LYC = 0x00;

uint32_t VBlankMS = 0;