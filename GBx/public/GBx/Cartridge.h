#ifndef GBX_CARTRIDGE_H
#define GBX_CARTRIDGE_H

#include <GBx/Macros.h>
#include <GBx/Types.h>

#include <assert.h>

typedef struct gbx gbx_t;

void GBx_ResetCartridge(gbx_t * ctx);

bool GBx_LoadCartridge(gbx_t * ctx, const char * filename);

void GBx_WriteByteMBC(gbx_t * ctx, uint16_t address, uint8_t data);

const char * GBx_GetCartridgeTypeString(uint8_t type);
const char * GBx_GetROMTypeString(uint8_t type);
const char * GBx_GetRAMTypeString(uint8_t type);

void GBx_PrintCartridge(gbx_t * ctx);
void GBx_PrintMBC(gbx_t * ctx);

#endif // GBX_CARTRIDGE_H