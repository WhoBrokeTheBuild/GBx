#include "tileData.h"
#include "../debug.h"

#include <GBx/lcd.h>
#include <GBx/memory.h>

#include <SDL.h>

SDL_Texture * sdlTileDataTexture = NULL;

#define TILE_DATA_TEXTURE_WIDTH  (145)
#define TILE_DATA_TEXTURE_HEIGHT (145)
#define TILE_DATA_TEXTURE_COMP   (3)

checkbox chkTileDataAddrAuto = {
    .x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH,
    .y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 2),
    .text = "AUTO",
};

checkbox chkTileDataAddr8800 = {
    .x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH,
    .y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 8),
    .text = "8800",
};

checkbox chkTileDataAddr8000 = {
    .x = DEBUG_WINDOW_CONTENT_X + DEBUG_CHARACTER_WIDTH,
    .y = DEBUG_WINDOW_CONTENT_Y + (DEBUG_CHARACTER_HEIGHT * 5),
    .text = "8000",
};

enum {
    TILE_DATA_ADDR_SEL_AUTO,
    TILE_DATA_ADDR_SEL_8800,
    TILE_DATA_ADDR_SEL_8000,
};

int tileDataAddrSelect;

void setTileDataAddrAuto()
{
    tileDataAddrSelect = TILE_DATA_ADDR_SEL_AUTO;
    chkTileDataAddr8000.checked = false;
    chkTileDataAddr8800.checked = false;

    TileDataTabRefresh();
}

void setTileDataAddr8800()
{
    tileDataAddrSelect = TILE_DATA_ADDR_SEL_8800;
    chkTileDataAddrAuto.checked = false;
    chkTileDataAddr8000.checked = false;

    TileDataTabRefresh();
}

void setTileDataAddr8000()
{
    tileDataAddrSelect = TILE_DATA_ADDR_SEL_8000;
    chkTileDataAddrAuto.checked = false;
    chkTileDataAddr8800.checked = false;

    TileDataTabRefresh();
}

void InitTileDataTab()
{
    sdlTileDataTexture = SDL_CreateTexture(GetDebugWindowRenderer(),
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        TILE_DATA_TEXTURE_WIDTH, TILE_DATA_TEXTURE_HEIGHT);

    chkTileDataAddrAuto.changed = &setTileDataAddrAuto;
    chkTileDataAddr8800.changed = &setTileDataAddr8800;
    chkTileDataAddr8000.changed = &setTileDataAddr8000;

    chkTileDataAddrAuto.checked = true;
    setTileDataAddrAuto();
}

void TermTileDataTab()
{
    SDL_DestroyTexture(sdlTileDataTexture);
}

void TileDataTabClick(SDL_Point * mouse)
{
    if (CheckCheckboxClick(&chkTileDataAddrAuto, mouse)) {
        chkTileDataAddrAuto.checked ^= true;
        if (chkTileDataAddrAuto.changed) {
            chkTileDataAddrAuto.changed();
        }
    }

    if (CheckCheckboxClick(&chkTileDataAddr8800, mouse)) {
        chkTileDataAddr8800.checked ^= true;
        if (chkTileDataAddr8800.changed) {
            chkTileDataAddr8800.changed();
        }
    }

    if (CheckCheckboxClick(&chkTileDataAddr8000, mouse)) {
        chkTileDataAddr8000.checked ^= true;
        if (chkTileDataAddr8000.changed) {
            chkTileDataAddr8000.changed();
        }
    }
}

void TileDataTabRefresh()
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTileDataTexture, NULL, (void **)&pixels, &pitch);

    word dataAddr = TILE_DATA_ADDR[LCDC.TileDataSelect];

    if (tileDataAddrSelect == TILE_DATA_ADDR_SEL_8800) {
        dataAddr = TILE_DATA_ADDR[0];
    }
    else if (tileDataAddrSelect == TILE_DATA_ADDR_SEL_8000) {
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
                byte color = GetColor(&BGP, tileCol, data1, data2);

                uint off = ((y + tileRow) * pitch) 
                    + ((x + tileCol) * TILE_DATA_TEXTURE_COMP);

                pixels[off + 0] = color;
                pixels[off + 1] = color;
                pixels[off + 2] = color;
            }
        }
    }

    SDL_UnlockTexture(sdlTileDataTexture);
}

void TileDataTabRender(SDL_Point * mouse)
{
    int x = DEBUG_WINDOW_CONTENT_X;
    int y = DEBUG_WINDOW_CONTENT_Y;

    RenderDebugString(x, y, "Tile Data:");

    RenderDebugCheckbox(&chkTileDataAddrAuto, mouse);
    RenderDebugCheckbox(&chkTileDataAddr8000, mouse);
    RenderDebugCheckbox(&chkTileDataAddr8800, mouse);

    x = DEBUG_WINDOW_CONTENT_X + (DEBUG_CHARACTER_WIDTH * 18);
    y = DEBUG_WINDOW_CONTENT_Y;

    SDL_Rect src = { .x = 0, .y = 0, .w = TILE_DATA_TEXTURE_WIDTH, .h = TILE_DATA_TEXTURE_HEIGHT };
    SDL_Rect dst = { .x = x, .y = y, .w = (TILE_DATA_TEXTURE_WIDTH * 2), .h = (TILE_DATA_TEXTURE_HEIGHT * 2) };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlTileDataTexture, &src, &dst);
}
