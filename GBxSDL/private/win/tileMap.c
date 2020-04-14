#include "tileMap.h"
#include "../debug.h"

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
bool showTileMapWindow = false;

void InitTileMapTab(gbx_t * ctx)
{
    sdlTileMapTexture = SDL_CreateTexture(GetDebugWindowRenderer(),
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        TILE_MAP_TEXTURE_WIDTH, TILE_MAP_TEXTURE_HEIGHT);
}

void TermTileMapTab(gbx_t * ctx)
{
    SDL_DestroyTexture(sdlTileMapTexture);
}

void TileMapTabRefresh(gbx_t * ctx)
{
    const int TILE_WIDTH     = 8;
    const int TILE_HEIGHT    = 8;
    const int TILE_DATA_SIZE = 16;
    const int TILES_PER_ROW  = 32;

    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTileMapTexture, NULL, (void **)&pixels, &pitch);

    uint16_t mapAddr = GBx_GetTileMapAddress(
        (ctx->LCDC.WindowDisplayEnabled ? 
            ctx->LCDC.WindowTileMapSelect : 
            ctx->LCDC.BGTileMapSelect)
    );

    if (tileMapAddrSelect == TILE_MAP_ADDR_WIN) {
        mapAddr = GBx_GetTileMapAddress(ctx->LCDC.WindowTileMapSelect);
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_BG) {
        mapAddr = GBx_GetTileMapAddress(ctx->LCDC.BGTileMapSelect);
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_9800) {
        mapAddr = GBx_GetTileMapAddress(0);
    }
    else if (tileMapAddrSelect == TILE_MAP_ADDR_9C00) {
        mapAddr = GBx_GetTileMapAddress(1);
    }

    const int TILE_DRAW_ROWS = 32;
    const int TILE_DRAW_COLS = 32;

    for (int row = 0; row < TILE_DRAW_ROWS; ++row) {
        int y = 1 + (row * (TILE_HEIGHT + 1));
        for (int col = 0; col < TILE_DRAW_COLS; ++col) {
            int x = 1 + (col * (TILE_WIDTH + 1));

            int tileIndex = GBx_ReadByte(ctx, mapAddr + (row * TILES_PER_ROW) + col);

            if (ctx->LCDC.TileDataSelect == 0) {
                tileIndex = (int8_t)tileIndex; // Convert to [-128, 127]
            }

            for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {
                uint16_t dataOffset = GBx_GetTileDataAddress(ctx->LCDC.TileDataSelect)
                    + (tileIndex * TILE_DATA_SIZE) + (tileRow * 2);

                uint8_t data1 = GBx_ReadByte(ctx, dataOffset);
                uint8_t data2 = GBx_ReadByte(ctx, dataOffset + 1);

                for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                    const uint8_t * color = GBx_GetPixelColor(ctx, &ctx->BGP, tileCol, data1, data2);

                    unsigned off = ((y + tileRow) * pitch) 
                        + ((x + tileCol) * GBX_BACKBUFFER_COMPONENTS);

                    pixels[off + 0] = color[0];
                    pixels[off + 1] = color[1];
                    pixels[off + 2] = color[2];
                }
            }
        }
    }

    SDL_UnlockTexture(sdlTileMapTexture);
}

void TileMapTabRender(gbx_t * ctx)
{
    DUI_Style * style = DUI_GetStyle();

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

    SDL_Rect dst = {
        .x = startX + (style->CharWidth * 18),
        .y = startY,
        .w = (TILE_MAP_TEXTURE_HEIGHT * 2),
        .h = (TILE_MAP_TEXTURE_HEIGHT * 2),
    };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlTileMapTexture, NULL, &dst);

    int scx = (ctx->SCX / 8.0) * 9.0;
    int scy = (ctx->SCY / 8.0) * 9.0;

    int width  = ((GBX_LCD_WIDTH / 8.0) * 9.0) + 1;
    int height = ((GBX_LCD_HEIGHT / 8.0) * 9.0) + 1;

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
        .y = startY + (style->CharHeight * 24),
        .w = (style->CharWidth * 7),
        .h = (style->CharHeight * 2),
    };

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &scrollLegend);

    DUI_PrintAt(
        scrollLegend.x + (style->CharWidth / 2), 
        scrollLegend.y + (style->CharHeight / 2),
        "SCROLL");

    DUI_CheckboxAt(
        startX,
        startY + (style->CharHeight * 20),
        "SCROLL",
        &showTileMapScroll
    );

    int wx = ((ctx->WX / 8.0) * 9.0);
    int wy = ((ctx->WY / 8.0) * 9.0);

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
        .y = startY + (style->CharHeight * 32),
        .w = (style->CharWidth * 7),
        .h = (style->CharHeight * 2),
    };

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &windowLegend);

    DUI_PrintAt(
        windowLegend.x + (style->CharWidth / 2), 
        windowLegend.y + (style->CharHeight / 2), 
        "WINDOW");

    DUI_CheckboxAt(
        startX,
        startY + (style->CharHeight * 28),
        "WINDOW",
        &showTileMapWindow
    );
}
