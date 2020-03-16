#include "debug.h"

#include "breakpoint.h"

#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>

#include <GBx/apu.h>
#include <GBx/bootstrap.h>
#include <GBx/cartridge.h>
#include <GBx/cpu.h>
#include <GBx/instruction.h>
#include <GBx/interrupt.h>
#include <GBx/lcd.h>
#include <GBx/log.h>
#include <GBx/memory.h>
#include <GBx/timer.h>

#include "cmd/break.h"
#include "cmd/delete.h"
#include "cmd/disassemble.h"
#include "cmd/help.h"
#include "cmd/info.h"
#include "cmd/next.h"
#include "cmd/read.h"
#include "cmd/write.h"

#ifdef HAVE_READLINE
    #include <readline/readline.h>
    #include <readline/history.h>

    #define HISTORY_FILE ".gbx_history"
#endif

bool DebugEnabled = false;

bool debugWindowShown = false;

SDL_Window   * sdlDebugWindow   = NULL;
SDL_Renderer * sdlDebugRenderer = NULL;
SDL_Texture  * sdlDebugTexture  = NULL;

#define DEBUG_WINDOW_WIDTH  (1600)
#define DEBUG_WINDOW_HEIGHT (650)

#define DEBUG_BUFFER_WIDTH  (DEBUG_WINDOW_WIDTH / 2)
#define DEBUG_BUFFER_HEIGHT (DEBUG_WINDOW_HEIGHT / 2)

void handleSignal(int sig)
{
    LogInfo("Caught signal %d", sig);

    if (sig == SIGINT) {
        SetBreakpoint("PC", R.PC);
    }
    else {
        DebugPrompt();
        exit(1);
    }
}

void DebugPromptInit()
{
    signal(SIGINT, handleSignal);
    signal(SIGSEGV, handleSignal);

    #ifdef HAVE_READLINE
        read_history(HISTORY_FILE);
    #endif
}

void DebugPromptTerm()
{   
    #ifdef HAVE_READLINE
        write_history(HISTORY_FILE);
    #endif
    
    signal(SIGINT, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
}

void DebugPrompt() 
{
#ifdef HAVE_READLINE

    signal(SIGINT, SIG_DFL);

    int oldVerboseLevel = VerboseLevel;
    VerboseLevel = 4;

    PrintR();

    char prompt[2048];
    snprintf(prompt, sizeof(prompt), "[%04X]> ", R.PC);

    word addr;
    char * input = NULL;
    while ((input = readline(prompt)) != NULL) {
        size_t length = 0;
        char * sep = strchr(input, ' ');
        char * args = NULL;
        if (sep) {
            length = sep - input;
            args = input + length + 1;
        }
        else {
            length = strlen(input);
        }

        if (length > 0) {
            add_history(input);
        }

        if (length == 0) {
            NextInstruction();
        }
        else if (strncmp(input, "help", length) == 0) {
            cmdHelp(args);
        }
        else if (strncmp(input, "info", length) == 0) {
            cmdInfo(args);
        }
        else if (strncmp(input, "break", length) == 0) {
            cmdBreak(args);
        }
        else if (strncmp(input, "delete", length) == 0) {
            cmdDelete(args);
        }
        else if (strncmp(input, "continue", length) == 0) {
            VerboseLevel = oldVerboseLevel;
            break;
        }
        else if (strncmp(input, "next", length) == 0) {
            cmdNext(args);
        }
        else if (strncmp(input, "read", length) == 0) {
            cmdRead(args);
        }
        else if (strncmp(input, "write", length) == 0) {
            cmdWrite(args);
        }
        else if (strncmp(input, "disassemble", length) == 0) {
            cmdDisassemble(args);
        }
        else if (strncmp(input, "quit", length) == 0
            || strncmp(input, "exit", length) == 0) {
            // TODO: Exit gracefully
            exit(0);
            break;
        }
        else if (strncmp(input, "reset", length) == 0) {
            Reset();
        }

        free(input);
        input = NULL;

        PrintR();
        snprintf(prompt, sizeof(prompt), "[%04X]> ", R.PC);
    }

    free(input);

    signal(SIGINT, handleSignal);
    
#endif
}

void DebugWindowInit()
{
    sdlDebugWindow = SDL_CreateWindow("GBx - Debug",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT,
        SDL_WINDOW_HIDDEN);
    
    if (!sdlDebugWindow) {
        LogFatal("Failed to create SDL2 Window, %s", SDL_GetError());
    }
    
    sdlDebugRenderer = SDL_CreateRenderer(sdlDebugWindow, 0,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!sdlDebugRenderer) {
        LogFatal("Failed to create SDL2 Renderer, %s", SDL_GetError());
    }
    
    sdlDebugTexture = SDL_CreateTexture(sdlDebugRenderer,
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        DEBUG_BUFFER_WIDTH, DEBUG_BUFFER_HEIGHT);

    SDL_SetRenderDrawColor(sdlDebugRenderer, 0, 0, 0, 255);

    DebugWindowRender();
}

void DebugWindowTerm()
{
    debugWindowShown = false;
    SDL_DestroyTexture(sdlDebugTexture);
    SDL_DestroyRenderer(sdlDebugRenderer);
    SDL_DestroyWindow(sdlDebugWindow);
}

void ToggleDebugWindow()
{
    debugWindowShown = !debugWindowShown;
    if (debugWindowShown) {
        SDL_ShowWindow(sdlDebugWindow);
    }
    else {
        SDL_HideWindow(sdlDebugWindow);
    }
}

void DebugWindowHandleEvent(SDL_Event * evt)
{
    if (evt->type == SDL_WINDOWEVENT) {
        SDL_Window * window = SDL_GetWindowFromID(evt->window.windowID);
        if (window == sdlDebugWindow) {
            switch (evt->window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                SDL_HideWindow(sdlDebugWindow);
                break;
            }
        }
    }
    else if (evt->type == SDL_KEYUP) {
        SDL_Window * window = SDL_GetWindowFromID(evt->key.windowID);
        if (window == sdlDebugWindow) {
            if (evt->key.keysym.sym == SDLK_1) {
                SDL_HideWindow(sdlDebugWindow);
            }
        }
    }
}

void DebugWindowRender()
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlDebugTexture, NULL, (void **)&pixels, &pitch);

    memset(pixels, 0x20, 
        DEBUG_BUFFER_WIDTH * DEBUG_BUFFER_HEIGHT * LCD_BUFFER_COMPONENTS);

    {
        const int TILE_DRAW_PER_ROW = 16;
        const int TILE_DRAW_X = 8;
        const int TILE_DRAW_Y = 8;
        const int TILE_COUNT = 256;

        for (int i = 0; i < TILE_COUNT; ++i) {
            int x = TILE_DRAW_X + ((i % TILE_DRAW_PER_ROW) * (TILE_WIDTH + 1));
            int y = TILE_DRAW_Y + ((i / TILE_DRAW_PER_ROW) * (TILE_HEIGHT + 1));
            for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {
                word dataOffset = TILE_DATA_ADDR[1] + (i * TILE_DATA_SIZE) + (tileRow * 2);

                byte data1 = ReadByte(dataOffset);
                byte data2 = ReadByte(dataOffset + 1);

                for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                    byte color = GetColor(&BGP, tileCol, data1, data2);

                    uint off = ((y + tileRow) * pitch) 
                        + ((x + tileCol) * LCD_BUFFER_COMPONENTS);

                    pixels[off + 0] = color;
                    pixels[off + 1] = color;
                    pixels[off + 2] = color;
                }
            }
        }
    }

    {
        const int TILE_DRAW_PER_ROW = 16;
        const int TILE_DRAW_X = 9;
        const int TILE_DRAW_Y = 161;
        const int TILE_COUNT = 256;

        for (int i = 0; i < TILE_COUNT; ++i) {
            int x = TILE_DRAW_X + ((i % TILE_DRAW_PER_ROW) * (TILE_WIDTH + 1));
            int y = TILE_DRAW_Y + ((i / TILE_DRAW_PER_ROW) * (TILE_HEIGHT + 1));
            for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {
                word dataOffset = TILE_DATA_ADDR[0] + ((i - 128) * TILE_DATA_SIZE) + (tileRow * 2);

                byte data1 = ReadByte(dataOffset);
                byte data2 = ReadByte(dataOffset + 1);

                for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                    byte color = GetColor(&BGP, tileCol, data1, data2);

                    uint off = ((y + tileRow) * pitch) 
                        + ((x + tileCol) * LCD_BUFFER_COMPONENTS);

                    pixels[off + 0] = color;
                    pixels[off + 1] = color;
                    pixels[off + 2] = color;
                }
            }
        }
    }

    {
        const int TILE_DRAW_ROWS = 32;
        const int TILE_DRAW_COLS = 32;
        const int TILE_DRAW_X = 161;
        const int TILE_DRAW_Y = 9;

        for (int row = 0; row < TILE_DRAW_ROWS; ++row) {
            int y = TILE_DRAW_Y + (row * (TILE_HEIGHT + 1));
            for (int col = 0; col < TILE_DRAW_COLS; ++col) {
                int x = TILE_DRAW_X + (col * (TILE_WIDTH + 1));

                int tileIndex = ReadByte(TILE_MAP_ADDR[0] 
                    + (row * TILES_PER_ROW) + col);

                if (LCDC.TileDataSelect == 0) {
                    tileIndex = (sbyte)tileIndex; // Convert to [-128, 127]
                }

                for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {
                    word dataOffset = TILE_DATA_ADDR[LCDC.TileDataSelect] 
                        + (tileIndex * TILE_DATA_SIZE) + (tileRow * 2);

                    byte data1 = ReadByte(dataOffset);
                    byte data2 = ReadByte(dataOffset + 1);

                    for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                        byte color = GetColor(&BGP, tileCol, data1, data2);

                        uint off = ((y + tileRow) * pitch) 
                            + ((x + tileCol) * LCD_BUFFER_COMPONENTS);

                        pixels[off + 0] = color;
                        pixels[off + 1] = color;
                        pixels[off + 2] = color;
                    }
                }
            }
        }
    }

    {
        const int TILE_DRAW_ROWS = 32;
        const int TILE_DRAW_COLS = 32;
        const int TILE_DRAW_X = 457;
        const int TILE_DRAW_Y = 9;

        for (int row = 0; row < TILE_DRAW_ROWS; ++row) {
            int y = TILE_DRAW_Y + (row * (TILE_HEIGHT + 1));
            for (int col = 0; col < TILE_DRAW_COLS; ++col) {
                int x = TILE_DRAW_X + (col * (TILE_WIDTH + 1));

                int tileIndex = ReadByte(TILE_MAP_ADDR[1] 
                    + (row * TILES_PER_ROW) + col);

                if (LCDC.TileDataSelect == 0) {
                    tileIndex = (sbyte)tileIndex; // Convert to [-128, 127]
                }

                for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {
                    word dataOffset = TILE_DATA_ADDR[LCDC.TileDataSelect] 
                        + (tileIndex * TILE_DATA_SIZE) + (tileRow * 2);

                    byte data1 = ReadByte(dataOffset);
                    byte data2 = ReadByte(dataOffset + 1);

                    for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                        byte color = GetColor(&BGP, tileCol, data1, data2);

                        uint off = ((y + tileRow) * pitch) 
                            + ((x + tileCol) * LCD_BUFFER_COMPONENTS);

                        pixels[off + 0] = color;
                        pixels[off + 1] = color;
                        pixels[off + 2] = color;
                    }
                }
            }
        }
    }

    {
        const int SCROLL_DRAW_X = 160;
        const int SCROLL_DRAW_Y = 8;
        const int WIN_DRAW_X = 456;
        const int WIN_DRAW_Y = 8;

        const int DRAW_WIDTH  = LCD_WIDTH + (LCD_WIDTH / 8);
        const int DRAW_HEIGHT = LCD_HEIGHT + (LCD_HEIGHT / 8);

        int off;
        int scx = SCX + (SCX / 8);
        int scy = SCY + (SCY / 8);
        int wxm7 = WX - 7;
        int wx = wxm7 + (wxm7 / 8);
        int wy = WY + (WY / 8);

        for (int i = 0; i < DRAW_WIDTH + 1; ++i) {

            off = ((SCROLL_DRAW_Y + scy) * pitch) 
                + ((SCROLL_DRAW_X + scx + i) * LCD_BUFFER_COMPONENTS);

            pixels[off + 0] = 0xFF;
            pixels[off + 2] = 0x00;
            pixels[off + 1] = 0x00;

            off = ((SCROLL_DRAW_Y + scy + DRAW_HEIGHT) * pitch) 
                + ((SCROLL_DRAW_X + scx + i) * LCD_BUFFER_COMPONENTS);

            pixels[off + 0] = 0xFF;
            pixels[off + 2] = 0x00;
            pixels[off + 1] = 0x00;

            off = ((WIN_DRAW_Y + wy) * pitch) 
                + ((WIN_DRAW_X + wx + i) * LCD_BUFFER_COMPONENTS);

            pixels[off + 0] = 0x00;
            pixels[off + 2] = 0xFF;
            pixels[off + 1] = 0x00;

            off = ((WIN_DRAW_Y + wy + DRAW_HEIGHT) * pitch) 
                + ((WIN_DRAW_X + wx + i) * LCD_BUFFER_COMPONENTS);

            pixels[off + 0] = 0x00;
            pixels[off + 2] = 0xFF;
            pixels[off + 1] = 0x00;
        }

        for (int i = 0; i < DRAW_HEIGHT + 1; ++i) {

            off = ((SCROLL_DRAW_Y + scy + i) * pitch) 
                + ((SCROLL_DRAW_X + scx) * LCD_BUFFER_COMPONENTS);

            pixels[off + 0] = 0xFF;
            pixels[off + 2] = 0x00;
            pixels[off + 1] = 0x00;

            off = ((SCROLL_DRAW_Y + scy + i) * pitch) 
                + ((SCROLL_DRAW_X + scx + DRAW_WIDTH) * LCD_BUFFER_COMPONENTS);

            pixels[off + 0] = 0xFF;
            pixels[off + 2] = 0x00;
            pixels[off + 1] = 0x00;

            off = ((WIN_DRAW_Y + wy + i) * pitch) 
                + ((WIN_DRAW_X + wx) * LCD_BUFFER_COMPONENTS);

            pixels[off + 0] = 0x00;
            pixels[off + 2] = 0xFF;
            pixels[off + 1] = 0x00;

            off = ((WIN_DRAW_Y + wy + i) * pitch) 
                + ((WIN_DRAW_X + wx + DRAW_WIDTH) * LCD_BUFFER_COMPONENTS);

            pixels[off + 0] = 0x00;
            pixels[off + 2] = 0xFF;
            pixels[off + 1] = 0x00;
        }
    }

    SDL_UnlockTexture(sdlDebugTexture);

    SDL_Rect src = { .x = 0, .y = 0, .w = DEBUG_BUFFER_WIDTH, .h = DEBUG_BUFFER_HEIGHT };
    SDL_Rect dst = { .x = 0, .y = 0, .w = DEBUG_WINDOW_WIDTH, .h = DEBUG_WINDOW_HEIGHT };

    SDL_RenderClear(sdlDebugRenderer);
    SDL_RenderCopy(sdlDebugRenderer, sdlDebugTexture, &src, &dst);
    SDL_RenderPresent(sdlDebugRenderer);
}
