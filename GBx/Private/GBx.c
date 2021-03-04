#include <GBx/GBx.h>
#include <GBx/Context.h>
#include <GBx/APU.h>
#include <GBx/CPU.h>
#include <GBx/PPU.h>
#include <GBx/Cartridge.h>
#include <GBx/Bootstrap.h>
#include <GBx/Log.h>

#include <stdlib.h>
#include <string.h>

bool GBx_Init(GBx ** pctx, GBx_Config * cfg)
{
    GBx * ctx = *pctx;

    clog_init();

    if (!ctx) {
        ctx = (GBx *)malloc(sizeof(GBx));
    }

    if (!ctx) {
        return false;
    }

    ctx->Mode = GBX_MODE_DMG;

    GBx_Bootstrap_Init(ctx);
    GBx_Cartridge_Init(ctx);

    GBx_APU_Reset(ctx);
    GBx_CPU_Reset(ctx);
    GBx_PPU_Reset(ctx);

    *pctx = ctx;
    return true;
}

void GBx_Reset(GBx * ctx)
{
    GBx_Bootstrap_Reset(ctx);
    GBx_Cartridge_Reset(ctx);
    GBx_APU_Reset(ctx);
    GBx_CPU_Reset(ctx);
    GBx_PPU_Reset(ctx);
}

void GBx_Term(GBx ** ctx)
{
    if (*ctx) {
        free(*ctx);
        *ctx = NULL;
    }

    clog_term();
}

void GBx_Step(GBx * ctx)
{
    GBx_CPU_Step(ctx);
}

void GBx_Frame(GBx * ctx)
{
    // TODO:

    while (ctx->PC < 0x0100) {
        GBx_CPU_Step(ctx);
        GBx_CPU_Print(ctx);
    }

    exit(0);
}