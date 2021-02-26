#include <GBx/PPU.h>

#include "Context.h"

#include <string.h>

GBx_PPU * GBx_GetPPU(GBx * ctx)
{
    return &ctx->PPU;
}

void GBx_PPU_Reset(GBx * ctx)
{
    memset(ctx->PPU.Pixels, 32, sizeof(ctx->PPU.Pixels));
}