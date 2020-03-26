#include "ui.h"
#include "debug.h"

#include <GBx/log.h>

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

SDL_Texture * sdlFontTexture = NULL;

SDL_Point uiCursor = { .x = 0, .y = 0 };
SDL_Point uiMouse  = { .x = -1, .y = -1 };

int uiDirection = UI_DIR_RIGHT;
bool uiClick = false;
bool uiMouseDown = false;

void UIInit()
{
    char fontPath[4096];
    GetExecutablePath(fontPath, sizeof(fontPath) - sizeof(FONT_FILENAME));
    strcat(fontPath + strlen(fontPath), FONT_FILENAME);

    SDL_Surface * fontSurface = SDL_LoadBMP(fontPath);
    if (!fontSurface) {
        LogError("Failed to load '%s'", fontPath);
    }

    sdlFontTexture = SDL_CreateTextureFromSurface(GetDebugWindowRenderer(), fontSurface);
    SDL_FreeSurface(fontSurface);
}

void UITerm()
{
    SDL_DestroyTexture(sdlFontTexture);
}

void UIUpdate()
{
    int state = SDL_GetMouseState(&uiMouse.x, &uiMouse.y);
    bool pressed = (state & SDL_BUTTON(SDL_BUTTON_LEFT));
    uiClick = (pressed && !uiMouseDown);
    uiMouseDown = pressed;
}

void moveCursor(SDL_Rect * r)
{
    switch (uiDirection) {
    case UI_DIR_UP:
        uiCursor.y -= UI_MARGIN;
        break;
    case UI_DIR_DOWN:
        uiCursor.y += r->h + UI_MARGIN;
        break;
    case UI_DIR_LEFT:
        uiCursor.x -= UI_MARGIN;
        break;
    case UI_DIR_RIGHT:
        uiCursor.x += r->w + UI_MARGIN;
        break;
    default:
        break;
    };
}

void adjustRect(SDL_Rect * r)
{
    switch (uiDirection) {
    case UI_DIR_UP:
        r->y -= r->h;
        break;
    case UI_DIR_LEFT:
        r->x -= r->w;
        break;
    default:
        break;
    }
}

void UISetDirection(int dir)
{
    uiDirection = dir;
}

void UISetCursor(int x, int y)
{
    uiCursor.x = x;
    uiCursor.y = y;
}

void UIGetCursor(int * x, int * y)
{
    *x = uiCursor.x;
    *y = uiCursor.y;
}

void UIMoveCursor(int dx, int dy)
{
    uiCursor.x += dx;
    uiCursor.y += dy;
}

void UINewline()
{
    uiCursor.y += UI_LINE_HEIGHT;
}

void UIPrint(const char * format, ...)
{
    static char buffer[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    size_t length = strlen(buffer);

    SDL_Rect src = { 
        .x = 0,
        .y = 0,
        .w = UI_CHAR_WIDTH,
        .h = UI_CHAR_HEIGHT,
    };

    SDL_Rect dst = { 
        .x = uiCursor.x,
        .y = uiCursor.y,
        .w = UI_CHAR_WIDTH,
        .h = UI_CHAR_HEIGHT,
    };

    char * questionMark = strchr(FONT_CHARACTER_MAP, '?');

    for (size_t i = 0; i < length; ++i) {
        if (buffer[i] == ' ') {
            dst.x += UI_CHAR_WIDTH;
            continue;
        }

        char * index = strchr(FONT_CHARACTER_MAP, toupper(buffer[i]));

        if (index == NULL) {
            index = questionMark;
        }

        size_t offset = index - FONT_CHARACTER_MAP;

        src.x = (offset % 8) * UI_CHAR_WIDTH;
        src.y = (offset / 8) * UI_CHAR_WIDTH;
        SDL_RenderCopy(GetDebugWindowRenderer(), sdlFontTexture, &src, &dst);

        dst.x += UI_CHAR_WIDTH;
    }
}

void UIPanel(int w, int h)
{
    SDL_Rect r = {
        .x = uiCursor.x,
        .y = uiCursor.y,
        .w = w,
        .h = h,
    };

    adjustRect(&r);

    SET_COLOR_DEFAULT();
    SDL_RenderFillRect(GetDebugWindowRenderer(), &r);

    SET_COLOR_BORDER();
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &r);

    switch (uiDirection) {
    case UI_DIR_RIGHT:
    case UI_DIR_DOWN:
        uiCursor.x += UI_PANEL_PADDING;
        uiCursor.y += UI_PANEL_PADDING;
        break;
    case UI_DIR_LEFT:
        uiCursor.x += r.w - UI_PANEL_PADDING;
        uiCursor.y += UI_PANEL_PADDING;
        break;
    case UI_DIR_UP:
        uiCursor.x += UI_PANEL_PADDING;
        uiCursor.y += r.y - UI_PANEL_PADDING;
        break;
    default:
        break;
    }
}

bool UIButton(const char * text)
{
    SDL_Rect r = {
        .x = uiCursor.x,
        .y = uiCursor.y,
        .w = (UI_CHAR_WIDTH * strlen(text))
            + (UI_BUTTON_PADDING * 2),
        .h = UI_CHAR_HEIGHT + (UI_BUTTON_PADDING * 2),
    };

    adjustRect(&r);

    bool hover = SDL_PointInRect(&uiMouse, &r);

    if (hover) {
        SET_COLOR_DEFAULT();
    }
    else {
        SET_COLOR_INACTIVE();
    }

    SDL_RenderFillRect(GetDebugWindowRenderer(), &r);

    SET_COLOR_BORDER();
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &r);

    uiCursor.x = r.x + UI_BUTTON_PADDING;
    uiCursor.y = r.y + UI_BUTTON_PADDING;

    UIPrint("%s", text);

    uiCursor.x = r.x;
    uiCursor.y = r.y;

    moveCursor(&r);

    return (hover && uiClick);
}

bool UICheckbox(const char * text, bool * active)
{
    SDL_Rect r = {
        .x = uiCursor.x,
        .y = uiCursor.y,
        .w = (UI_CHAR_WIDTH * strlen(text))
            + (UI_BUTTON_PADDING * 2)
            + (UI_CHAR_WIDTH * 2),
        .h = UI_CHAR_HEIGHT + (UI_BUTTON_PADDING * 2),
    };

    adjustRect(&r);

    bool hover = SDL_PointInRect(&uiMouse, &r);

    if (hover) {
        SET_COLOR_DEFAULT();
    }
    else {
        SET_COLOR_INACTIVE();
    }

    SDL_RenderFillRect(GetDebugWindowRenderer(), &r);

    SET_COLOR_BORDER();
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &r);

    SDL_Rect mark = { 
        .x = r.x + UI_BUTTON_PADDING, 
        .y = r.y + UI_BUTTON_PADDING,
        .w = UI_CHAR_WIDTH, 
        .h = UI_CHAR_WIDTH
    };

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &mark);

    if (*active) {
        ++mark.x;
        ++mark.y;
        mark.w -= 2;
        mark.h -= 2;

        SDL_RenderFillRect(GetDebugWindowRenderer(), &mark);
    }

    uiCursor.x = r.x + UI_BUTTON_PADDING + (UI_CHAR_WIDTH * 2);
    uiCursor.y = r.y + UI_BUTTON_PADDING;

    UIPrint("%s", text);

    uiCursor.x = r.x;
    uiCursor.y = r.y;

    moveCursor(&r);

    if (hover && uiClick) {
        *active ^= true;
    }

    return *active;
}

bool UIRadio(const char * text, int index, int * currentIndex)
{
    SDL_Rect r = {
        .x = uiCursor.x,
        .y = uiCursor.y,
        .w = (UI_CHAR_WIDTH * strlen(text))
            + (UI_BUTTON_PADDING * 2)
            + (UI_CHAR_WIDTH * 2),
        .h = UI_CHAR_HEIGHT 
            + (UI_BUTTON_PADDING * 2),
    };

    adjustRect(&r);

    bool hover = SDL_PointInRect(&uiMouse, &r);
    bool active = (index == *currentIndex);

    if (hover) {
        SET_COLOR_DEFAULT();
    }
    else {
        SET_COLOR_INACTIVE();
    }

    SDL_RenderFillRect(GetDebugWindowRenderer(), &r);
    
    SET_COLOR_BORDER();
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &r);

    SDL_Rect mark = { 
        .x = r.x + UI_BUTTON_PADDING, 
        .y = r.y + UI_BUTTON_PADDING,
        .w = UI_CHAR_WIDTH, 
        .h = UI_CHAR_WIDTH
    };

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &mark);

    if (active) {
        ++mark.x;
        ++mark.y;
        mark.w -= 2;
        mark.h -= 2;

        SDL_RenderFillRect(GetDebugWindowRenderer(), &mark);
    }

    uiCursor.x = r.x + UI_BUTTON_PADDING + (UI_CHAR_WIDTH * 2);
    uiCursor.y = r.y + UI_BUTTON_PADDING;

    UIPrint("%s", text);

    uiCursor.x = r.x;
    uiCursor.y = r.y;

    moveCursor(&r);

    if (hover && uiClick) {
        *currentIndex = index;
    }

    return active;
}

bool UITab(const char * text, int index, int * currentIndex)
{
    SDL_Rect r = {
        .x = uiCursor.x,
        .y = uiCursor.y,
        .w = (UI_CHAR_WIDTH * strlen(text))
            + (UI_BUTTON_PADDING * 2),
        .h = UI_CHAR_HEIGHT 
            + (UI_BUTTON_PADDING * 2),
    };

    adjustRect(&r);

    bool hover = SDL_PointInRect(&uiMouse, &r);
    bool active = (index == *currentIndex);

    if (active || hover) {
        SET_COLOR_DEFAULT();
    }
    else {
        SET_COLOR_INACTIVE();
    }

    SDL_RenderFillRect(GetDebugWindowRenderer(), &r);

    SET_COLOR_BORDER();
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &r);

    uiCursor.x = r.x + UI_BUTTON_PADDING;
    uiCursor.y = r.y + UI_BUTTON_PADDING;

    UIPrint("%s", text);

    uiCursor.x = r.x;
    uiCursor.y = r.y;

    moveCursor(&r);

    if (hover && uiClick) {
        *currentIndex = index;
    }

    return active;
}