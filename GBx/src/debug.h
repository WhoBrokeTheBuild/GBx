#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

#include <SDL.h>

extern bool DebugEnabled;

void DebugPromptInit();
void DebugPromptTerm();

void DebugPrompt();

void DebugWindowInit();
void DebugWindowTerm();

void ToggleDebugWindow();

void DebugWindowHandleEvent(SDL_Event * evt);
void DebugWindowRender();

#endif // DEBUG_H