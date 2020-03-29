#include "tileMap.h"
#include "../debug.h"

#include <GBx/GBx.h>
#include <DUI/DUI.h>
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

bool showTileMapScroll = true;
bool showTileMapWindow = true;

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
    const int TILE_WIDTH     = 8;
    const int TILE_HEIGHT    = 8;
    const int TILE_DATA_SIZE = 16;
    const int TILES_PER_ROW  = 32;

    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTileMapTexture, NULL, (void **)&pixels, &pitch);

    word mapAddr = GetTileMapAddress(
        (LCDC.WindowDisplayEnabled ? 
            LCDC.WindowTileMapSelect : 
            LCDC.BGTileMapSelect)
    );

    if (tileMapAddrSelect == TILE_MAP_ADDR_WIN) {
        mapAddr = GetTileMapAddress(LCDC.WindowTileMapSelect);
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_BG) {
        mapAddr = GetTileMapAddress(LCDC.BGTileMapSelect);
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_9800) {
        mapAddr = GetTileMapAddress(0);
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_9C00) {
        mapAddr = GetTileMapAddress(1);
    }

    const int TILE_DRAW_ROWS = 32;
    const int TILE_DRAW_COLS = 32;

    for (int row = 0; row < TILE_DRAW_ROWS; ++row) {
        int y = 1 + (row * (TILE_HEIGHT + 1));
        for (int col = 0; col < TILE_DRAW_COLS; ++col) {
            int x = 1 + (col * (TILE_WIDTH + 1));

            int tileIndex = ReadByte(mapAddr + (row * TILES_PER_ROW) + col);

            if (LCDC.TileDataSelect == 0) {
                tileIndex = (sbyte)tileIndex; // Convert to [-128, 127]
            }

            for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {
                word dataOffset = GetTileDataAddress(LCDC.TileDataSelect)
                    + (tileIndex * TILE_DATA_SIZE) + (tileRow * 2);

                byte data1 = ReadByte(dataOffset);
                byte data2 = ReadByte(dataOffset + 1);

                for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                    const byte * color = GetColor(&BGP, tileCol, data1, data2);

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
    DUI_MoveCursor(DEBUG_CONTENT_X, DEBUG_CONTENT_Y);
    DUI_Panel(DEBUG_CONTENT_WIDTH, DEBUG_CONTENT_HEIGHT);

    int startX, startY;
    DUI_GetCursor(&startX, &startY);

    DUI_Println("TILE MAP:");
    DUI_Newline();

    DUI_Radio("AUTO", TILE_MAP_ADDR_AUTO, &tileMapAddrSelect);
    DUI_Newline();
    DUI_Newline();

    DUI_Radio("WIN ", TILE_MAP_ADDR_WIN, &tileMapAddrSelect);
    DUI_Newline();
    DUI_Newline();
    
    DUI_Radio("BG  ", TILE_MAP_ADDR_BG, &tileMapAddrSelect);
    DUI_Newline();
    DUI_Newline();
    
    DUI_Radio("9800", TILE_MAP_ADDR_9800, &tileMapAddrSelect);
    DUI_Newline();
    DUI_Newline();
    
    DUI_Radio("9C00", TILE_MAP_ADDR_9C00, &tileMapAddrSelect);

    SDL_Rect src = {
        .x = 0,
        .y = 0,
        .w = TILE_MAP_TEXTURE_WIDTH,
        .h = TILE_MAP_TEXTURE_WIDTH,
    };
    
    SDL_Rect dst = {
        .x = startX + (DUI_GetStyle()->CharSize * 18),
        .y = startY,
        .w = (TILE_MAP_TEXTURE_HEIGHT * 2),
        .h = (TILE_MAP_TEXTURE_HEIGHT * 2),
    };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlTileMapTexture, &src, &dst);

    int scx = (SCX / 8.0) * 9.0;
    int scy = (SCY / 8.0) * 9.0;

    int width  = ((LCD_WIDTH / 8.0) * 9.0) + 1;
    int height = ((LCD_HEIGHT / 8.0) * 9.0) + 1;

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0xFF, 0x00, 0x00, 0xFF);
        
    if (showTileMapScroll) {
        int scrollX = dst.x + (scx * 2);
        int scrollY = dst.y + (scy * 2);

        SDL_Rect scroll[2] = {
            {
                .x = scrollX,
                .y = scrollY,
                .w = (width * 2), 
                .h = (height * 2),
            },
            {
                .x = scrollX + 1,
                .y = scrollY + 1,
                .w = (width * 2) - 1, 
                .h = (height * 2) - 1,
            },
        };

        SDL_RenderDrawRects(GetDebugWindowRenderer(), scroll, 2);
    }

    SDL_Rect scrollLegend = {
        .x = startX,
        .y = startY + (DUI_GetStyle()->CharSize * 24),
        .w = (DUI_GetStyle()->CharSize * 7),
        .h = (DUI_GetStyle()->CharSize * 2),
    };

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &scrollLegend);

    DUI_PrintAt(
        scrollLegend.x + (DUI_GetStyle()->CharSize / 2), 
        scrollLegend.y + (DUI_GetStyle()->CharSize / 2),
        "SCROLL");

    DUI_CheckboxAt(
        startX,
        startY + (DUI_GetStyle()->CharSize * 20),
        "SCROLL",
        &showTileMapScroll
    );

    int wx = (((WX - 7) / 8.0) * 9.0) - 1;
    int wy = ((WY / 8.0) * 9.0) - 1;

    SDL_Rect window = {
        .x = dst.x + (wx * 2),
        .y = dst.y + (wy * 2),
        .w = width * 2, 
        .h = height * 2,
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0x00, 0x00, 0xFF, 0xFF);

    if (showTileMapWindow) {
        int windowX = dst.x + (wx * 2);
        int windowY = dst.y + (wy * 2);

        SDL_Rect window[2] = {
            {
                .x = windowX,
                .y = windowY,
                .w = (width * 2), 
                .h = (height * 2),
            },
            {
                .x = windowX + 1,
                .y = windowY + 1,
                .w = (width * 2) - 1, 
                .h = (height * 2) - 1,
            },
        };

        SDL_RenderDrawRects(GetDebugWindowRenderer(), window, 2);
    }

    SDL_Rect windowLegend = {
        .x = startX,
        .y = startY + (DUI_GetStyle()->CharSize * 32),
        .w = (DUI_GetStyle()->CharSize * 7),
        .h = (DUI_GetStyle()->CharSize * 2),
    };

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &windowLegend);

    DUI_PrintAt(
        windowLegend.x + (DUI_GetStyle()->CharSize / 2), 
        windowLegend.y + (DUI_GetStyle()->CharSize / 2), 
        "WINDOW");

    DUI_CheckboxAt(
        startX,
        startY + (DUI_GetStyle()->CharSize * 28),
        "WINDOW",
        &showTileMapWindow
    );
}
