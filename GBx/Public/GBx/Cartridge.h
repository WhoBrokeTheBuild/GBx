#ifndef GBX_CARTRIDGE_H
#define GBX_CARTRIDGE_H

#include <GBx/GBx.h>
#include <GBx/Types/Cartridge.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void GBx_Cartridge_Init(GBx * ctx);

void GBx_Cartridge_Reset(GBx * ctx);

bool GBx_Cartridge_Load(GBx * ctx, const char * filename);

const char * GBx_Cartridge_GetTypeString(uint8_t type);

const char * GBx_Cartridge_GetROMTypeString(uint8_t type);

const char * GBx_Cartridge_GetSRAMTypeString(uint8_t type);

void GBx_Cartridge_PrintHeader(GBx * ctx);

void GBx_Cartridge_PrintMBC(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_CARTRIDGE_H