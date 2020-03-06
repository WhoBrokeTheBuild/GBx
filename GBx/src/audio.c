#include "audio.h"

#include <GBx/apu.h>
#include <GBx/log.h>

#include <SDL.h>

SDL_AudioSpec sdlAudioSpec;
SDL_AudioDeviceID sdlAudioDev;

void sdlAudioCallback(void * userdata, uint8_t * stream, int length)
{
    uint8_t sample = (
        (DutyCycles[Tone1.Duty][Tone1DutyIndex] / 2) +
        (DutyCycles[Tone2.Duty][Tone2DutyIndex] / 2)
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

void audioInit()
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

void audioTerm()
{
    SDL_CloseAudioDevice(sdlAudioDev);
}
