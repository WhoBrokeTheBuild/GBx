#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

#include <SDL.h>

extern bool DebugEnabled;

void DebugPromptInit();
void DebugPromptTerm();

void DebugPrompt();

#define DEBUG_WINDOW_WIDTH  (1086)
#define DEBUG_WINDOW_HEIGHT (900)

void DebugWindowInit();
void DebugWindowTerm();

SDL_Renderer * GetDebugWindowRenderer();

void ToggleDebugWindow();

void DebugWindowHandleEvent(SDL_Event * evt);

void DebugWindowRender();
void DebugWindowRefresh();

#endif // DEBUG_H