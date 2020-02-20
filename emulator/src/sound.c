#include "sound.h"
#include "log.h"

square_wave_t Wave1;
square_wave_t Wave2;
wave_t Wave3;
noise_t Noise;
volume_control_t VolumeControl;

uint8_t WaveRAM[0x10];

void printWave1()
{
    LogInfo("Wave1 Shift=%d Negate=%s SweepPeriod=%d LengthLoad=%d Duty=%d Period=%d EnvelopeAddMode=%s Volume=%d Frequency=%d LengthEnable=%s Trigger=%s",
        Wave1.Shift,
        (Wave1.Negate ? "true" : "false"),
        Wave1.SweepPeriod,
        Wave1.LengthLoad, Wave1.Duty, Wave1.Period,
        (Wave1.EnvelopeAddMode ? "true" : "false"),
        Wave1.Volume, Wave1.Frequency,
        (Wave1.LengthEnable ? "true" : "false"),
        (Wave1.Trigger ? "true" : "false")
    );
}

void printWave2()
{
    LogInfo("Wave2 LengthLoad=%d Duty=%d Period=%d EnvelopeAddMode=%s Volume=%d Frequency=%d LengthEnable=%s Trigger=%s",
        Wave2.LengthLoad, Wave2.Duty, Wave2.Period,
        (Wave2.EnvelopeAddMode ? "true" : "false"),
        Wave2.Volume, Wave2.Frequency,
        (Wave2.LengthEnable ? "true" : "false"),
        (Wave2.Trigger ? "true" : "false")
    );
}

void printWave3()
{
    const char * volume[4] = {
        "0%",
        "100%",
        "50%",
        "25%",
    };

    LogInfo("Wave3 DACPower=%s LengthLoad=%d VolumeCode=%s Frequency=%d LengthEnable=%s Trigger=%s",
        (Wave3.DACPower ? "true" : "false"),
        Wave3.LengthLoad, volume[Wave3.VolumeCode],
        Wave3.Frequency,
        (Wave3.LengthEnable ? "true" : "false"),
        (Wave3.Trigger ? "true" : "false")
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
        (Wave2.LengthEnable ? "true" : "false"),
        (Wave2.Trigger ? "true" : "false")
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
