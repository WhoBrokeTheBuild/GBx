#include <GBx/PPU.h>
#include <GBx/Context.h>

#include <string.h>

void GBx_PPU_Reset(GBx * ctx)
{
    memset(ctx->Pixels, 32, sizeof(ctx->Pixels));
}