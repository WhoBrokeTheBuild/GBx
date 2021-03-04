#ifndef GBX_MMU_H
#define GBX_MMU_H

#include <GBx/GBx.h>
#include <GBx/Context.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t GBx_ReadByte(GBx * ctx, uint16_t address);

void GBx_WriteByte(GBx * ctx, uint16_t address, uint8_t data);

static GBX_INLINE uint16_t GBx_ReadWord(GBx * ctx, uint16_t address)
{
    return GBx_ReadByte(ctx, address)
        | (GBx_ReadByte(ctx, address + 1) << 8);
}

static GBX_INLINE void GBx_WriteWord(GBx * ctx, uint16_t address, uint16_t data)
{
    GBx_WriteByte(ctx, address + 1, (uint8_t)(data >> 8));
    GBx_WriteByte(ctx, address, (uint8_t)(data & 0xFF));
}

static GBX_INLINE uint8_t GBx_NextByte(GBx * ctx)
{
    uint8_t data = GBx_ReadByte(ctx, ctx->PC);
    ++ctx->PC;
    return data;
}

static GBX_INLINE uint16_t GBx_NextWord(GBx * ctx)
{
    uint16_t data = GBx_ReadWord(ctx, ctx->PC);
    ctx->PC += 2;
    return data;
}

static GBX_INLINE void GBx_PushWord(GBx * ctx, uint16_t data)
{
    ctx->SP -= 2;
    GBx_WriteWord(ctx, ctx->SP, data);
}

static GBX_INLINE uint16_t GBx_PopWord(GBx * ctx)
{
    uint16_t data = GBx_ReadWord(ctx, ctx->SP);
    ctx->SP += 2;
    return data;
}

#ifdef __cplusplus
}
#endif

#endif // GBX_MMU_H