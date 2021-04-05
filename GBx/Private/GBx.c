#include <GBx/GBx.h>
#include <GBx/Context.h>
#include <GBx/APU.h>
#include <GBx/CPU.h>
#include <GBx/PPU.h>
#include <GBx/Cartridge.h>
#include <GBx/Bootstrap.h>
#include <GBx/Log.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void GBx_GetVersion(int * major, int * minor, int * patch)
{
    assert(major);
    assert(minor);
    assert(patch);

    *major = GBX_VERSION_MAJOR;
    *minor = GBX_VERSION_MINOR;
    *patch = GBX_VERSION_PATCH;
}

const char * GBx_GetVersionString()
{
    return GBX_VERSION_STRING;
}

bool GBx_Init(GBx ** pctx)
{
    assert(pctx);

    if (!*pctx) {
        *pctx = (GBx *)malloc(sizeof(GBx));
    }

    if (!*pctx) {
        return false;
    }

    GBx * ctx = *pctx;

    ctx->Model = GBX_MODEL_DMG;

    GBxLogInfo("Initializing");

    GBx_Bootstrap_Init(ctx);
    GBx_Cartridge_Init(ctx);
    GBx_Serial_Init(ctx);

    GBx_APU_Reset(ctx);
    GBx_CPU_Reset(ctx);
    GBx_PPU_Reset(ctx);
    GBx_Joypad_Reset(ctx);
    GBx_Serial_Reset(ctx);
    GBx_Timer_Reset(ctx);

    ctx->MemoryReadTracker = NULL;
    ctx->MemoryWriteTracker = NULL;

    return true;
}

void GBx_Reset(GBx * ctx)
{
    assert(ctx);

    GBxLogInfo("Resetting");

    GBx_Bootstrap_Reset(ctx);
    GBx_Cartridge_Reset(ctx);
    GBx_APU_Reset(ctx);
    GBx_CPU_Reset(ctx);
    GBx_PPU_Reset(ctx);
    GBx_Joypad_Reset(ctx);
    GBx_Serial_Reset(ctx);
    GBx_Timer_Reset(ctx);

    GBx_CPU_Print(ctx);

    if (ctx->MemoryReadTracker) {
        memset(ctx->MemoryReadTracker, 0, 0xFFFF);
    }

    if (ctx->MemoryWriteTracker) {
        memset(ctx->MemoryWriteTracker, 0, 0xFFFF);
    }
}

void GBx_Term(GBx ** ctx)
{
    GBxLogInfo("Terminate");

    if (*ctx) {
        free(*ctx);
        *ctx = NULL;
    }
}

void GBx_SetModel(GBx * ctx, GBx_Model model)
{
    assert(ctx);
    
    // TODO: ?
    ctx->Model = model;
}

unsigned GBx_GetClockSpeed(GBx * ctx)
{
    assert(ctx);
    
    switch (ctx->Model) {
    case GBX_MODEL_SGB:
        return GBX_CLOCK_SPEED_SGB;
    case GBX_MODEL_CGB:
        // TODO: Double Speed
        return GBX_CLOCK_SPEED_DMG;
    default:
        return GBX_CLOCK_SPEED_DMG;
    }
}

void GBx_EnableMemoryTracking(GBx * ctx)
{
    if (!ctx->MemoryReadTracker) {
        ctx->MemoryReadTracker = (uint8_t *)malloc(0xFFFF);
    }

    if (!ctx->MemoryWriteTracker) {
        ctx->MemoryWriteTracker = (uint8_t *)malloc(0xFFFF);
    }
}

unsigned GBx_Step(GBx * ctx)
{
    unsigned cycles = GBx_CPU_Step(ctx);
    
    GBx_APU_Tick(ctx, cycles);
    GBx_PPU_Tick(ctx, cycles);
    GBx_Serial_Tick(ctx, cycles);
    GBx_Timer_Tick(ctx, cycles);

    return cycles;
}

void GBx_Frame(GBx * ctx)
{
    // TODO:

    // const unsigned STEPS = 17556;
    const unsigned STEPS = 1000000;

    for (unsigned i = 0; i < STEPS;) {
        i += GBx_Step(ctx);
    }

    // GBx_CPU_Print(ctx);
}