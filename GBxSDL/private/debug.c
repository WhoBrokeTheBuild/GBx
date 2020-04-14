#include "debug.h"
#include "breakpoint.h"

#include "cmd/break.h"
#include "cmd/delete.h"
#include "cmd/disassemble.h"
#include "cmd/help.h"
#include "cmd/info.h"
#include "cmd/next.h"
#include "cmd/read.h"
#include "cmd/write.h"

#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <GBx/GBx.h>

#define DUI_IMPLEMENTATION
#include <DUI/DUI.h>

#ifdef HAVE_READLINE
    #include <readline/readline.h>
    #include <readline/history.h>

    #define HISTORY_FILENAME ".gbx_history"
#endif

bool DebugEnabled = false;

void handleSignal(int sig)
{
    printf("Caught signal %d\n", sig);

    // TODO
}

void DebugPromptInit(gbx_t * ctx)
{
    signal(SIGINT, handleSignal);
    signal(SIGSEGV, handleSignal);

    #ifdef HAVE_READLINE
        read_history(HISTORY_FILENAME);
    #endif
}

void DebugPromptTerm(gbx_t * ctx)
{   
    #ifdef HAVE_READLINE
        write_history(HISTORY_FILENAME);
    #endif
    
    signal(SIGINT, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
}

void DebugPrompt(gbx_t * ctx) 
{
#ifdef HAVE_READLINE

    signal(SIGINT, SIG_DFL);

    int oldVerboseLevel = ctx->VerboseLevel;
    ctx->VerboseLevel = 4;
    ctx->CPU->VerboseLevel = 4;

    SM83_PrintRegisters(ctx->CPU);

    char prompt[2048];
    snprintf(prompt, sizeof(prompt), "[%04X]> ", ctx->CPU->PC);

    uint16_t addr;
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
            SM83_Step(ctx->CPU);
        }
        else if (strncmp(input, "help", length) == 0) {
            cmdHelp(ctx, args);
        }
        else if (strncmp(input, "info", length) == 0) {
            cmdInfo(ctx, args);
        }
        else if (strncmp(input, "break", length) == 0) {
            cmdBreak(ctx, args);
        }
        else if (strncmp(input, "delete", length) == 0) {
            cmdDelete(ctx, args);
        }
        else if (strncmp(input, "continue", length) == 0) {
            ctx->VerboseLevel = oldVerboseLevel;
            ctx->CPU->VerboseLevel = oldVerboseLevel;
            break;
        }
        else if (strncmp(input, "next", length) == 0) {
            cmdNext(ctx, args);
        }
        else if (strncmp(input, "read", length) == 0) {
            cmdRead(ctx, args);
        }
        else if (strncmp(input, "write", length) == 0) {
            cmdWrite(ctx, args);
        }
        else if (strncmp(input, "disassemble", length) == 0) {
            cmdDisassemble(ctx, args);
        }
        else if (strncmp(input, "quit", length) == 0
            || strncmp(input, "exit", length) == 0) {
            // TODO: Exit gracefully
            exit(0);
            break;
        }
        else if (strncmp(input, "reset", length) == 0) {
            GBx_Reset(ctx);
        }

        free(input);
        input = NULL;

        SM83_PrintRegisters(ctx->CPU);
        snprintf(prompt, sizeof(prompt), "[%04X]> ", ctx->CPU->PC);
    }

    free(input);

    signal(SIGINT, handleSignal);
    
#endif
}

#include "win/status.h"
#include "win/tileData.h"
#include "win/tileMap.h"
// #include "win/spriteData.h"
// #include "win/audio.h"
#include "win/memory.h"

bool debugWindowShown = false;
bool debugAutoRefresh = true;

SDL_Window   * sdlDebugWindow   = NULL;
SDL_Renderer * sdlDebugRenderer = NULL;

enum {
    DBG_TAB_STATUS,
    DBG_TAB_TILE_DATA,
    DBG_TAB_TILE_MAP,
    DBG_TAB_SPRITE,
    DBG_TAB_AUDIO,
    DBG_TAB_MEMORY,
};

void DebugWindowInit(gbx_t * ctx)
{
    sdlDebugWindow = SDL_CreateWindow("GBx - Debug",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT,
        SDL_WINDOW_HIDDEN);
    
    if (!sdlDebugWindow) {
        fprintf(stderr, "Failed to create SDL2 Window, %s\n", SDL_GetError());
    }
    
    sdlDebugRenderer = SDL_CreateRenderer(sdlDebugWindow, 0,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    if (!sdlDebugRenderer) {
        fprintf(stderr, "Failed to create SDL2 Renderer, %s\n", SDL_GetError());
    }

    DUI_Init(sdlDebugWindow);
    DUI_Style * style = DUI_GetStyle();
    // style->CharWidth = 16;
    // style->CharHeight = 16;

    SM83_EnableInstructionLogging(ctx->CPU);
    SM83_EnableStackLogging(ctx->CPU);

    GBx_InitMemoryTracker(ctx);
    
    InitTileDataTab(ctx);
    InitTileMapTab(ctx);
    InitMemoryTab(ctx);

    DebugWindowRender(ctx);
}

void DebugWindowTerm(gbx_t * ctx)
{
    debugWindowShown = false;

    GBx_TermMemoryTracker(ctx);

    DUI_Term();

    TermMemoryTab(ctx);
    TermTileMapTab(ctx);
    TermTileDataTab(ctx);

    SDL_DestroyRenderer(sdlDebugRenderer);
    SDL_DestroyWindow(sdlDebugWindow);
}

SDL_Renderer * GetDebugWindowRenderer()
{
    return sdlDebugRenderer;
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

void DebugWindowHandleEvent(gbx_t * ctx, SDL_Event * evt)
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

bool dbgAutoRefresh = true;
int dbgTabIndex = 0;

void DebugWindowRender(gbx_t * ctx)
{
    DUI_Update();
    DUI_Style * style = DUI_GetStyle();

    if (dbgAutoRefresh) {
        DebugWindowRefresh(ctx);
    }

    SDL_SetRenderDrawColor(sdlDebugRenderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(sdlDebugRenderer);

    DUI_CheckboxAt(DEBUG_WINDOW_WIDTH - 160, 8, "Auto", &dbgAutoRefresh);

    if (DUI_ButtonAt(DEBUG_WINDOW_WIDTH - 86, 8, "Refresh")) {
        DebugWindowRefresh(ctx);
    }

    DUI_MoveCursor(8, 8);

    DUI_BeginTabBar();

    DUI_Tab("Status", DBG_TAB_STATUS, &dbgTabIndex);
    DUI_Tab("Tile Data", DBG_TAB_TILE_DATA, &dbgTabIndex);
    DUI_Tab("Tile Map", DBG_TAB_TILE_MAP, &dbgTabIndex);
    DUI_Tab("Sprite", DBG_TAB_SPRITE, &dbgTabIndex);
    DUI_Tab("Audio", DBG_TAB_AUDIO, &dbgTabIndex);
    DUI_Tab("Memory", DBG_TAB_MEMORY, &dbgTabIndex);
    
    DUI_MoveCursor(8, 48);
    DUI_PanelStart(NULL, 
        DEBUG_WINDOW_WIDTH - 16 - (style->PanelPadding * 2),
        DEBUG_WINDOW_HEIGHT - 56 - (style->PanelPadding * 2), true);

    switch (dbgTabIndex) {
    case DBG_TAB_STATUS:
        StatusTabRender(ctx);
        break;
    case DBG_TAB_TILE_DATA:
        TileDataTabRender(ctx);
        break;
    case DBG_TAB_TILE_MAP:
        TileMapTabRender(ctx);
        break;
    case DBG_TAB_SPRITE:
        break;
    case DBG_TAB_AUDIO:
        break;
    case DBG_TAB_MEMORY:
        MemoryTabRender(ctx);
        break;
    default:
        break;
    }

    DUI_PanelEnd();

    DUI_Render();
    SDL_RenderPresent(sdlDebugRenderer);
}

void DebugWindowRefresh(gbx_t * ctx)
{
    switch (dbgTabIndex) {
    case DBG_TAB_TILE_DATA:
        TileDataTabRefresh(ctx);
        break;
    case DBG_TAB_TILE_MAP:
        TileMapTabRefresh(ctx);
        break;
    case DBG_TAB_MEMORY:
        MemoryTabRefresh(ctx);
        break;
    default:
        break;
    }
}