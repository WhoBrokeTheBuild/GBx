#include "debug.h"
#include "breakpoint.h"
#include "util.h"
#include "ui.h"

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

#include <GBx/bootstrap.h>
#include <GBx/lcd.h>
#include <GBx/log.h>
#include <GBx/memory.h>
#include <GBx/types.h>

#ifdef HAVE_READLINE
    #include <readline/readline.h>
    #include <readline/history.h>

    #define HISTORY_FILENAME ".gbx_history"
#endif

bool DebugEnabled = false;

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
        read_history(HISTORY_FILENAME);
    #endif
}

void DebugPromptTerm()
{   
    #ifdef HAVE_READLINE
        write_history(HISTORY_FILENAME);
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

#include "win/status.h"
#include "win/tileData.h"
#include "win/tileMap.h"
// #include "win/spriteData.h"
// #include "win/audio.h"

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
};

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

    UIInit();

    InitTileDataTab();
    InitTileMapTab();

    DebugWindowRender();
}

void DebugWindowTerm()
{
    debugWindowShown = false;

    UITerm();

    TermTileMapTab();
    TermTileDataTab();

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

bool dbgAutoRefresh = true;
int dbgTabIndex = 0;

void DebugWindowRender()
{
    if (dbgAutoRefresh) {
        DebugWindowRefresh();
    }

    UIUpdate();

    SET_COLOR_BACKGROUND();

    SDL_RenderClear(sdlDebugRenderer);

    UISetCursor(DEBUG_WINDOW_WIDTH - UI_MARGIN, UI_MARGIN);
    UISetDirection(UI_DIR_LEFT);

    UICheckbox("AUTO", &dbgAutoRefresh);

    if (UIButton("REFRESH")) {
        DebugWindowRefresh();
    }

    UISetCursor(UI_MARGIN, UI_MARGIN);
    UISetDirection(UI_DIR_RIGHT);

    UITab("STATUS",    DBG_TAB_STATUS,    &dbgTabIndex);
    UITab("TILE DATA", DBG_TAB_TILE_DATA, &dbgTabIndex);
    UITab("TILE MAP",  DBG_TAB_TILE_MAP,  &dbgTabIndex);
    UITab("SPRITE",    DBG_TAB_SPRITE,    &dbgTabIndex);
    UITab("AUDIO",     DBG_TAB_AUDIO,     &dbgTabIndex);

    UISetCursor(DEBUG_CONTENT_X, DEBUG_CONTENT_Y);
    UIPanel(DEBUG_CONTENT_WIDTH, DEBUG_CONTENT_HEIGHT);

    switch (dbgTabIndex) {
    case DBG_TAB_STATUS:
        StatusTabRender();
        break;
    case DBG_TAB_TILE_DATA:
        TileDataTabRender();
        break;
    case DBG_TAB_TILE_MAP:
        TileMapTabRender();
        break;
    default:
        break;
    }

    SDL_RenderPresent(sdlDebugRenderer);
}

void DebugWindowRefresh()
{
    switch (dbgTabIndex) {
    case DBG_TAB_TILE_DATA:
        TileDataTabRefresh();
        break;
    case DBG_TAB_TILE_MAP:
        TileMapTabRefresh();
        break;
    default:
        break;
    }
}