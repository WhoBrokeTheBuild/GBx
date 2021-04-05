#include <GBx/Interrupt.h>
#include <GBx/Context.h>
#include <GBx/MMU.h>

unsigned GBx_CallInterrupt(GBx * ctx, uint16_t addr)
{
    ctx->CPUEnabled = true;
    ctx->IME = false;

    GBx_PushWord(ctx, ctx->PC);

    ctx->PC = addr;

    return 4;
}

unsigned GBx_CheckInterrupts(GBx * ctx)
{
    if (!ctx->IME) {
        return 0;
    }

    unsigned cycles = 0;

    GBx_InterruptFlags enabledInterrupts = {
        ._raw = ctx->IE._raw & ctx->IF._raw,
    };

    // TODO: Verbose Logging

    if (enabledInterrupts.Int40) {
        ctx->IF.Int40 = false;
        cycles += GBx_CallInterrupt(ctx, 0x0040);
    }

    if (enabledInterrupts.Int48) {
        ctx->IF.Int48 = false;
        cycles += GBx_CallInterrupt(ctx, 0x0048);
    }

    if (enabledInterrupts.Int50) {
        ctx->IF.Int50 = false;
        cycles += GBx_CallInterrupt(ctx, 0x0050);
    }

    if (enabledInterrupts.Int58) {
        ctx->IF.Int58 = false;
        cycles += GBx_CallInterrupt(ctx, 0x0058);
    }

    if (enabledInterrupts.Int60) {
        ctx->IF.Int60 = false;
        cycles += GBx_CallInterrupt(ctx, 0x0060);
    }

    return cycles;
}
