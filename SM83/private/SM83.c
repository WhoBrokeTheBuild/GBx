#include <SM83/SM83.h>

#include <assert.h>
#include <stdlib.h>

#include "Internal.h"
#include "Log.h"

sm83_t * SM83_Init()
{
    sm83_t * ctx = malloc(sizeof(sm83_t));
    assert(ctx);

    ctx->VerboseLevel = 0;

    SM83_Reset(ctx);

    ctx->internal = malloc(sizeof(sm83_internal_t));
    assert(ctx->internal);

    ctx->internal->TotalTicks = 0;

    ctx->internal->InstructionLoggingEnabled = false;
    ctx->internal->LastInstructionAddress = ctx->PC;
    ctx->internal->StackLoggingEnabled = false;

    SM83_SetMode(ctx, SM83_MODE_DMG);

    return ctx;
}

void SM83_Term(sm83_t * ctx)
{
    free(ctx->internal);
    free(ctx);
}

void SM83_Reset(sm83_t * ctx)
{
    ctx->Enabled = true;

    ctx->AF = 0x0000;
    ctx->BC = 0x0000;
    ctx->DE = 0x0000;
    ctx->HL = 0x0000;
    ctx->PC = 0x0000;
    ctx->SP = 0x0000;

    ctx->IME = true;
    ctx->RequestEnableIME = false;

    ctx->IF.raw = 0x00;
    ctx->IE.raw = 0x00;
}

bool SM83_GetEnabled(sm83_t * ctx)
{
    return ctx->Enabled;
}

void SM83_SetEnabled(sm83_t * ctx, bool enabled)
{
    ctx->Enabled = enabled;
}

sm83_mode_t SM83_GetMode(sm83_t * ctx)
{
    return ctx->internal->Mode;
}

void SM83_SetMode(sm83_t * ctx, sm83_mode_t mode)
{
    static const int DMG_CLOCK_SPEED = 4194304; // Hz
    static const int SGB_CLOCK_SPEED = 4295454; // Hz
    static const int CGB_CLOCK_SPEED = 8388608; // Hz

    ctx->internal->Mode = mode;

    switch (mode) {
    case SM83_MODE_DMG:
        ctx->internal->ClockSpeed = DMG_CLOCK_SPEED;
        break;
    case SM83_MODE_SGB:
        ctx->internal->ClockSpeed = SGB_CLOCK_SPEED;
        break;
    case SM83_MODE_CGB:
        ctx->internal->ClockSpeed = CGB_CLOCK_SPEED;
        break;
    }
}

int SM83_GetClockSpeed(sm83_t * ctx)
{
    return ctx->internal->ClockSpeed;
}