#include <GBx/APU.h>

#include "Internal.h"
#include "Log.h"

// uint8_t DutyCycles[4][8] = {
//     { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
//     { 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
//     { 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 },
//     { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00 },
// };

void GBx_ResetAPU(gbx_t * ctx)
{
    gbx_internal_t * inctx = ctx->internal;

    // NR10-NR14
    ctx->Tone1.raw[0] = 0x80;
    ctx->Tone1.raw[1] = 0xBF;
    ctx->Tone1.raw[2] = 0xF3;
    ctx->Tone1.raw[4] = 0xBF;

    inctx->Tone1Timer = 0;
    inctx->Tone1DutyIndex = 0;

    // NR21-NR24
    ctx->Tone2.raw[0] = 0x3F;
    ctx->Tone2.raw[1] = 0x00;
    ctx->Tone2.raw[3] = 0xBF;

    inctx->Tone2Timer = 0;
    inctx->Tone2DutyIndex = 0;

    // NR30-NR34
    ctx->Wave.raw[0] = 0x7F;
    ctx->Wave.raw[1] = 0xFF;
    ctx->Wave.raw[2] = 0x9F;
    ctx->Wave.raw[3] = 0xBF;
    ctx->Wave.raw[4] = 0x00;

    inctx->WaveTimer = 0;

    // NR41-NR44
    ctx->Noise.raw[0] = 0xFF;
    ctx->Noise.raw[1] = 0x00;
    ctx->Noise.raw[2] = 0x00;
    ctx->Noise.raw[3] = 0xBF;

    inctx->NoiseTimer = 0;

    // NR50
    ctx->VolumeControl.raw = 0x77;

    // NR51
    ctx->SoundOutputTerminal.raw = 0xF3;

    // NR52
    ctx->APUC.raw = 0xF1;
}

void GBx_TickAPU(gbx_t * ctx, unsigned cycles)
{
    gbx_internal_t * inctx = ctx->internal;

    inctx->Tone1Timer -= cycles;
    if (inctx->Tone1Timer <= 0) {
        inctx->Tone1DutyIndex = (inctx->Tone1DutyIndex + 1) % 8;
        inctx->Tone1Timer = ctx->Tone1.Frequency;
    }

    inctx->Tone2Timer -= cycles;
    if (inctx->Tone2Timer <= 0) {
        inctx->Tone2DutyIndex = (inctx->Tone2DutyIndex + 1) % 8;
        inctx->Tone2Timer = ctx->Tone2.Frequency;
    }
}

const char * GBx_GetVolumeCodeString(unsigned index)
{
    const char * STR[4] = {
        "0%",
        "100%",
        "50%",
        "25%",
    };

    return STR[index];
}

void GBx_PrintAPUC(gbx_t * ctx)
{
    LogInfo("APUC %s Tone1=%s Tone2=%s Wave=%s Noise=%s",
        GBx_GetEnabledString(ctx->APUC.Enabled),
        (ctx->APUC.Tone1Playing ? "Playing" : "Stopped"),
        (ctx->APUC.Tone2Playing ? "Playing" : "Stopped"),
        (ctx->APUC.WavePlaying ? "Playing" : "Stopped"),
        (ctx->APUC.NoisePlaying ? "Playing" : "Stopped"));
}

void GBx_PrintVolumeControl(gbx_t * ctx)
{
    LogInfo("VolumeControl RightVolume=%d RightVinEnable=%s LeftVolume=%d "
            "LeftVinEnable=%s",
        ctx->VolumeControl.RightVolume,
        (ctx->VolumeControl.RightVinEnabled ? "true" : "false"),
        ctx->VolumeControl.LeftVolume,
        (ctx->VolumeControl.LeftVinEnabled ? "true" : "false"));
}

void GBx_PrintSoundOutputTerminal(gbx_t * ctx)
{
    // TODO
}

void GBx_PrintTone1(gbx_t * ctx)
{
    LogInfo("Tone1 Shift=%d Negate=%s SweepPeriod=%d LengthLoad=%d Duty=%d "
            "Period=%d EnvelopeAddMode=%s Volume=%d Frequency=%d "
            "LengthEnabled=%s Trigger=%s",
        ctx->Tone1.Shift,
        GBx_GetEnabledString(ctx->Tone1.Negate),
        ctx->Tone1.SweepPeriod,
        ctx->Tone1.LengthLoad,
        ctx->Tone1.Duty,
        ctx->Tone1.Period,
        (ctx->Tone1.EnvelopeAddMode ? "true" : "false"),
        ctx->Tone1.Volume,
        ctx->Tone1.Frequency,
        GBx_GetEnabledString(ctx->Tone1.LengthEnabled),
        GBx_GetEnabledString(ctx->Tone1.Trigger));
}

void GBx_PrintTone2(gbx_t * ctx)
{
    LogInfo("Tone2 LengthLoad=%d Duty=%d Period=%d EnvelopeAddMode=%s "
            "Volume=%d Frequency=%d LengthEnabled=%s Trigger=%s",
        ctx->Tone2.LengthLoad,
        ctx->Tone2.Duty,
        ctx->Tone2.Period,
        (ctx->Tone2.EnvelopeAddMode ? "true" : "false"),
        ctx->Tone2.Volume,
        ctx->Tone2.Frequency,
        GBx_GetEnabledString(ctx->Tone2.LengthEnabled),
        GBx_GetEnabledString(ctx->Tone2.Trigger));
}

void GBx_PrintWave(gbx_t * ctx)
{
    LogInfo("Wave DACPower=%s LengthLoad=%d VolumeCode=%s Frequency=%d "
            "LengthEnabled=%s Trigger=%s",
        GBx_GetEnabledString(ctx->Wave.DACPower),
        ctx->Wave.LengthLoad,
        GBx_GetVolumeCodeString(ctx->Wave.VolumeCode),
        ctx->Wave.Frequency,
        GBx_GetEnabledString(ctx->Wave.LengthEnabled),
        GBx_GetEnabledString(ctx->Wave.Trigger));
}

void GBx_PrintWaveRAM(gbx_t * ctx)
{
    LogInfo("WaveRAM "
            "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        ctx->WaveRAM[0],
        ctx->WaveRAM[1],
        ctx->WaveRAM[2],
        ctx->WaveRAM[3],
        ctx->WaveRAM[4],
        ctx->WaveRAM[5],
        ctx->WaveRAM[6],
        ctx->WaveRAM[7],
        ctx->WaveRAM[8],
        ctx->WaveRAM[9],
        ctx->WaveRAM[10],
        ctx->WaveRAM[11],
        ctx->WaveRAM[12],
        ctx->WaveRAM[13],
        ctx->WaveRAM[14],
        ctx->WaveRAM[15]);
}

void GBx_PrintNoise(gbx_t * ctx)
{
    LogInfo("Noise LengthLoad=%d Period=%d EnvelopeAddMode=%s Volume=%d "
            "DivisorCode=%d LFSRWidthMode=%s ClockShift=%d LengthEnabled=%s "
            "Trigger=%s",
        ctx->Noise.LengthLoad,
        ctx->Noise.Period,
        (ctx->Noise.EnvelopeAddMode ? "true" : "false"),
        ctx->Noise.Volume,
        ctx->Noise.DivisorCode,
        (ctx->Noise.LFSRWidthMode ? "true" : "false"),
        ctx->Noise.ClockShift,
        GBx_GetEnabledString(ctx->Noise.LengthEnabled),
        GBx_GetEnabledString(ctx->Noise.Trigger));
}