#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

#include <SDL.h>
#include <GBx/GBx.h>

extern bool DebugEnabled;

void DebugPromptInit(gbx_t * ctx);
void DebugPromptTerm(gbx_t * ctx);

void DebugPrompt(gbx_t * ctx);

#define DEBUG_WINDOW_WIDTH  (1086)
#define DEBUG_WINDOW_HEIGHT (908)

void DebugWindowInit(gbx_t * ctx);
void DebugWindowTerm(gbx_t * ctx);

SDL_Renderer * GetDebugWindowRenderer();

void ToggleDebugWindow();

void DebugWindowHandleEvent(gbx_t * ctx, SDL_Event * evt);

void DebugWindowRender(gbx_t * ctx);
void DebugWindowRefresh(gbx_t * ctx);

#endif // DEBUG_H