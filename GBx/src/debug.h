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

#define DEBUG_CHAR_WIDTH  (8)
#define DEBUG_CHAR_HEIGHT (8)

#define DEBUG_LINE_HEIGHT (12)

#define DEBUG_MARGIN (8)

#define DEBUG_PANEL_PADDING  (16)
#define DEBUG_BUTTON_PADDING (4)

#define DEBUG_CONTENT_X (8)
#define DEBUG_CONTENT_Y (32)

#define DEBUG_CONTENT_WIDTH \
    (DEBUG_WINDOW_WIDTH - 16)

#define DEBUG_CONTENT_HEIGHT \
    (DEBUG_WINDOW_HEIGHT - 40)

enum {
    DEBUG_DIR_NONE,
    DEBUG_DIR_UP,
    DEBUG_DIR_DOWN,
    DEBUG_DIR_LEFT,
    DEBUG_DIR_RIGHT,
};

void DebugWindowInit();
void DebugWindowTerm();

SDL_Renderer * GetDebugWindowRenderer();

void ToggleDebugWindow();

void DebugWindowHandleEvent(SDL_Event * evt);

void DebugWindowUpdate();
void DebugWindowRender();
void DebugWindowRefresh();

void DebugSetDirection(int dir);

void DebugSetCursor(int x, int y);
void DebugGetCursor(int * x, int * y);
void DebugMoveCursor(int dx, int dy);

void DebugNewline();

void DebugPrint(const char * format, ...);

void DebugPanel(int w, int h);

#define DebugPrintln(format, ...)           \
    do {                                    \
        DebugPrint(format, ##__VA_ARGS__);  \
        DebugNewline();                     \
    } while (0)

bool DebugButton(const char * text);
bool DebugCheckbox(const char * text, bool * active);
bool DebugRadio(const char * text, int index, int * currentIndex);
bool DebugTab(const char * text, int index, int * currentIndex);

#endif // DEBUG_H