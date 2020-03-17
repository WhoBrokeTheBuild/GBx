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
    int x;
    int y;
    const char * text;
    void (*refresh)();
    void (*render)(SDL_Point *);
    void (*windowClick)(SDL_Point *);

} tab;

typedef struct 
{
    int x;
    int y;
    const char * text;
    bool checked;
    void (*changed)();
    
} checkbox;

#define DEBUG_WINDOW_WIDTH  (768)
#define DEBUG_WINDOW_HEIGHT (768)

#define DEBUG_WINDOW_PADDING (8)

#define DEBUG_WINDOW_CONTENT_X \
    (8 + DEBUG_WINDOW_PADDING)

#define DEBUG_WINDOW_CONTENT_Y \
    (32 + DEBUG_WINDOW_PADDING)

#define DEBUG_WINDOW_CONTENT_WIDTH \
    (DEBUG_WINDOW_WIDTH - 16 - (DEBUG_WINDOW_PADDING * 2))

#define DEBUG_WINDOW_CONTENT_HEIGHT \
    (DEBUG_WINDOW_HEIGHT - 40 - (DEBUG_WINDOW_PADDING * 2))

#define DEBUG_CHARACTER_WIDTH  (8)
#define DEBUG_CHARACTER_HEIGHT (8)

#define DEBUG_LINE_HEIGHT (12)

#define COLOR_DEFAULT  (0xDD)
#define COLOR_INACTIVE (0xAA)
#define COLOR_BORDER   (0x00)

void DebugWindowInit();
void DebugWindowTerm();

SDL_Renderer * GetDebugWindowRenderer();

void ToggleDebugWindow();

SDL_Rect GetStringBounds(int x, int y, const char * string);
SDL_Rect GetButtonBounds(button * b);
SDL_Rect GetTabBounds(tab * t);
SDL_Rect GetCheckboxBounds(checkbox * c);

void ToggleCheckbox(checkbox * c);

bool CheckButtonClick(button * b, SDL_Point * mouse);
bool CheckTabClick(tab * t, SDL_Point * mouse);
bool CheckCheckboxClick(checkbox * c, SDL_Point * mouse);

void RenderDebugString(int x, int y, const char * string);
void RenderDebugButton(button * b, SDL_Point * mouse);
void RenderDebugTab(tab * t, SDL_Point * mouse, bool active);
void RenderDebugCheckbox(checkbox * c, SDL_Point * mouse);

void DebugWindowHandleClick(SDL_Point * mouse);
void DebugWindowHandleEvent(SDL_Event * evt);

void DebugWindowRender();

#endif // DEBUG_H