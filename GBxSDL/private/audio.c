#include "audio.h"

#include <GBx/GBx.h>

#include <SDL.h>

SDL_AudioSpec sdlAudioSpec;
SDL_AudioDeviceID sdlAudioDev;

void sdlAudioCallback(void * userdata, uint8_t * stream, int length)
{
    gbx_t * ctx = (gbx_t *)userdata;

    // uint16_t * data = (uint16_t *)stream;
    // int dataLength = length / sizeof(uint16_t);

    // uint16_t sample = (
    //     (DutyCycles[Tone1.Duty][Tone1DutyIndex] / 2) +
    //     (DutyCycles[Tone2.Duty][Tone2DutyIndex] / 2)
    // );

    // sample *= 2;

    // if (VolumeControl.LeftVinEnabled) {
    //     for (int i = 0; i < dataLength; i += 2) {
    //         data[i] = sample; 
    //     }
    // }
    // else {
    //     for (int i = 0; i < dataLength; i += 2) {
    //         data[i] = 0;
    //     }
    // }

    // if (VolumeControl.RightVinEnabled) {
    //     for (int i = 1; i < dataLength; i += 2) {
    //         data[i] = sample;
    //     }
    // }
    // else {
    //     for (int i = 1; i < dataLength; i += 2) {
    //         data[i] = 0;
    //     }
    // }
}

void AudioInit(gbx_t * ctx)
{
    // SDL_AudioSpec request;
    // memset(&request, 0, sizeof(request));

    // request.freq = 44100;
    // request.format = AUDIO_U16;
    // request.channels = 2;
    // request.samples = 128;
    // request.callback = sdlAudioCallback;
    // request.userdata = (void *)ctx;
    // sdlAudioDev = SDL_OpenAudioDevice(NULL, 0, &request, &sdlAudioSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    // printf("AudioDevice Frequency=%d Format=%d Channels=%d Samples=%d\n",
    //     sdlAudioSpec.freq,
    //     sdlAudioSpec.format,
    //     sdlAudioSpec.channels,
    //     sdlAudioSpec.samples
    // );
    
    // SDL_PauseAudioDevice(sdlAudioDev, true);
}

void AudioTerm(gbx_t * ctx)
{
    // SDL_CloseAudioDevice(sdlAudioDev);
}
