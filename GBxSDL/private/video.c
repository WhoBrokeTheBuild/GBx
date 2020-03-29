#include "video.h"
#include "debug.h"

#include <GBx/GBx.h>

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
    
    sdlRenderer = SDL_CreateRenderer(sdlWindow, 0, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!sdlRenderer) {
        LogFatal("Failed to create SDL2 Renderer, %s", SDL_GetError());
    }
    
    sdlTexture = SDL_CreateTexture(sdlRenderer, 
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 
        LCD_BUFFER_WIDTH, LCD_BUFFER_HEIGHT);

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    
    LogInfo("Window Size=(%dx%d) LCDBuffer=(%dx%d)", 
        width, height, 
        LCD_BUFFER_WIDTH, LCD_BUFFER_HEIGHT);
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
        else {
            HandleEvent(&evt);

            if (DebugEnabled) {
                DebugWindowHandleEvent(&evt);
            }
        }
    }
}

void HandleEvent(SDL_Event * evt)
{
    if (evt->type == SDL_WINDOWEVENT) {
        SDL_Window * window = SDL_GetWindowFromID(evt->window.windowID);
        if (window == sdlWindow) {
            switch (evt->window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                // TODO: Shutdown gracefully
                exit(0);
                break;
            }
        }
    }
    else if (evt->type == SDL_KEYUP) {
        SDL_Window * window = SDL_GetWindowFromID(evt->key.windowID);
        if (window == sdlWindow) {
            switch (evt->key.keysym.sym) {
            case SDLK_1:
                if (DebugEnabled) {
                    ToggleDebugWindow();
                }
                break;
            case SDLK_0:
                LCDUseOriginalColors ^= true;
                break;

            case SDLK_RIGHT:
                RightPressed = false;
                break;
            case SDLK_LEFT:
                LeftPressed = false;
                break;
            case SDLK_UP:
                UpPressed = false;
                break;
            case SDLK_DOWN:
                DownPressed = false;
                break;
            case SDLK_a:
                APressed = false;
                break;
            case SDLK_s:
                BPressed = false;
                break;
            case SDLK_d:
                SelectPressed = false;
                break;
            case SDLK_f:
                StartPressed = false;
                break;
            }
        }
    }
    else if (evt->type == SDL_KEYDOWN) {
        SDL_Window * window = SDL_GetWindowFromID(evt->key.windowID);
        if (window == sdlWindow) {
            switch (evt->key.keysym.sym) {
            case SDLK_RIGHT:
                RightPressed = true;
                break;
            case SDLK_LEFT:
                LeftPressed = true;
                break;
            case SDLK_UP:
                UpPressed = true;
                break;
            case SDLK_DOWN:
                DownPressed = true;
                break;
            case SDLK_a:
                APressed = true;
                break;
            case SDLK_s:
                BPressed = true;
                break;
            case SDLK_d:
                SelectPressed = true;
                break;
            case SDLK_f:
                StartPressed = true;
                break;
            }
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
    
    SDL_Rect src = { .x = 0, .y = 0, .w = LCD_WIDTH, .h = LCD_HEIGHT };
    SDL_Rect dst = { .x = 0, .y = 0, .w = LCD_WIDTH, .h = LCD_HEIGHT };

    float scale = 1.0;
    if (width > height) {
        scale = height / (double)LCD_HEIGHT;
        dst.x = (width - (scale * LCD_WIDTH)) / 2;
    }
    else {
        scale = width / (double)LCD_WIDTH;
        dst.y = (height - (scale * LCD_HEIGHT)) / 2;
    }

    dst.w = LCD_WIDTH * scale;
    dst.h = LCD_HEIGHT * scale;

    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, &src, &dst);
    SDL_RenderPresent(sdlRenderer);
}
