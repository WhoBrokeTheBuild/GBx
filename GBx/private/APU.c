#include <GBx/APU.h>

#include <GBx/Util.h>
#include <GBx/Log.h>

tone_control_t Tone1;

int Tone1Timer;
int Tone1DutyIndex;

tone_control_t Tone2;

int Tone2Timer;
int Tone2DutyIndex;

wave_control_t Wave;

int WaveTimer;

byte WaveRAM[0x10];

noise_control_t Noise;

int NoiseTimer;

volume_control_t VolumeControl;

volume_control_t VolumeControl;

sound_output_terminal_t SoundOutputTerminal;

sound_control_t APUC;

byte DutyCycles[4][8] = {
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00 },
};

void ResetAPU()
{
    // NR10-NR14
    Tone1.raw[0] = 0x80;
    Tone1.raw[1] = 0xBF;
    Tone1.raw[2] = 0xF3;
    Tone1.raw[4] = 0xBF;

    Tone1Timer = 0;
    Tone1DutyIndex = 0;

    // NR21-NR24
    Tone2.raw[0] = 0x3F;
    Tone2.raw[1] = 0x00;
    Tone2.raw[3] = 0xBF;

    Tone2Timer = 0;
    Tone2DutyIndex = 0;

    // NR30-NR34
    Wave.raw[0] = 0x7F;
    Wave.raw[1] = 0xFF;
    Wave.raw[2] = 0x9F;
    Wave.raw[3] = 0xBF;
    Wave.raw[4] = 0x00;

    WaveTimer = 0;

    // NR41-NR44
    Noise.raw[0] = 0xFF;
    Noise.raw[1] = 0x00;
    Noise.raw[2] = 0x00;
    Noise.raw[3] = 0xBF;

    NoiseTimer = 0;

    // NR50
    VolumeControl.raw = 0x77;

    // NR51
    SoundOutputTerminal.raw = 0xF3;

    // NR52
    APUC.raw = 0xF1;
}

void APUTick(unsigned cycles)
{
    Tone1Timer -= cycles;
    if (Tone1Timer <= 0) {
        Tone1DutyIndex = (Tone1DutyIndex + 1) % 8;
        Tone1Timer = Tone1.Frequency;
    }

    Tone2Timer -= cycles;
    if (Tone2Timer <= 0) {
        Tone2DutyIndex = (Tone2DutyIndex + 1) % 8;
        Tone2Timer = Tone2.Frequency;
    }
}

const char * GetVolumeCodeString(unsigned index)
{
    const char * STR[4] = {
        "0%",
        "100%",
        "50%",
        "25%",
    };

    return STR[index];
}

void PrintAPUC()
{
    LogInfo("APUC %s Tone1=%s Tone2=%s Wave=%s Noise=%s",
        GetEnabledString(APUC.Enabled),
        (APUC.Tone1Playing ? "Playing" : "Stopped"),
        (APUC.Tone2Playing ? "Playing" : "Stopped"),
        (APUC.WavePlaying ? "Playing" : "Stopped"),
        (APUC.NoisePlaying ? "Playing" : "Stopped"));
}

void PrintVolumeControl()
{
    LogInfo("VolumeControl RightVolume=%d RightVinEnable=%s LeftVolume=%d LeftVinEnable=%s",
        VolumeControl.RightVolume,
        (VolumeControl.RightVinEnabled ? "true" : "false"),
        VolumeControl.LeftVolume,
        (VolumeControl.LeftVinEnabled ? "true" : "false")
    );
}

void PrintSoundOutputTerminal()
{
    // TODO
}

void PrintTone1()
{
    LogInfo("Tone1 Shift=%d Negate=%s SweepPeriod=%d LengthLoad=%d Duty=%d Period=%d EnvelopeAddMode=%s Volume=%d Frequency=%d LengthEnabled=%s Trigger=%s",
        Tone1.Shift,
        GetEnabledString(Tone1.Negate),
        Tone1.SweepPeriod,
        Tone1.LengthLoad, Tone1.Duty, Tone1.Period,
        (Tone1.EnvelopeAddMode ? "true" : "false"),
        Tone1.Volume, Tone1.Frequency,
        GetEnabledString(Tone1.LengthEnabled),
        GetEnabledString(Tone1.Trigger)
    );
}

void PrintTone2()
{
    LogInfo("Tone2 LengthLoad=%d Duty=%d Period=%d EnvelopeAddMode=%s Volume=%d Frequency=%d LengthEnabled=%s Trigger=%s",
        Tone2.LengthLoad,
        Tone2.Duty,
        Tone2.Period,
        (Tone2.EnvelopeAddMode ? "true" : "false"),
        Tone2.Volume,
        Tone2.Frequency,
        GetEnabledString(Tone2.LengthEnabled),
        GetEnabledString(Tone2.Trigger)
    );
}

void PrintWave()
{
    LogInfo("Wave DACPower=%s LengthLoad=%d VolumeCode=%s Frequency=%d LengthEnabled=%s Trigger=%s",
        GetEnabledString(Wave.DACPower),
        Wave.LengthLoad, 
        GetVolumeCodeString(Wave.VolumeCode),
        Wave.Frequency,
        GetEnabledString(Wave.LengthEnabled),
        GetEnabledString(Wave.Trigger)
    );
}

void PrintWaveRAM()
{
    LogInfo("WaveRAM %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        WaveRAM[0], WaveRAM[1], WaveRAM[2], WaveRAM[3], WaveRAM[4], WaveRAM[5], WaveRAM[6], WaveRAM[7], 
        WaveRAM[8], WaveRAM[9], WaveRAM[10], WaveRAM[11], WaveRAM[12], WaveRAM[13], WaveRAM[14], WaveRAM[15]
    );
}


void PrintNoise()
{
    LogInfo("Noise LengthLoad=%d Period=%d EnvelopeAddMode=%s Volume=%d DivisorCode=%d LFSRWidthMode=%s ClockShift=%d LengthEnabled=%s Trigger=%s",
        Noise.LengthLoad,
        Noise.Period,
        (Noise.EnvelopeAddMode ? "true" : "false"),
        Noise.Volume,
        Noise.DivisorCode,
        (Noise.LFSRWidthMode ? "true" : "false"),
        Noise.ClockShift,
        GetEnabledString(Noise.LengthEnabled),
        GetEnabledString(Noise.Trigger)
    );
}