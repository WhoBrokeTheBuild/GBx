#include <GBx/APU.h>

#include "Context.h"

GBx_APU * GBx_GetAPU(GBx * ctx)
{
    return &ctx->APU;
}

void GBx_APU_Reset(GBx * ctx)
{
    ctx->APU.Volume = 0.5f;
}