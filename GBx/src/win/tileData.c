#include "tileData.h"
#include "../debug.h"
#include "../ui.h"

#include <GBx/lcd.h>
#include <GBx/memory.h>

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

void InitTileDataTab()
{
    sdlTileDataTexture = SDL_CreateTexture(GetDebugWindowRenderer(),
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        TILE_DATA_TEXTURE_WIDTH, TILE_DATA_TEXTURE_HEIGHT);
}

void TermTileDataTab()
{
    SDL_DestroyTexture(sdlTileDataTexture);
}

void TileDataTabRefresh()
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTileDataTexture, NULL, (void **)&pixels, &pitch);

    word dataAddr = TILE_DATA_ADDR[LCDC.TileDataSelect];

    if (tileDataAddrSelect == TILE_DATA_ADDR_8800) {
        dataAddr = TILE_DATA_ADDR[0];
    }
    else if (tileDataAddrSelect == TILE_DATA_ADDR_8000) {
        dataAddr = TILE_DATA_ADDR[1];
    }

    const int TILE_DRAW_PER_ROW = 16;
    const int TILE_COUNT = 256;

    for (int i = 0; i < TILE_COUNT; ++i) {

        int x = 1 + ((i % TILE_DRAW_PER_ROW) * (TILE_WIDTH + 1));
        int y = 1 + ((i / TILE_DRAW_PER_ROW) * (TILE_HEIGHT + 1));

        for (int tileRow = 0; tileRow < TILE_HEIGHT; ++tileRow) {

            word dataOffset = dataAddr;
            if (dataAddr == 0x8800) {
                dataOffset += ((i - 128) * TILE_DATA_SIZE) + (tileRow * 2);
            }
            else {
                dataOffset += (i * TILE_DATA_SIZE) + (tileRow * 2);
            }

            byte data1 = ReadByte(dataOffset);
            byte data2 = ReadByte(dataOffset + 1);

            for (int tileCol = 0; tileCol < TILE_WIDTH; ++tileCol) {
                byte * color = GetColor(&BGP, tileCol, data1, data2);

                uint off = ((y + tileRow) * pitch) 
                    + ((x + tileCol) * TILE_DATA_TEXTURE_COMP);

                pixels[off + 0] = color[0];
                pixels[off + 1] = color[0];
                pixels[off + 2] = color[0];
            }
        }
    }

    SDL_UnlockTexture(sdlTileDataTexture);
}

void TileDataTabRender(SDL_Point * mouse)
{
    int startX, startY;
    UIGetCursor(&startX, &startY);

    UIPrintln("TILE DATA:");
    UINewline();

    UISetDirection(UI_DIR_DOWN);

    UIRadio("AUTO", TILE_DATA_ADDR_AUTO, &tileDataAddrSelect);
    UIRadio("8000", TILE_DATA_ADDR_8000, &tileDataAddrSelect);
    UIRadio("8800", TILE_DATA_ADDR_8800, &tileDataAddrSelect);

    UISetDirection(UI_DIR_RIGHT);

    SDL_Rect src = {
        .x = 0,
        .y = 0,
        .w = TILE_DATA_TEXTURE_WIDTH,
        .h = TILE_DATA_TEXTURE_HEIGHT,
    };

    SDL_Rect dst = {
        .x = startX + (UI_CHAR_WIDTH * 18),
        .y = startY,
        .w = (TILE_DATA_TEXTURE_WIDTH * 2),
        .h = (TILE_DATA_TEXTURE_HEIGHT * 2),
    };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlTileDataTexture, &src, &dst);
}
