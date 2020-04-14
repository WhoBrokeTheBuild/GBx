#include "tileData.h"
#include "../debug.h"

#include <GBx/GBx.h>
#include <DUI/DUI.h>
#include <SDL.h>

SDL_Texture * sdlTileDataTexture = NULL;

#define TILE_DATA_TEXTURE_WIDTH  (145)
#define TILE_DATA_TEXTURE_HEIGHT (145)
#define TILE_DATA_TEXTURE_COMP   (3)

enum {
    TILE_DATA_ADDR_AUTO,
    TILE_DATA_ADDR_8800,
    TILE_DATA_ADDR_8000,
};

int tileDataAddrSelect;

void InitTileDataTab(gbx_t * ctx)
{
    sdlTileDataTexture = SDL_CreateTexture(GetDebugWindowRenderer(),
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        TILE_DATA_TEXTURE_WIDTH, TILE_DATA_TEXTURE_HEIGHT);
}

void TermTileDataTab(gbx_t * ctx)
{
    SDL_DestroyTexture(sdlTileDataTexture);
}

void TileDataTabRefresh(gbx_t * ctx)
{
    const int TILE_WIDTH     = 8;
    const int TILE_HEIGHT    = 8;
    const int TILE_DATA_SIZE = 16;
    const int TILES_PER_ROW  = 32;

    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTileDataTexture, NULL, (void **)&pixels, &pitch);

    uint16_t dataAddr = GBx_GetTileDataAddress(ctx->LCDC.TileDataSelect);

    if (tileDataAddrSelect == TILE_DATA_ADDR_8800) {
        dataAddr = GBx_GetTileDataAddress(0);
    }
    else if (tileDataAddrSelect == TILE_DATA_ADDR_8000) {
        dataAddr = GBx_GetTileDataAddress(1);
    }

    const int TILE_DRAW_PER_ROW = 16;
    const int TILE_COUNT = 256;

    for (int i = 0; i < TILE_COUNT; ++i) {

        int x = 1 + ((i % TILE_DRAW_PER_ROW) * (TILE_WIDTH + 1));
        int y = 1 + ((i / TILE_DRAW_PER_ROW) * (TILE_HEIGHT + 1));

        for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {

            uint16_t dataOffset = dataAddr;
            if (dataAddr == 0x8800) {
                dataOffset += ((i - 128) * TILE_DATA_SIZE) + (tileRow * 2);
            }
            else {
                dataOffset += (i * TILE_DATA_SIZE) + (tileRow * 2);
            }

            uint8_t data1 = GBx_ReadByte(ctx, dataOffset);
            uint8_t data2 = GBx_ReadByte(ctx, dataOffset + 1);

            for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                const uint8_t * color = GBx_GetPixelColor(ctx, &ctx->BGP, tileCol, data1, data2);

                unsigned off = ((y + tileRow) * pitch) 
                    + ((x + tileCol) * TILE_DATA_TEXTURE_COMP);

                pixels[off + 0] = color[0];
                pixels[off + 1] = color[0];
                pixels[off + 2] = color[0];
            }
        }
    }

    SDL_UnlockTexture(sdlTileDataTexture);
}

void TileDataTabRender(gbx_t * ctx)
{
    int startX, startY;
    DUI_GetCursor(&startX, &startY);

    DUI_Println("TILE DATA:");
    DUI_Newline();

    DUI_Radio("AUTO", TILE_DATA_ADDR_AUTO, &tileDataAddrSelect);
    DUI_Newline();
    DUI_Newline();

    DUI_Radio("8000", TILE_DATA_ADDR_8000, &tileDataAddrSelect);
    DUI_Newline();
    DUI_Newline();

    DUI_Radio("8800", TILE_DATA_ADDR_8800, &tileDataAddrSelect);
    DUI_Newline();
    DUI_Newline();

    SDL_Rect dst = {
        .x = startX + (DUI_GetStyle()->CharWidth * 18),
        .y = startY,
        .w = (TILE_DATA_TEXTURE_WIDTH * 2),
        .h = (TILE_DATA_TEXTURE_HEIGHT * 2),
    };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlTileDataTexture, NULL, &dst);
}
