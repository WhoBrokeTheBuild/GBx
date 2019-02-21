#include "video.h"
#include "memory.h"
#include <SDL.h>

LCDC_t LCDC = { 
    .BGWindowDisplay = false,
    .SpriteDisplay = false,
    .SpriteSize = 0,
    .BGTileMapSelect = 0,
    .BGTileDataSelect = 0,
    .WindowDisplay = false,
    .WindowTileMapSelect = 0,
    .LCDEnable = false,
};
STAT_t STAT;
uint8_t SCY = 0x00;
uint8_t SCX = 0x00;
uint8_t LY  = 0x00;
uint8_t LYC = 0x00;

uint8_t CharacterRAM[0x1800];
uint8_t BGMapData1[0x400];
uint8_t BGMapData2[0x400];

uint64_t LCDTicks = 0;

SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;

void lcdInit()
{
    if (SDL_CreateWindowAndRenderer(160, 144, 0, &window, &renderer)) {
        LogFatal("failed to create SDL2 window/renderer, %s", SDL_GetError());
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void lcdTerm()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void lcdTick(unsigned cycles)
{
    const unsigned SCANLINE_TICK_COUNT = 456;
    static unsigned scanlineCounter = 0;

    if (!LCDC.LCDEnable) {
        scanlineCounter = 0;
        LY = 0;
        STAT.IntHBlank = false;
        STAT.IntVBlank = false;
        STAT.IntSearchSprite = false;
        STAT.IntLYCLY = false;
    }

    LCDTicks += cycles;
    scanlineCounter += cycles;

    uint8_t line = readByte(0xFF44);
    uint8_t oldMode = STAT.Mode;
    bool requestInterrupt = false;

    if (line >= 144) {
        STAT.Mode = MODE_VBLANK;
        requestInterrupt = STAT.IntVBlank;
    } else {
        const int MODE2 = 80;
        const int MODE3 = 252;
        if (scanlineCounter <= MODE2) {
            STAT.Mode = MODE_SEARCH_SPRITE;
            requestInterrupt = STAT.IntSearchSprite;
        } else if (scanlineCounter <= MODE3) {
            STAT.Mode = MODE_DATA_TRANSFER;
        } else {
            STAT.Mode = MODE_HBLANK;
            requestInterrupt = STAT.IntHBlank;
        }
    }

    if (requestInterrupt && (oldMode != STAT.Mode)) {
        // Inerrupt 1
    }

    STAT.Coincidence = (LY == LYC);
    if (STAT.Coincidence && STAT.IntLYCLY) {
        // Interrupt 1
    }

    if (scanlineCounter >= SCANLINE_TICK_COUNT) {
        scanlineCounter = SCANLINE_TICK_COUNT;

        
        ++line;
        if (line > 153) {
            line = 0;
        }
        writeByte(0xFF44, line);

        if (line == 144) {
            STAT.Mode = MODE_VBLANK;
            // Interrupt 0
        }
        else if (line < 144) {

            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
    }

    //SDL_RenderClear(renderer);
    //SDL_RenderPresent(renderer);
}