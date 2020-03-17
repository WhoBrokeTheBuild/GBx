#include "debug.h"
#include "breakpoint.h"
#include "util.h"

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

#define FONT_FILENAME "font.bmp"

const char * FONT_CHARACTER_MAP = 
"ABCDEFGH"
"IJKLMNOP"
"QRSTUVWX"
"YZ+-/%*="
"01234567"
"89:;!?\"'"
"(){}[]<>"
"#$^~_&,.";

bool debugWindowShown = false;

bool debugAutoRefresh = true;

SDL_Window   * sdlDebugWindow   = NULL;
SDL_Renderer * sdlDebugRenderer = NULL;
SDL_Texture  * sdlFontTexture   = NULL;

#define BUTTON_PADDING (4)

int debugTabIndex = 0;

tab debugTabs[] = {
    { 16,  16, "STATUS",    NULL,               StatusTabRender,   NULL },
    { 80,  16, "TILE DATA", TileDataTabRefresh, TileDataTabRender, TileDataTabClick },
    { 168, 16, "TILE MAP",  TileMapTabRefresh,  TileMapTabRender,  TileMapTabClick },
    { 248, 16, "SPRITE",    NULL,               NULL,              NULL },
    { 312, 16, "AUDIO",     NULL,               NULL,              NULL },
};

void onRefreshClick()
{
    if (debugTabs[debugTabIndex].refresh) {
        debugTabs[debugTabIndex].refresh();
    }
}

button btnRefresh = { 
    .x = DEBUG_WINDOW_WIDTH - 200,
    .y = 8,
    .text = "REFRESH",
    .click = onRefreshClick,
};

checkbox chkAutoRefresh = {
    .x = DEBUG_WINDOW_WIDTH - 128,
    .y = 8,
    .text = "AUTO REFRESH",
    .checked = true,
    .changed = NULL,
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

    char fontPath[4096];
    GetExecutablePath(fontPath, sizeof(fontPath) - sizeof(FONT_FILENAME));
    strcat(fontPath + strlen(fontPath), FONT_FILENAME);

    SDL_Surface * fontSurface = SDL_LoadBMP(fontPath);
    if (!fontSurface) {
        LogError("Failed to load '%s'", fontPath);
    }

    sdlFontTexture = SDL_CreateTextureFromSurface(sdlDebugRenderer, fontSurface);
    SDL_FreeSurface(fontSurface);

    InitTileDataTab();
    InitTileMapTab();

    DebugWindowRender();
}

void DebugWindowTerm()
{
    debugWindowShown = false;

    TermTileMapTab();
    TermTileDataTab();

    SDL_DestroyTexture(sdlFontTexture);
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

SDL_Rect GetStringBounds(int x, int y, const char * string)
{
    return (SDL_Rect){
        .x = x,
        .y = y,
        .w = strlen(string) * DEBUG_CHARACTER_WIDTH,
        .h = DEBUG_CHARACTER_HEIGHT
    };
}

SDL_Rect GetButtonBounds(button * b)
{
    SDL_Rect rect = GetStringBounds(b->x, b->y, b->text);
    rect.w += (BUTTON_PADDING * 2);
    rect.h += (BUTTON_PADDING * 2);
    return rect;
}

SDL_Rect GetTabBounds(tab * t)
{
    SDL_Rect rect = GetStringBounds(t->x, t->y, t->text);
    rect.w += (BUTTON_PADDING * 2);
    rect.h += (BUTTON_PADDING * 2);
    return rect;
}

SDL_Rect GetCheckboxBounds(checkbox * c)
{
    SDL_Rect rect = GetStringBounds(c->x, c->y, c->text);
    rect.w += (BUTTON_PADDING * 2) + (DEBUG_CHARACTER_WIDTH * 2);
    rect.h += (BUTTON_PADDING * 2);
    return rect;
}

bool CheckButtonClick(button * b, SDL_Point * mouse)
{
    SDL_Rect rect = GetButtonBounds(b);
    return SDL_PointInRect(mouse, &rect);
}

bool CheckTabClick(tab * t, SDL_Point * mouse)
{
    SDL_Rect rect = GetTabBounds(t);
    return SDL_PointInRect(mouse, &rect);
}

bool CheckCheckboxClick(checkbox * c, SDL_Point * mouse)
{
    SDL_Rect rect = GetCheckboxBounds(c);
    return SDL_PointInRect(mouse, &rect);
}

void RenderDebugString(int x, int y, const char * string)
{
    size_t length = strlen(string);

    SDL_Rect src = { .x = 0, .y = 0, .w = DEBUG_CHARACTER_WIDTH, .h = DEBUG_CHARACTER_HEIGHT };
    SDL_Rect dst = { .x = x, .y = y, .w = DEBUG_CHARACTER_WIDTH, .h = DEBUG_CHARACTER_HEIGHT };

    char * questionMark = strchr(FONT_CHARACTER_MAP, '?');

    for (size_t i = 0; i < length; ++i) {
        if (string[i] == ' ') {
            dst.x += DEBUG_CHARACTER_WIDTH;
            continue;
        }

        char * index = strchr(FONT_CHARACTER_MAP, toupper(string[i]));

        if (index == NULL) {
            index = questionMark;
        }

        size_t offset = index - FONT_CHARACTER_MAP;

        src.x = (offset % 8) * DEBUG_CHARACTER_WIDTH;
        src.y = (offset / 8) * DEBUG_CHARACTER_WIDTH;
        SDL_RenderCopy(sdlDebugRenderer, sdlFontTexture, &src, &dst);

        dst.x += DEBUG_CHARACTER_WIDTH;
    }
}

void RenderDebugButton(button * b, SDL_Point * mouse)
{
    SDL_Rect dst = GetButtonBounds(b);

    if (SDL_PointInRect(mouse, &dst)) {
        SDL_SetRenderDrawColor(sdlDebugRenderer, 
            COLOR_DEFAULT, COLOR_DEFAULT, COLOR_DEFAULT, 0xFF);
    }
    else {
        SDL_SetRenderDrawColor(sdlDebugRenderer, 
            COLOR_INACTIVE, COLOR_INACTIVE, COLOR_INACTIVE, 0xFF);
    }

    SDL_RenderFillRect(sdlDebugRenderer, &dst);

    SDL_SetRenderDrawColor(sdlDebugRenderer, 
        COLOR_BORDER, COLOR_BORDER, COLOR_BORDER, 0xFF);

    SDL_RenderDrawRect(sdlDebugRenderer, &dst);

    int textX = dst.x + BUTTON_PADDING;
    int textY = dst.y + BUTTON_PADDING;

    RenderDebugString(textX, textY, b->text);
}

void RenderDebugTab(tab * t, SDL_Point * mouse, bool active)
{
    SDL_Rect dst = GetTabBounds(t);
    dst.h += (BUTTON_PADDING * 2);

    if (active) {
        dst.y -= BUTTON_PADDING;

        SDL_SetRenderDrawColor(sdlDebugRenderer,
            COLOR_DEFAULT, COLOR_DEFAULT, COLOR_DEFAULT, 0xFF);
    }
    else if (SDL_PointInRect(mouse, &dst)) {
        SDL_SetRenderDrawColor(sdlDebugRenderer, 
            COLOR_DEFAULT, COLOR_DEFAULT, COLOR_DEFAULT, 0xFF);
    }
    else {
        SDL_SetRenderDrawColor(sdlDebugRenderer, 
            COLOR_INACTIVE, COLOR_INACTIVE, COLOR_INACTIVE, 0xFF);
    }

    SDL_RenderFillRect(sdlDebugRenderer, &dst);

    SDL_SetRenderDrawColor(sdlDebugRenderer, 
        COLOR_BORDER, COLOR_BORDER, COLOR_BORDER, 0xFF);

    SDL_RenderDrawRect(sdlDebugRenderer, &dst);
    
    int textX = dst.x + BUTTON_PADDING;
    int textY = dst.y + BUTTON_PADDING;

    RenderDebugString(textX, textY, t->text);
}

void RenderDebugCheckbox(checkbox * c, SDL_Point * mouse)
{
    SDL_Rect dst = GetCheckboxBounds(c);

    if (SDL_PointInRect(mouse, &dst)) {
        SDL_SetRenderDrawColor(sdlDebugRenderer, 
            COLOR_DEFAULT, COLOR_DEFAULT, COLOR_DEFAULT, 0xFF);
    }
    else {
        SDL_SetRenderDrawColor(sdlDebugRenderer, 
            COLOR_INACTIVE, COLOR_INACTIVE, COLOR_INACTIVE, 0xFF);
    }

    SDL_RenderFillRect(sdlDebugRenderer, &dst);

    SDL_SetRenderDrawColor(sdlDebugRenderer, 
        COLOR_BORDER, COLOR_BORDER, COLOR_BORDER, 0xFF);

    SDL_RenderDrawRect(sdlDebugRenderer, &dst);
    
    SDL_Rect checkbox = { 
        .x = dst.x + BUTTON_PADDING, 
        .y = dst.y + BUTTON_PADDING,
        .w = DEBUG_CHARACTER_WIDTH, 
        .h = DEBUG_CHARACTER_HEIGHT
    };

    SDL_RenderDrawRect(sdlDebugRenderer, &checkbox);

    if (c->checked) {
        ++checkbox.x;
        ++checkbox.y;
        checkbox.w -= 2;
        checkbox.h -= 2;

        SDL_RenderFillRect(sdlDebugRenderer, &checkbox);
    }
    
    int textX = dst.x + BUTTON_PADDING + (DEBUG_CHARACTER_WIDTH * 2);
    int textY = dst.y + BUTTON_PADDING;

    RenderDebugString(textX, textY, c->text);
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
    else if (evt->type == SDL_MOUSEBUTTONUP) {
        SDL_Window * window = SDL_GetWindowFromID(evt->key.windowID);
        if (window == sdlDebugWindow) {
            if (evt->button.button == SDL_BUTTON_LEFT) {
                SDL_Point mouse = { evt->button.x, evt->button.y };
                DebugWindowHandleClick(&mouse);
            }
        }
    }
}

void DebugWindowHandleClick(SDL_Point * mouse)
{
    if (CheckButtonClick(&btnRefresh, mouse)) {
        if (btnRefresh.click) {
            btnRefresh.click();
        }
    }

    if (CheckCheckboxClick(&chkAutoRefresh, mouse)) {
        chkAutoRefresh.checked ^= true;
        if (chkAutoRefresh.changed) {
            chkAutoRefresh.changed();
        }
    }

    for (int i = 0; i < sizeof(debugTabs) / sizeof(tab); ++i) {
        tab * tab = &debugTabs[i];
        if (CheckTabClick(tab, mouse)) {
            debugTabIndex = i;
            break;
        }
    }

    if (debugTabs[debugTabIndex].windowClick) {
        debugTabs[debugTabIndex].windowClick(mouse);
    }
}

void DebugWindowRender()
{
    if (chkAutoRefresh.checked) {
        if (debugTabs[debugTabIndex].refresh) {
            debugTabs[debugTabIndex].refresh();
        }
    }

    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    SDL_SetRenderDrawColor(sdlDebugRenderer, 0x33, 0x33, 0x33, 0xFF);
    SDL_RenderClear(sdlDebugRenderer);

    RenderDebugButton(&btnRefresh, &mouse);
    RenderDebugCheckbox(&chkAutoRefresh, &mouse);

    for (int i = 0; i < sizeof(debugTabs) / sizeof(tab); ++i) {
        tab * tab = &debugTabs[i];
        RenderDebugTab(tab, &mouse, (debugTabIndex == i));
    }

    SDL_Rect dst = { 
        .x = DEBUG_WINDOW_CONTENT_X - DEBUG_WINDOW_PADDING, 
        .y = DEBUG_WINDOW_CONTENT_Y - DEBUG_WINDOW_PADDING, 
        .w = DEBUG_WINDOW_CONTENT_WIDTH + (DEBUG_WINDOW_PADDING * 2), 
        .h = DEBUG_WINDOW_CONTENT_HEIGHT + (DEBUG_WINDOW_PADDING * 2),
    };

    SDL_SetRenderDrawColor(sdlDebugRenderer, 
        COLOR_DEFAULT, COLOR_DEFAULT, COLOR_DEFAULT, 0xFF);

    SDL_RenderFillRect(sdlDebugRenderer, &dst);

    SDL_SetRenderDrawColor(sdlDebugRenderer, 
        COLOR_BORDER, COLOR_BORDER, COLOR_BORDER, 0xFF);

    SDL_RenderDrawRect(sdlDebugRenderer, &dst);

    if (debugTabs[debugTabIndex].render) {
        debugTabs[debugTabIndex].render(&mouse);
    }

    SDL_RenderPresent(sdlDebugRenderer);
}
