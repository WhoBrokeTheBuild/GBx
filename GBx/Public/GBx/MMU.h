#ifndef GBX_MMU_H
#define GBX_MMU_H

#include <GBx/GBx.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t GBx_ReadByte(GBx * ctx, uint16_t address);

void GBx_WriteByte(GBx * ctx, uint16_t address, uint8_t data);

inline uint16_t GBx_ReadWord(GBx * ctx, uint16_t address)
{
    return GBx_ReadByte(ctx, address)
        | (GBx_ReadByte(ctx, address + 1) << 8);
}

inline void GBx_WriteWord(GBx * ctx, uint16_t address, uint16_t data)
{
    GBx_WriteByte(ctx, address + 1, (uint8_t)(data >> 8));
    GBx_WriteByte(ctx, address, (uint8_t)(data & 0xFF));
}

#ifdef __cplusplus
}
#endif

#endif // GBX_MMU_H