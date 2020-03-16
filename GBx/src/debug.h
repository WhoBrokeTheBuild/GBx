#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

#include <SDL.h>

extern bool DebugEnabled;

void DebugPromptInit();
void DebugPromptTerm();

void DebugPrompt();

typedef struct
{
    int x;
    int y;
    const char * text;
    void (*click)();

} button;

typedef struct 
{
    button button;
    void (*render)();

} tab;

#define DEBUG_WINDOW_WIDTH  (600)
#define DEBUG_WINDOW_HEIGHT (600)

#define DEBUG_WINDOW_PADDING (8)

#define DEBUG_WINDOW_CONTENT_X \
    (8 + DEBUG_WINDOW_PADDING)

#define DEBUG_WINDOW_CONTENT_Y \
    (32 + DEBUG_WINDOW_PADDING)

#define DEBUG_WINDOW_CONTENT_WIDTH \
    (DEBUG_WINDOW_WIDTH - 16 - (DEBUG_WINDOW_PADDING * 2))

#define DEBUG_WINDOW_CONTENT_HEIGHT \
    (DEBUG_WINDOW_HEIGHT - 40 - (DEBUG_WINDOW_PADDING * 2))

#define DEBUG_LINE_HEIGHT (12)

#define COLOR_DEFAULT  (0xDD)
#define COLOR_INACTIVE (0xAA)
#define COLOR_BORDER   (0x00)

void DebugWindowInit();
void DebugWindowTerm();

void ToggleDebugWindow();

SDL_Rect GetStringBounds(int x, int y, const char * string);
SDL_Rect GetButtonBounds(const button * b);

void RenderDebugString(int x, int y, const char * string);
void RenderDebugButton(const button * b, SDL_Point * mouse);
void RenderDebugTab(const tab * t, SDL_Point * mouse, bool active);

void DebugWindowHandleEvent(SDL_Event * evt);
void DebugWindowRender();

#endif // DEBUG_H