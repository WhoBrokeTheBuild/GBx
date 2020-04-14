#ifndef GBX_PPU_H
#define GBX_PPU_H

#include <GBx/Types.h>

typedef struct gbx gbx_t;

void GBx_ResetPPU(gbx_t * ctx);

void GBx_TickPPU(gbx_t * ctx, unsigned cycles);

void GBx_DrawTiles(gbx_t * ctx);
void GBx_DrawSprites(gbx_t * ctx);

void GBx_DMATransferOAM(gbx_t * ctx, uint8_t data);

const uint8_t * GBx_GetPixelColor(gbx_t * ctx,
    gbx_palette_t * pal,
    int bit,
    uint8_t data1,
    uint8_t data2);

uint16_t GBx_GetTileMapAddress(unsigned index);
uint16_t GBx_GetTileDataAddress(unsigned index);

const char * GBx_GetPPUModeString(unsigned mode);
const char * GBx_GetTileMapRangeString(unsigned index);
const char * GBx_GetTileDataRangeString(unsigned index);
const char * GBx_GetSpriteSizeString(unsigned index);

void GBx_PrintLCDC(gbx_t * ctx);
void GBx_PrintSTAT(gbx_t * ctx);
void GBx_PrintPalettes(gbx_t * ctx);
void GBx_PrintLCDCoordinates(gbx_t * ctx);

#endif // GBX_PPU_H