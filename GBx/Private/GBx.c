#include <GBx/GBx.h>

#include "Context.h"

#include <stdlib.h>
#include <string.h>

int GBx_Init(GBx ** pctx, GBx_Config * cfg)
{
    GBx * ctx = *pctx;

    clog_init();

    if (!ctx) {
        ctx = (GBx *)malloc(sizeof(GBx));
    }

    if (!ctx) {
        return GBX_OUT_OF_MEMORY;
    }

    ctx->Mode = GBX_MODE_DMG;

    GBx_APU_Reset(ctx);
    GBx_CPU_Reset(ctx);
    GBx_PPU_Reset(ctx);
    GBx_Cartridge_Reset(ctx);
    GBx_Bootstrap_Reset(ctx);

    *pctx = ctx;
    return GBX_SUCCESS;
}

void GBx_Term(GBx ** ctx)
{
    if (*ctx) {
        free(*ctx);
        *ctx = NULL;
    }

    clog_term();
}

void GBx_Tick(GBx * ctx, unsigned cycles)
{

}

void GBx_Step(GBx * ctx)
{

}

void GBx_Frame(GBx * ctx)
{

}