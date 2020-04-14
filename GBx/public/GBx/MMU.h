#ifndef GBX_MMU_H
#define GBX_MMU_H

#include <GBx/Types.h>

typedef struct gbx gbx_t;

void GBx_ResetMMU(gbx_t * ctx);

uint8_t GBx_ReadByte(gbx_t * ctx, uint16_t address);

void GBx_WriteByte(gbx_t * ctx, uint16_t address, uint8_t data);

uint16_t GBx_ReadWord(gbx_t * ctx, uint16_t address);

void GBx_WriteWord(gbx_t * ctx, uint16_t address, uint16_t data);

#endif // GBX_MMU_H