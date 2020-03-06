#include "video.h"

#include <GBx/cartridge.h>
#include <GBx/lcd.h>
#include <GBx/log.h>

#include <SDL.h>

#define DEFAULT_SCALE (4)

unsigned winWidth  = SCREEN_WIDTH  * DEFAULT_SCALE;
unsigned winHeight = SCREEN_HEIGHT * DEFAULT_SCALE;

SDL_Window   * sdlWindow   = NULL;
SDL_Renderer * sdlRenderer = NULL;
SDL_Texture  * sdlTexture  = NULL;

void videoInit()
{
    char windowTitle[21];
    snprintf(windowTitle, sizeof(windowTitle), "GBx - %.*s", 15, CartridgeHeader.Title);

    sdlWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, SDL_WINDOW_RESIZABLE);
    if (!sdlWindow) {
        LogFatal("Failed to create SDL2 Window, %s", SDL_GetError());
    }
    
    sdlRenderer = SDL_CreateRenderer(sdlWindow, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRenderer) {
        LogFatal("Failed to create SDL2 Renderer, %s", SDL_GetError());
    }
    
    sdlTexture = SDL_CreateTexture(sdlRenderer, 
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 
        BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    
    LogInfo("Window Size=(%dx%d) Backbuffer=(%dx%d)", winWidth, winHeight, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);

    render();
}

void videoTerm()
{
    SDL_DestroyTexture(sdlTexture);

    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
}

void pollEvents() 
{
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            // TODO: Shutdown gracefully
            exit(0);
            break;
        }
        else if (evt.type == SDL_WINDOWEVENT) {
            switch (evt.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                winWidth = evt.window.data1;
                winHeight = evt.window.data2;
                break;
            default:
                break;
            };
        }
    }
}

void render()
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTexture, NULL, (void **)&pixels, &pitch);
    memcpy(pixels, Backbuffer, sizeof(Backbuffer));
    SDL_UnlockTexture(sdlTexture);
    
    SDL_Rect src = { .x = 0, .y = 0, .w = 160, .h = 144 };
    SDL_Rect dst = { .x = 0, .y = 0, .w = 160, .h = 144 };

    float scale = 1.0;
    if (winWidth > winHeight) {
        scale = winHeight / 144.0;
        dst.x = (winWidth - (scale * 160)) / 2;
    }
    else {
        scale = winWidth / 160.0;
        dst.y = (winHeight - (scale * 144)) / 2;
    }

    dst.w = 160 * scale;
    dst.h = 144 * scale;

    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, &src, &dst);
    SDL_RenderPresent(sdlRenderer);
}