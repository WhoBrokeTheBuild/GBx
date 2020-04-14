#include <GBx/GBx.h>

#include <stdlib.h>

#include "Internal.h"

gbx_t * GBx_Init()
{
    gbx_t * ctx = malloc(sizeof(gbx_t));

    ctx->CPU = SM83_Init();
    ctx->CPU->UserData = ctx;
    ctx->CPU->Tick = (sm83_tick_func_t)GBx_Tick;
    ctx->CPU->ReadByte = (sm83_read_byte_func_t)GBx_ReadByte;
    ctx->CPU->WriteByte = (sm83_write_byte_func_t)GBx_WriteByte;
    
    ctx->internal = malloc(sizeof(gbx_internal_t));

    GBx_Reset(ctx);

    GBx_InitDebug(ctx);

    return ctx;
}

void GBx_Term(gbx_t * ctx)
{
    GBx_TermDebug(ctx);
    
    free(ctx->internal);

    SM83_Term(ctx->CPU);

    free(ctx);
}

void GBx_Tick(gbx_t * ctx, unsigned cycles)
{
    GBx_TickAPU(ctx, cycles);
    GBx_TickPPU(ctx, cycles);
    GBx_TickTimer(ctx, cycles);
}

void GBx_Reset(gbx_t * ctx)
{
    SM83_Reset(ctx->CPU);

    if (!ctx->internal->HaveBootstrap) {
        GBx_StubBootstrap(ctx);
    }

    GBx_ResetCartridge(ctx);
    GBx_ResetAPU(ctx);
    GBx_ResetJoypad(ctx);
    GBx_ResetMMU(ctx);
    GBx_ResetSerial(ctx);
    GBx_ResetPPU(ctx);
    GBx_ResetTimer(ctx);
}