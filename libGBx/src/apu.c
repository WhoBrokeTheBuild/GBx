#include "apu.h"

#include "log.h"

tone_t Tone1;
tone_t Tone2;

wave_t Wave;

noise_t Noise;

volume_control_t VolumeControl;

uint8_t WaveRAM[0x10];

int Tone1Timer = 0;
int Tone1DutyIndex = 0;

int Tone2Timer = 0;
int Tone2DutyIndex = 0;

int WaveTimer = 0;
int NoiseTimer = 0;

uint8_t DutyCycles[4][8] = {
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00 },
};

void apuTick(unsigned cycles)
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

void printTone1()
{
    LogInfo("Tone1 Shift=%d Negate=%s SweepPeriod=%d LengthLoad=%d Duty=%d Period=%d EnvelopeAddMode=%s Volume=%d Frequency=%d LengthEnable=%s Trigger=%s",
        Tone1.Shift,
        (Tone1.Negate ? "true" : "false"),
        Tone1.SweepPeriod,
        Tone1.LengthLoad, Tone1.Duty, Tone1.Period,
        (Tone1.EnvelopeAddMode ? "true" : "false"),
        Tone1.Volume, Tone1.Frequency,
        (Tone1.LengthEnable ? "true" : "false"),
        (Tone1.Trigger ? "true" : "false")
    );
}

void printTone2()
{
    LogInfo("Tone2 LengthLoad=%d Duty=%d Period=%d EnvelopeAddMode=%s Volume=%d Frequency=%d LengthEnable=%s Trigger=%s",
        Tone2.LengthLoad, Tone2.Duty, Tone2.Period,
        (Tone2.EnvelopeAddMode ? "true" : "false"),
        Tone2.Volume, Tone2.Frequency,
        (Tone2.LengthEnable ? "true" : "false"),
        (Tone2.Trigger ? "true" : "false")
    );
}

void printWave()
{
    const char * volume[4] = {
        "0%",
        "100%",
        "50%",
        "25%",
    };

    LogInfo("Wave DACPower=%s LengthLoad=%d VolumeCode=%s Frequency=%d LengthEnable=%s Trigger=%s",
        (Wave.DACPower ? "true" : "false"),
        Wave.LengthLoad, volume[Wave.VolumeCode],
        Wave.Frequency,
        (Wave.LengthEnable ? "true" : "false"),
        (Wave.Trigger ? "true" : "false")
    );
}

void printNoise()
{
    LogInfo("Noise LengthLoad=%d Period=%d EnvelopeAddMode=%s Volume=%d DivisorCode=%d LFSRWidthMode=%s ClockShift=%d LengthEnable=%s Trigger=%s",
        Noise.LengthLoad, Noise.Period,
        (Noise.EnvelopeAddMode ? "true" : "false"),
        Noise.Volume, Noise.DivisorCode,
        (Noise.LFSRWidthMode ? "true" : "false"),
        Noise.ClockShift,
        (Tone2.LengthEnable ? "true" : "false"),
        (Tone2.Trigger ? "true" : "false")
    );
}

void printVolumeControl()
{
    LogInfo("VolumeControl RightVolume=%d RightVinEnable=%s LeftVolume=%d LeftVinEnable=%s",
        VolumeControl.RightVolume,
        (VolumeControl.RightVinEnable ? "true" : "false"),
        VolumeControl.LeftVolume,
        (VolumeControl.LeftVinEnable ? "true" : "false")
    );
}

void printWaveRAM()
{
    LogInfo("WaveRAM %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
        WaveRAM[0], WaveRAM[1], WaveRAM[2], WaveRAM[3], WaveRAM[4], WaveRAM[5], WaveRAM[6], WaveRAM[7], 
        WaveRAM[8], WaveRAM[9], WaveRAM[10], WaveRAM[11], WaveRAM[12], WaveRAM[13], WaveRAM[14], WaveRAM[15]
    );
}
