#include <SM83/Context.h>
#include <SM83/Interrupts.h>

#include "Internal.h"

#include "Log.h"

void SM83_CallInterrupt(sm83_t * ctx, uint16_t addr)
{
    ctx->Enabled = true;
    ctx->IME = false;
    SM83_Tick(ctx, 2);

    SM83_PushWord(ctx, ctx->PC);
    SM83_Tick(ctx, 2);

    ctx->PC = addr;
    SM83_Tick(ctx, 1);
}

void SM83_CheckInterrupts(sm83_t * ctx)
{
    if (!ctx->IME) {
        return;
    }

    sm83_interrupt_flags_t enabledInts = {
        .raw = ctx->IE.raw & ctx->IF.raw,
    };

    if (enabledInts.Int40) {
        Verbose(ctx, 2, LogInfo("Interrupt 0x40"));

        ctx->IF.Int40 = false;
        SM83_CallInterrupt(ctx, 0x40);
    }

    if (enabledInts.Int48) {
        Verbose(ctx, 2, LogInfo("Interrupt 0x48"));

        ctx->IF.Int48 = false;
        SM83_CallInterrupt(ctx, 0x48);
    }

    if (enabledInts.Int50) {
        Verbose(ctx, 2, LogInfo("Interrupt 0x50"));

        ctx->IF.Int50 = false;
        SM83_CallInterrupt(ctx, 0x50);
    }

    if (enabledInts.Int58) {
        Verbose(ctx, 2, LogInfo("Interrupt 0x58"));

        ctx->IF.Int58 = false;
        SM83_CallInterrupt(ctx, 0x58);
    }

    if (enabledInts.Int60) {
        Verbose(ctx, 2, LogInfo("Interrupt 0x60"));

        ctx->IF.Int60 = false;
        SM83_CallInterrupt(ctx, 0x60);
    }
}

void SM83_PrintInterrupts(sm83_t * ctx)
{
    LogInfo("IME %s", (ctx->IME ? "Enabled" : "Disabled"));

    LogInfo("IF 0x40=%d 0x48=%d 0x50=%d 0x58=%d 0x60=%d",
        ctx->IF.Int40,
        ctx->IF.Int48,
        ctx->IF.Int50,
        ctx->IF.Int58,
        ctx->IF.Int60);

    LogInfo("IF 0x40=%d 0x48=%d 0x50=%d 0x58=%d 0x60=%d",
        ctx->IE.Int40,
        ctx->IE.Int48,
        ctx->IE.Int50,
        ctx->IE.Int58,
        ctx->IE.Int60);
}
