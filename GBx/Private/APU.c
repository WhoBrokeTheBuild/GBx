#include <GBx/APU.h>
#include <GBx/Context.h>

#include <string.h>

void GBx_APU_Reset(GBx * ctx)
{
    memset(ctx->WaveRAM, 0, sizeof(ctx->WaveRAM));

    memset(ctx->Tone1._raw, 0, sizeof(ctx->Tone1));
    memset(ctx->Tone2._raw, 0, sizeof(ctx->Tone2));
    memset(ctx->Wave._raw, 0, sizeof(ctx->Wave));
    memset(ctx->Noise._raw, 0, sizeof(ctx->Noise));

    ctx->VolumeControl._raw = 0x77;

    ctx->OutputTerminalControl._raw = 0xFF;

    ctx->APUC._raw = 0x80;
}