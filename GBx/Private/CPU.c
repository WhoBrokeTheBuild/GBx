#include <GBx/CPU.h>

#include "Context.h"

GBx_CPU * GBx_GetCPU(GBx * ctx)
{
    return &ctx->CPU;
}

void GBx_CPU_Reset(GBx * ctx)
{
    ctx->CPU.Enabled = true;

    ctx->CPU.AF = 0x0000;
    ctx->CPU.BC = 0x0000;
    ctx->CPU.DE = 0x0000;
    ctx->CPU.HL = 0x0000;
    ctx->CPU.PC = 0x0000;
    ctx->CPU.SP = 0x0000;

    ctx->CPU.IME = true;
    ctx->CPU.RequestEnableIME = false;

    ctx->CPU.IF._raw = 0x00;
    ctx->CPU.IE._raw = 0x00;
}