#include "video.h"

#include <GBx/cartridge.h>
#include <GBx/lcd.h>
#include <GBx/log.h>

#include <SDL.h>

SDL_Window   * sdlWindow   = NULL;
SDL_Renderer * sdlRenderer = NULL;
SDL_Texture  * sdlTexture  = NULL;

void VideoInit(int scale)
{
    char windowTitle[21];
    snprintf(windowTitle, sizeof(windowTitle), "GBx - %.*s", 15, CartridgeHeader.Title);

    int width  = LCD_WIDTH * scale;
    int height = LCD_HEIGHT * scale;

    sdlWindow = SDL_CreateWindow(windowTitle, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, SDL_WINDOW_RESIZABLE);
    
    if (!sdlWindow) {
        LogFatal("Failed to create SDL2 Window, %s", SDL_GetError());
    }
    
    sdlRenderer = SDL_CreateRenderer(sdlWindow, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRenderer) {
        LogFatal("Failed to create SDL2 Renderer, %s", SDL_GetError());
    }
    
    sdlTexture = SDL_CreateTexture(sdlRenderer, 
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 
        LCD_BUFFER_WIDTH, LCD_BUFFER_HEIGHT);

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    
    LogInfo("Window Size=(%dx%d) LCDBuffer=(%dx%d)", 
        width, height, 
        LCD_BUFFER_WIDTH, LCD_BUFFER_HEIGHT);

    Render();
}

void VideoTerm()
{
    SDL_DestroyTexture(sdlTexture);

    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
}

void PollEvents() 
{
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            // TODO: Shutdown gracefully
            exit(0);
            break;
        }
    }
}

void Render()
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTexture, NULL, (void **)&pixels, &pitch);
    memcpy(pixels, LCDBuffer, sizeof(LCDBuffer));
    SDL_UnlockTexture(sdlTexture);

    int width, height;
    SDL_GetWindowSize(sdlWindow, &width, &height);
    
    SDL_Rect src = { .x = 0, .y = 0, .w = 160, .h = 144 };
    SDL_Rect dst = { .x = 0, .y = 0, .w = 160, .h = 144 };

    float scale = 1.0;
    if (width > height) {
        scale = height / 144.0;
        dst.x = (width - (scale * 160)) / 2;
    }
    else {
        scale = width / 160.0;
        dst.y = (height - (scale * 144)) / 2;
    }

    dst.w = 160 * scale;
    dst.h = 144 * scale;

    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, &src, &dst);
    SDL_RenderPresent(sdlRenderer);
}