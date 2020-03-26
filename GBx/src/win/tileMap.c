#include "tileMap.h"
#include "../debug.h"

#include <GBx/lcd.h>
#include <GBx/memory.h>

#include <SDL.h>

SDL_Texture * sdlTileMapTexture = NULL;

#define TILE_MAP_TEXTURE_WIDTH  (289)
#define TILE_MAP_TEXTURE_HEIGHT (289)
#define TILE_MAP_TEXTURE_COMP   (3)

enum {
    TILE_MAP_ADDR_AUTO,
    TILE_MAP_ADDR_WIN,
    TILE_MAP_ADDR_BG,
    TILE_MAP_ADDR_9800,
    TILE_MAP_ADDR_9C00,
};

int tileMapAddrSelect = TILE_MAP_ADDR_AUTO;

void InitTileMapTab()
{
    sdlTileMapTexture = SDL_CreateTexture(GetDebugWindowRenderer(),
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        TILE_MAP_TEXTURE_WIDTH, TILE_MAP_TEXTURE_HEIGHT);
}

void TermTileMapTab()
{
    SDL_DestroyTexture(sdlTileMapTexture);
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

    if (tileMapAddrSelect == TILE_MAP_ADDR_WIN) {
        dataAddr = TILE_MAP_ADDR[LCDC.WindowTileMapSelect];
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_BG) {
        dataAddr = TILE_MAP_ADDR[LCDC.BGTileMapSelect];
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_9800) {
        dataAddr = TILE_MAP_ADDR[0];
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_9C00) {
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
                    byte * color = GetColor(&BGP, tileCol, data1, data2);

                    uint off = ((y + tileRow) * pitch) 
                        + ((x + tileCol) * LCD_BUFFER_COMPONENTS);

                    pixels[off + 0] = color[0];
                    pixels[off + 1] = color[1];
                    pixels[off + 2] = color[2];
                }
            }
        }
    }

    SDL_UnlockTexture(sdlTileMapTexture);
}

void TileMapTabRender(SDL_Point * mouse)
{
    int startX, startY;
    DebugGetCursor(&startX, &startY);

    DebugPrintln("TILE MAP:");
    DebugNewline();

    DebugSetDirection(DEBUG_DIR_DOWN);

    DebugRadio("AUTO", TILE_MAP_ADDR_AUTO, &tileMapAddrSelect);
    DebugRadio("WIN", TILE_MAP_ADDR_WIN, &tileMapAddrSelect);
    DebugRadio("BG", TILE_MAP_ADDR_BG, &tileMapAddrSelect);
    DebugRadio("9800", TILE_MAP_ADDR_9800, &tileMapAddrSelect);
    DebugRadio("9C00", TILE_MAP_ADDR_9C00, &tileMapAddrSelect);

    DebugSetDirection(DEBUG_DIR_RIGHT);

    SDL_Rect src = {
        .x = 0,
        .y = 0,
        .w = TILE_MAP_TEXTURE_WIDTH,
        .h = TILE_MAP_TEXTURE_WIDTH,
    };
    
    SDL_Rect dst = {
        .x = startX + (DEBUG_CHAR_WIDTH * 18),
        .y = startY,
        .w = (TILE_MAP_TEXTURE_HEIGHT * 2),
        .h = (TILE_MAP_TEXTURE_HEIGHT * 2),
    };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlTileMapTexture, &src, &dst);

    int scx = (SCX / 8.0) * 9.0;
    int scy = (SCY / 8.0) * 9.0;

    int width  = ((LCD_WIDTH / 8.0) * 9.0) + 1;
    int height = ((LCD_HEIGHT / 8.0) * 9.0) + 1;

    SDL_Rect scroll = {
        .x = dst.x + (scx * 2),
        .y = dst.y + (scy * 2),
        .w = width * 2, 
        .h = height * 2,
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &scroll);

    ++scroll.x;
    ++scroll.y;
    scroll.w -= 2;
    scroll.h -= 2;

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &scroll);

    scroll.x = startX;
    scroll.y = startY + (DEBUG_CHAR_HEIGHT * 20);
    scroll.w = (DEBUG_CHAR_HEIGHT * 7);
    scroll.h = (DEBUG_CHAR_HEIGHT * 2);

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &scroll);

    DebugSetCursor(scroll.x + (DEBUG_CHAR_WIDTH / 2), scroll.y + (DEBUG_CHAR_HEIGHT / 2));
    DebugPrint("SCROLL");

    int wx = (((WX - 7) / 8.0) * 9.0) - 1;
    int wy = ((WY / 8.0) * 9.0) - 1;

    SDL_Rect window = {
        .x = dst.x + (wx * 2),
        .y = dst.y + (wy * 2),
        .w = width * 2, 
        .h = height * 2,
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &window);
    
    ++window.x;
    ++window.y;
    window.w -= 2;
    window.h -= 2;

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &window);

    window.x = startX;
    window.y = startY + (DEBUG_CHAR_HEIGHT * 24);
    window.w = (DEBUG_CHAR_HEIGHT * 7);
    window.h = (DEBUG_CHAR_HEIGHT * 2);

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &window);

    DebugSetCursor(window.x + (DEBUG_CHAR_WIDTH / 2), window.y + (DEBUG_CHAR_HEIGHT / 2));
    DebugPrint("WINDOW");
}
