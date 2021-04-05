#include <GBx/Timer.h>
#include <GBx/Context.h>

void GBx_Timer_Reset(GBx * ctx)
{
    ctx->DIV = 0x00;
    ctx->TIMA = 0x00;
    ctx->TMA = 0x00;

    ctx->TAC._raw = 0x00;

    ctx->TimerCycles = 0;
    ctx->DividerCycles = 0;
}

void GBx_Timer_Tick(GBx * ctx, unsigned cycles)
{
    ctx->DividerCycles += cycles;
    if (ctx->DividerCycles >= 0xFF) {
        ctx->DividerCycles -= 0xFF;
        ++ctx->DIV;
    }

    if (ctx->TAC.Enabled) {
        unsigned speed = GBx_GetClockSpeed(ctx);

        switch (ctx->TAC.Type) {
        case 0b00:
            speed /= 1024;
            break;
        case 0b01:
            speed /= 16;
            break;
        case 0b10:
            speed /= 64;
            break;
        case 0b11:
            speed /= 256;
            break;
        default:
            break; 
        }

        ctx->TimerCycles += cycles;
        if (ctx->TimerCycles >= speed) {
            ctx->TimerCycles -= speed;

            if (ctx->TIMA == 0xFF) {
                // Timer Rollover
                ctx->TIMA = ctx->TMA;
                ctx->IF.Int50 = 1;
            }
            else {
                ++ctx->TIMA;
            }
        }
    }
}
