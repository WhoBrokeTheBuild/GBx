#include "tileMap.h"
#include "../debug.h"

#include <GBx/lcd.h>
#include <GBx/memory.h>

#include <SDL.h>

SDL_Texture * sdlTileMapTexture = NULL;

#define TILE_MAP_TEXTURE_WIDTH  (289)
#define TILE_MAP_TEXTURE_HEIGHT (289)
#define TILE_MAP_TEXTURE_COMP   (3)

checkbox chkTileMapAddrAuto = {
    .x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH,
    .y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 2),
    .text = "AUTO",
};

checkbox chkTileMapAddrWinAuto = {
    .x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH,
    .y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 5),
    .text = "WIN AUTO",
};

checkbox chkTileMapAddrBGAuto = {
    .x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH,
    .y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 8),
    .text = "BG AUTO",
};

checkbox chkTileMapAddr9800 = {
    .x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH,
    .y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 11),
    .text = "9800",
};

checkbox chkTileMapAddr9C00 = {
    .x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH,
    .y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 14),
    .text = "9C00",
};

enum {
    TILE_MAP_ADDR_SEL_AUTO,
    TILE_MAP_ADDR_SEL_WIN_AUTO,
    TILE_MAP_ADDR_SEL_BG_AUTO,
    TILE_MAP_ADDR_SEL_9800,
    TILE_MAP_ADDR_SEL_9C00,
};

int tileMapAddrSelect;

void setTileMapAddrAuto()
{
    tileMapAddrSelect = TILE_MAP_ADDR_SEL_AUTO;
    chkTileMapAddrWinAuto.checked = false;
    chkTileMapAddrBGAuto.checked = false;
    chkTileMapAddr9800.checked = false;
    chkTileMapAddr9C00.checked = false;

    TileMapTabRefresh();
}

void setTileMapAddrWinAuto()
{
    tileMapAddrSelect = TILE_MAP_ADDR_SEL_WIN_AUTO;
    chkTileMapAddrAuto.checked = false;
    chkTileMapAddrBGAuto.checked = false;
    chkTileMapAddr9800.checked = false;
    chkTileMapAddr9C00.checked = false;

    TileMapTabRefresh();
}

void setTileMapAddrBGAuto()
{
    tileMapAddrSelect = TILE_MAP_ADDR_SEL_BG_AUTO;
    chkTileMapAddrAuto.checked = false;
    chkTileMapAddrWinAuto.checked = false;
    chkTileMapAddr9800.checked = false;
    chkTileMapAddr9C00.checked = false;

    TileMapTabRefresh();
}

void setTileMapAddr9800()
{
    tileMapAddrSelect = TILE_MAP_ADDR_SEL_9800;
    chkTileMapAddrAuto.checked = false;
    chkTileMapAddrWinAuto.checked = false;
    chkTileMapAddrBGAuto.checked = false;
    chkTileMapAddr9C00.checked = false;

    TileMapTabRefresh();
}

void setTileMapAddr9C00()
{
    tileMapAddrSelect = TILE_MAP_ADDR_SEL_9C00;
    chkTileMapAddrAuto.checked = false;
    chkTileMapAddrWinAuto.checked = false;
    chkTileMapAddrBGAuto.checked = false;
    chkTileMapAddr9800.checked = false;

    TileMapTabRefresh();
}

void InitTileMapTab()
{
    sdlTileMapTexture = SDL_CreateTexture(GetDebugWindowRenderer(),
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        TILE_MAP_TEXTURE_WIDTH, TILE_MAP_TEXTURE_HEIGHT);

    chkTileMapAddrAuto.changed = &setTileMapAddrAuto;
    chkTileMapAddrWinAuto.changed = &setTileMapAddrWinAuto;
    chkTileMapAddrBGAuto.changed = &setTileMapAddrBGAuto;
    chkTileMapAddr9800.changed = &setTileMapAddr9800;
    chkTileMapAddr9C00.changed = &setTileMapAddr9C00;

    chkTileMapAddrAuto.checked = true;
    setTileMapAddrAuto();
}

void TermTileMapTab()
{
    SDL_DestroyTexture(sdlTileMapTexture);
}

void TileMapTabClick(SDL_Point * mouse)
{
    if (CheckCheckboxClick(&chkTileMapAddrAuto, mouse)) {
        chkTileMapAddrAuto.checked ^= true;
        if (chkTileMapAddrAuto.changed) {
            chkTileMapAddrAuto.changed();
        }
    }

    if (CheckCheckboxClick(&chkTileMapAddrWinAuto, mouse)) {
        chkTileMapAddrWinAuto.checked ^= true;
        if (chkTileMapAddrWinAuto.changed) {
            chkTileMapAddrWinAuto.changed();
        }
    }

    if (CheckCheckboxClick(&chkTileMapAddrBGAuto, mouse)) {
        chkTileMapAddrBGAuto.checked ^= true;
        if (chkTileMapAddrBGAuto.changed) {
            chkTileMapAddrBGAuto.changed();
        }
    }

    if (CheckCheckboxClick(&chkTileMapAddr9800, mouse)) {
        chkTileMapAddr9800.checked ^= true;
        if (chkTileMapAddr9800.changed) {
            chkTileMapAddr9800.changed();
        }
    }

    if (CheckCheckboxClick(&chkTileMapAddr9C00, mouse)) {
        chkTileMapAddr9C00.checked ^= true;
        if (chkTileMapAddr9C00.changed) {
            chkTileMapAddr9C00.changed();
        }
    }
}

void TileMapTabRefresh()
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTileMapTexture, NULL, (void **)&pixels, &pitch);

    word dataAddr = TILE_MAP_ADDR[
        (LCDC.WindowDisplayEnabled ? 
            LCDC.WindowTileMapSelect : 
            LCDC.BGTileMapSelect)
    ];

    if (tileMapAddrSelect == TILE_MAP_ADDR_SEL_WIN_AUTO) {
        dataAddr = TILE_MAP_ADDR[LCDC.WindowTileMapSelect];
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_SEL_BG_AUTO) {
        dataAddr = TILE_MAP_ADDR[LCDC.BGTileMapSelect];
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_SEL_9800) {
        dataAddr = TILE_MAP_ADDR[0];
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_SEL_9C00) {
        dataAddr = TILE_MAP_ADDR[1];
    }

    const int TILE_DRAW_ROWS = 32;
    const int TILE_DRAW_COLS = 32;

    for (int row = 0; row < TILE_DRAW_ROWS; ++row) {
        int y = 1 + (row * (TILE_HEIGHT + 1));
        for (int col = 0; col < TILE_DRAW_COLS; ++col) {
            int x = 1 + (col * (TILE_WIDTH + 1));

            int tileIndex = ReadByte(dataAddr + (row * TILES_PER_ROW) + col);

            if (LCDC.TileDataSelect == 0) {
                tileIndex = (sbyte)tileIndex; // Convert to [-128, 127]
            }

            for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {
                word dataOffset = TILE_DATA_ADDR[LCDC.TileDataSelect] 
                    + (tileIndex * TILE_DATA_SIZE) + (tileRow * 2);

                byte data1 = ReadByte(dataOffset);
                byte data2 = ReadByte(dataOffset + 1);

                for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                    byte color = GetColor(&BGP, tileCol, data1, data2);

                    uint off = ((y + tileRow) * pitch) 
                        + ((x + tileCol) * LCD_BUFFER_COMPONENTS);

                    pixels[off + 0] = color;
                    pixels[off + 1] = color;
                    pixels[off + 2] = color;
                }
            }
        }
    }

    SDL_UnlockTexture(sdlTileMapTexture);
}

void TileMapTabRender(SDL_Point * mouse)
{
    int x = DEBUG_WINDOW_CONTENT_X;
    int y = DEBUG_WINDOW_CONTENT_Y;

    RenderDebugString(x, y, "Tile Map:");

    RenderDebugCheckbox(&chkTileMapAddrAuto, mouse);
    RenderDebugCheckbox(&chkTileMapAddrWinAuto, mouse);
    RenderDebugCheckbox(&chkTileMapAddrBGAuto, mouse);
    RenderDebugCheckbox(&chkTileMapAddr9800, mouse);
    RenderDebugCheckbox(&chkTileMapAddr9C00, mouse);

    x = DEBUG_WINDOW_CONTENT_X + (DEBUG_CHARACTER_WIDTH * 18);
    y = DEBUG_WINDOW_CONTENT_Y;

    SDL_Rect src = { .x = 0, .y = 0, .w = TILE_MAP_TEXTURE_WIDTH, .h = TILE_MAP_TEXTURE_WIDTH };
    SDL_Rect dst = { .x = x, .y = y, .w = (TILE_MAP_TEXTURE_HEIGHT * 2), .h = (TILE_MAP_TEXTURE_HEIGHT * 2) };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlTileMapTexture, &src, &dst);

    x = DEBUG_WINDOW_CONTENT_X + (DEBUG_CHARACTER_WIDTH * 18);
    y = DEBUG_WINDOW_CONTENT_Y;

    int scx = (SCX / 8.0) * 9.0;
    int scy = (SCY / 8.0) * 9.0;

    int width  = ((LCD_WIDTH / 8.0) * 9.0) + 1;
    int height = ((LCD_HEIGHT / 8.0) * 9.0) + 1;

    SDL_Rect scroll = {
        .x = x + (scx * 2),
        .y = y + (scy * 2),
        .w = width * 2, 
        .h = height * 2,
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(),
        0xFF, 0x00, 0x00, 0xFF);

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &scroll);

    ++scroll.x;
    ++scroll.y;
    scroll.w -= 2;
    scroll.h -= 2;

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &scroll);

    scroll.x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH;
    scroll.y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 20);
    scroll.w = (DEBUG_CHARACTER_HEIGHT * 7);
    scroll.h = (DEBUG_CHARACTER_HEIGHT * 2);

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &scroll);
    RenderDebugString(scroll.x + (DEBUG_CHARACTER_WIDTH / 2), scroll.y + (DEBUG_CHARACTER_HEIGHT / 2), "SCROLL");

    int wx = (((WX - 7) / 8.0) * 9.0) - 1;
    int wy = ((WY / 8.0) * 9.0) - 1;

    SDL_Rect window = {
        .x = x + (wx * 2),
        .y = y + (wy * 2),
        .w = width * 2, 
        .h = height * 2,
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(),
        0x00, 0x00, 0xFF, 0xFF);

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &window);
    
    ++window.x;
    ++window.y;
    window.w -= 2;
    window.h -= 2;

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &window);

    window.x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH;
    window.y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 24);
    window.w = (DEBUG_CHARACTER_HEIGHT * 7);
    window.h = (DEBUG_CHARACTER_HEIGHT * 2);

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &window);
    RenderDebugString(window.x + (DEBUG_CHARACTER_WIDTH / 2), window.y + (DEBUG_CHARACTER_HEIGHT / 2), "WINDOW");
}
