#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include <SDL.h>

#define UI_CHAR_WIDTH  (8)
#define UI_CHAR_HEIGHT (8)

#define UI_LINE_HEIGHT (12)

#define UI_MARGIN (8)

#define UI_PANEL_PADDING  (16)
#define UI_BUTTON_PADDING (4)

#define SET_COLOR_BACKGROUND() \
    do { \
        SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0x33, 0x33, 0x33, 0xFF); \
    } while (0)

#define SET_COLOR_DEFAULT() \
    do { \
        SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0xEE, 0xEE, 0xEE, 0xFF); \
    } while (0)

#define SET_COLOR_INACTIVE() \
    do { \
        SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0xAA, 0xAA, 0xAA, 0xFF); \
    } while (0)

#define SET_COLOR_BORDER() \
    do { \
        SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0x00, 0x00, 0x00, 0xFF); \
    } while (0)

enum {
    UI_DIR_NONE,
    UI_DIR_UP,
    UI_DIR_DOWN,
    UI_DIR_LEFT,
    UI_DIR_RIGHT,
};

void UIUpdate();

void UISetDirection(int dir);

void UISetCursor(int x, int y);
void UIGetCursor(int * x, int * y);
void UIMoveCursor(int dx, int dy);

void UINewline();

void UIPrint(const char * format, ...);

void UIPanel(int w, int h);

#define UIPrintln(format, ...)           \
    do {                                    \
        UIPrint(format, ##__VA_ARGS__);  \
        UINewline();                     \
    } while (0)

bool UIButton(const char * text);
bool UICheckbox(const char * text, bool * active);
bool UIRadio(const char * text, int index, int * currentIndex);
bool UITab(const char * text, int index, int * currentIndex);

#endif // UI_H