#include "apu.h"

#include "log.h"
#include "math.h"

#include <SDL.h>

tone_t Tone1;
tone_t Tone2;

wave_t Wave;

noise_t Noise;

volume_control_t VolumeControl;

uint8_t WaveRAM[0x10];

int tone1Timer = 0;
int tone1DutyIndex = 0;

int tone2Timer = 0;
int tone2DutyIndex = 0;

int waveTimer = 0;
int noiseTimer = 0;

SDL_AudioSpec sdlAudioSpec;
SDL_AudioDeviceID sdlAudioDev;

uint8_t duties[4][8] = {
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00 },
};

#include <time.h>
#include <math.h>

void sdlAudioCallback(void * userdata, uint8_t * stream, int length)
{
    uint8_t sample = (
        (duties[Tone1.Duty][tone1DutyIndex] / 2) +
        (duties[Tone2.Duty][tone2DutyIndex] / 2)
    );

    if (VolumeControl.LeftVinEnable) {
        for (int i = 0; i < length; i += 2) {
            stream[i] = sample; 
        }
    }
    else {
        for (int i = 0; i < length; i += 2) {
            stream[i] = 0;
        }
    }

    if (VolumeControl.RightVinEnable) {
        for (int i = 1; i < length; i += 2) {
            stream[i] = 0;
        }
    }
    else {
        for (int i = 1; i < length; i += 2) {
            stream[i] = 0;
        }
    }
}

void apuInit()
{
    SDL_AudioSpec request;
    memset(&request, 0, sizeof(request));

    request.freq = 44100 / 10;
    request.format = AUDIO_U8;
    request.channels = 2;
    request.samples = 128;
    request.callback = sdlAudioCallback;
    sdlAudioDev = SDL_OpenAudioDevice(NULL, 0, &request, &sdlAudioSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    LogInfo("AudioDevice Frequency=%d Format=%d Channels=%d Samples=%d",
        sdlAudioSpec.freq,
        sdlAudioSpec.format,
        sdlAudioSpec.channels,
        sdlAudioSpec.samples
    );

    // SDL_PauseAudioDevice(sdlAudioDev, false);
}

void apuTerm()
{
    SDL_CloseAudioDevice(sdlAudioDev);
}

void apuTick(unsigned cycles)
{
    tone1Timer -= cycles;
    if (tone1Timer <= 0) {
        tone1DutyIndex = (tone1DutyIndex + 1) % 8;
        tone1Timer = Tone1.Frequency;
    }

    tone2Timer -= cycles;
    if (tone2Timer <= 0) {
        tone2DutyIndex = (tone2DutyIndex + 1) % 8;
        tone2Timer = Tone2.Frequency;
    }
}

void apuPause()
{
    SDL_PauseAudioDevice(sdlAudioDev, true);
}

void apuPlay()
{
    SDL_PauseAudioDevice(sdlAudioDev, false);
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
