#include "video.h"
#include "debug.h"

SDL_Window   * sdlWindow   = NULL;
SDL_Renderer * sdlRenderer = NULL;
SDL_Texture  * sdlTexture  = NULL;

void VideoInit(gbx_t * ctx, int scale)
{
    char windowTitle[21];
    snprintf(windowTitle, sizeof(windowTitle), "GBx - %.*s", 15, ctx->CartridgeHeader.Title);

    int width  = GBX_LCD_WIDTH * scale;
    int height = GBX_LCD_HEIGHT * scale;

    sdlWindow = SDL_CreateWindow(windowTitle, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, SDL_WINDOW_RESIZABLE);
    
    if (!sdlWindow) {
        fprintf(stderr, "Failed to create SDL2 Window, %s\n", SDL_GetError());
        exit(1);
    }
    
    sdlRenderer = SDL_CreateRenderer(sdlWindow, 0, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!sdlRenderer) {
        fprintf(stderr, "Failed to create SDL2 Renderer, %s\n", SDL_GetError());
        exit(1);
    }
    
    sdlTexture = SDL_CreateTexture(sdlRenderer, 
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 
        GBX_BACKBUFFER_WIDTH, GBX_BACKBUFFER_HEIGHT);

    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    
    printf("Window Size=(%dx%d) LCDBuffer=(%dx%d)\n",
        width, height, 
        GBX_BACKBUFFER_WIDTH, GBX_BACKBUFFER_HEIGHT);
}

void VideoTerm(gbx_t * ctx)
{
    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
}

void PollEvents(gbx_t * ctx) 
{
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            // TODO: Shutdown gracefully
            exit(0);
            break;
        }
        else {
            HandleEvent(ctx, &evt);

            if (DebugEnabled) {
                DebugWindowHandleEvent(ctx, &evt);
            }
        }
    }
}

void HandleEvent(gbx_t * ctx, SDL_Event * evt)
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
                ctx->UseOriginalColors ^= true;
                break;

            case SDLK_RIGHT:
                ctx->RightPressed = false;
                break;
            case SDLK_LEFT:
                ctx->LeftPressed = false;
                break;
            case SDLK_UP:
                ctx->UpPressed = false;
                break;
            case SDLK_DOWN:
                ctx->DownPressed = false;
                break;
            case SDLK_a:
                ctx->APressed = false;
                break;
            case SDLK_s:
                ctx->BPressed = false;
                break;
            case SDLK_d:
                ctx->SelectPressed = false;
                break;
            case SDLK_f:
                ctx->StartPressed = false;
                break;
            }
        }
    }
    else if (evt->type == SDL_KEYDOWN) {
        SDL_Window * window = SDL_GetWindowFromID(evt->key.windowID);
        if (window == sdlWindow) {
            switch (evt->key.keysym.sym) {
            case SDLK_RIGHT:
                ctx->RightPressed = true;
                break;
            case SDLK_LEFT:
                ctx->LeftPressed = true;
                break;
            case SDLK_UP:
                ctx->UpPressed = true;
                break;
            case SDLK_DOWN:
                ctx->DownPressed = true;
                break;
            case SDLK_a:
                ctx->APressed = true;
                break;
            case SDLK_s:
                ctx->BPressed = true;
                break;
            case SDLK_d:
                ctx->SelectPressed = true;
                break;
            case SDLK_f:
                ctx->StartPressed = true;
                break;
            }
        }
    }
}

void Render(gbx_t * ctx)
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTexture, NULL, (void **)&pixels, &pitch);
    memcpy(pixels, ctx->Backbuffer, sizeof(ctx->Backbuffer));
    SDL_UnlockTexture(sdlTexture);

    int width, height;
    SDL_GetWindowSize(sdlWindow, &width, &height);
    
    SDL_Rect src = { .x = 0, .y = 0, .w = GBX_LCD_WIDTH, .h = GBX_LCD_HEIGHT };
    SDL_Rect dst = { .x = 0, .y = 0, .w = GBX_LCD_WIDTH, .h = GBX_LCD_HEIGHT };

    float scale = 1.0;
    if (width > height) {
        scale = height / (double)GBX_LCD_HEIGHT;
        dst.x = (width - (scale * GBX_LCD_WIDTH)) / 2;
    }
    else {
        scale = width / (double)GBX_LCD_WIDTH;
        dst.y = (height - (scale * GBX_LCD_HEIGHT)) / 2;
    }

    dst.w = GBX_LCD_WIDTH * scale;
    dst.h = GBX_LCD_HEIGHT * scale;

    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, &src, &dst);
    SDL_RenderPresent(sdlRenderer);
}
