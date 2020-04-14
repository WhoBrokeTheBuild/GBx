#include <GBx/Timer.h>

#include "Internal.h"
#include "Log.h"

int GBx_GetTimerSpeed(gbx_t * ctx)
{
    int clockSpeed = SM83_GetClockSpeed(ctx->CPU);

    switch (ctx->TAC.Type) {
    case 0b00:
        // 4096 Hz
        return clockSpeed / 1024;
    case 0b01:
        // 262144 Hz
        return clockSpeed / 16;
    case 0b10:
        // 65536 Hz
        return clockSpeed / 64;
    case 0b11:
        // 16384 Hz
        return clockSpeed / 256;
    default:
        break;
    }

    return 0;
}

void GBx_ResetTimer(gbx_t * ctx)
{
    ctx->TAC.raw = 0x00;

    ctx->DIV = 0;
    ctx->TIMA = 0;
    ctx->TMA = 0;

    ctx->internal->TimerCounter = 0;
    ctx->internal->DIVCounter = 0;
}

void GBx_TickTimer(gbx_t * ctx, unsigned cycles)
{
    gbx_internal_t * inctx = ctx->internal;

    inctx->DIVCounter += cycles;
    if (inctx->DIVCounter >= 0xFF) {
        inctx->DIVCounter -= 0xFF;
        ++ctx->DIV;
    }

    if (!ctx->TAC.Enabled) {
        return;
    }

    int speed = GBx_GetTimerSpeed(ctx);

    inctx->TimerCounter += cycles;
    if (inctx->TimerCounter > speed) {
        inctx->TimerCounter -= speed;

        if (ctx->TIMA == 0xFF) {
            Verbose(ctx, 2, LogInfo("Timer Rollover"));
            ctx->TIMA = ctx->TMA;
            ctx->CPU->IF.Int50 = true;
        }
        else {
            ++ctx->TIMA;
        }
    }
}

void GBx_PrintTimer(gbx_t * ctx)
{
    LogInfo("Timer %s Type=%d (%d Hz) TIMA=%d TMA=%d DIV=%d",
        GBx_GetEnabledString(ctx->TAC.Enabled),
        ctx->TAC.Type,
        GBx_GetTimerSpeed(ctx),
        ctx->TIMA,
        ctx->TMA,
        ctx->DIV);
}
