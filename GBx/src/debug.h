#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

#include <SDL.h>

extern bool DebugEnabled;

void DebugPromptInit();
void DebugPromptTerm();

void DebugPrompt();

#define DEBUG_WINDOW_WIDTH  (768)
#define DEBUG_WINDOW_HEIGHT (768)

#define DEBUG_CONTENT_X (8)
#define DEBUG_CONTENT_Y (32)

#define DEBUG_CONTENT_WIDTH \
    (DEBUG_WINDOW_WIDTH - 16)

#define DEBUG_CONTENT_HEIGHT \
    (DEBUG_WINDOW_HEIGHT - 40)

void DebugWindowInit();
void DebugWindowTerm();

SDL_Renderer * GetDebugWindowRenderer();

void ToggleDebugWindow();

void DebugWindowHandleEvent(SDL_Event * evt);

void DebugWindowRender();
void DebugWindowRefresh();

#endif // DEBUG_H