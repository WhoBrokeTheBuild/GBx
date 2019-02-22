#include "video.h"
#include "memory.h"
#include <SDL.h>

LCDC_t LCDC = { 
    .TileDisplay = false,
    .SpriteDisplay = false,
    .SpriteSize = 0,
    .TileMapSelect = 0,
    .TileDataSelect = 0,
    .WindowDisplay = false,
    .WindowTileMapSelect = 0,
    .LCDEnable = false,
};
STAT_t STAT;
uint8_t SCY = 0x00;
uint8_t SCX = 0x00;
uint8_t LY  = 0x00;
uint8_t LYC = 0x00;
palette_t BGP = { .data = 0b11100100 };
palette_t OBP0 = { .data = 0b11100100 };
palette_t OBP1 = { .data = 0b11100100 };
uint8_t WX = 0x00;
uint8_t WY = 0x00;

uint8_t CharacterRAM[0x1800];
uint8_t BGMapData1[0x400];
uint8_t BGMapData2[0x400];
uint8_t OAM[0xA0];

uint64_t LCDTicks = 0;

SDL_Window * sdlWindow = NULL;
SDL_Renderer * sdlRenderer = NULL;
SDL_Texture * sdlTexture = NULL;

uint8_t VRAM[160*144*3];

const int SCALE = 1;

void drawTiles(uint8_t * pixels, int pitch)
{
    int y = 0;
    if (LCDC.WindowDisplay) {
        y = LY - WY;
    } else {
        y = SCY + LY;
    }

    uint16_t row = (y / 8) * 32;

    for (unsigned pixel = 0; pixel < 160; ++pixel) {
        uint8_t x = pixel + SCX;
        if (LCDC.WindowDisplay) {
            if (pixel >= WX - 7) {
                x = pixel - (WX - 7);
            }
        }


        uint16_t tileBaseAddress;
        if (LCDC.WindowDisplay) {
            if (LCDC.WindowTileMapSelect == 0) {
                tileBaseAddress = 0x9800;
            } else {
                tileBaseAddress = 0x9C00;
            }
        } else {
            if (LCDC.TileMapSelect == 0) {
                tileBaseAddress = 0x9800;
            } else {
                tileBaseAddress = 0x9C00;
            }
        }

        uint16_t col = x / 8;

        uint16_t tileOffset;
        if (LCDC.TileDataSelect == 0) {
            int8_t tileIndex = (int8_t)readByte(tileBaseAddress + row + col);
            tileOffset = 0x8800 + ((tileIndex + 128) * 16);
        } else {
            uint8_t tileIndex = readByte(tileBaseAddress + row + col);
            tileOffset = 0x8000 + (tileIndex * 16);
        }

        uint8_t line = (y % 8) * 2;
        uint8_t data1 = readByte(tileOffset + line);
        uint8_t data2 = readByte(tileOffset + line + 1);
        //LogInfo("%02X %02X %04X", data1, data2, tileOffset);

        int bit = (x % 8);
        bit -= 7;
        bit *= -1;
        bit = 1 << bit;
        uint8_t colorIndex = ((data2 & bit) ? 2 : 0) | ((data1 & bit) ? 1 : 0);
        
        uint8_t color = 0;
        switch (colorIndex) {
        case 0b00:
            colorIndex = BGP.Color00;
            break;
        case 0b01:
            colorIndex = BGP.Color01;
            break;
        case 0b10:
            colorIndex = BGP.Color10;
            break;
        case 0b11:
            colorIndex = BGP.Color11;
            break;
        }

        switch (colorIndex) {
        case 0b00:
            color = 0xFF;
            break;
        case 0b01:
            color = 0xCC;
            break;
        case 0b10:
            color = 0x77;
            break;
        case 0b11:
            color = 0x00;
            break;
        }

        unsigned off = (LY * pitch) + (x * 3);
        pixels[off + 0] = color;
        pixels[off + 1] = color;
        pixels[off + 2] = color;
    }

    SDL_UnlockTexture(sdlTexture);
}

void drawSprites(uint8_t * pixels, int pitch)
{
    sprite_t s;
    for (int sprite = 0; sprite < 40; ++sprite) {
        uint8_t index = sprite * 4;
        s.Y = readWord(0xFE00 + index) - 16;
        s.X = readWord(0xFE00 + index + 1) - 8;
        s.Pattern = readWord(0xFE00 + index + 2);
        s.Attrib = readWord(0xFE00 + index + 3);

        int height = 8;
        if (LCDC.SpriteSize == 1) {
            height = 16;
        }

        if (LY >= s.Y && LY < s.Y + height) {
            int line = LY - s.Y;
            if (s.YFlip) {
                line -= height;
                line *= -1;
            }
            line *= 2;
            
            uint16_t tileAddress = 0x8000 + (s.Pattern * 16);
            uint8_t data1 = readByte(tileAddress + line);
            uint8_t data2 = readByte(tileAddress + line + 1);

            for (int pixel = 7; pixel >= 0; --pixel) {
                int bit = pixel;
                if (s.XFlip) {
                    bit -= 7;
                    bit *= -1;
                }

                uint8_t colorIndex = ((data2 & (1 << bit)) ? 2 : 0) | 
                                    ((data1 & (1 << bit)) ? 1 : 0);
                
                palette_t * pal = (s.Palette == 0 ? &OBP0 : &OBP1);

                uint8_t color = 0;
                switch (colorIndex) {
                case 0b00:
                    colorIndex = pal->Color00;
                    break;
                case 0b01:
                    colorIndex = pal->Color01;
                    break;
                case 0b10:
                    colorIndex = pal->Color10;
                    break;
                case 0b11:
                    colorIndex = pal->Color11;
                    break;
                }

                switch (colorIndex) {
                case 0b00:
                    color = 0xFF;
                    break;
                case 0b01:
                    color = 0xCC;
                    break;
                case 0b10:
                    color = 0x77;
                    break;
                case 0b11:
                    color = 0x00;
                    break;
                }

                int x = -pixel + 7;

                unsigned off = (LY * pitch) + (x * 3);
                pixels[off + 0] = 0x00;
                pixels[off + 1] = 0x00;
                pixels[off + 2] = 0x00;
            }
        }
    }
}

void drawScanline()
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTexture, NULL, (void **)&pixels, &pitch);

    if (LCDC.TileDisplay) {
        drawTiles(pixels, pitch);
    }

    if (LCDC.SpriteDisplay) {
        drawSprites(pixels, pitch);
    }

    SDL_UnlockTexture(sdlTexture);

    SDL_Rect src = { .x = 0, .y = 0, .w = 160, .h = 144 };
    SDL_Rect dst = { .x = 0, .y = 0, .w = 160 * SCALE, .h = 144 * SCALE, };
    
    SDL_RenderCopy(sdlRenderer, sdlTexture, &src, &dst);
    SDL_RenderPresent(sdlRenderer);
}

void lcdInit()
{
    if (SDL_CreateWindowAndRenderer(160 * SCALE, 144 * SCALE, 0, &sdlWindow, &sdlRenderer)) {
        LogFatal("failed to create SDL2 sdlWindow/sdlRenderer, %s", SDL_GetError());
    }
    
    SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderPresent(sdlRenderer);

    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 256, 256);

    int w, h;
    SDL_QueryTexture(sdlTexture, NULL, NULL, &w, &h);
    
    uint8_t * pixels = NULL;
    int pitch = 0;
    
    SDL_LockTexture(sdlTexture, NULL, (void **)&pixels, &pitch);
    for (unsigned y = 0; y < 144; ++y) {
        for (unsigned x = 0; x < 160; ++x) {
            unsigned off = (y * pitch) + (x * 3);
            pixels[off + 0] = 0xFF;
            pixels[off + 1] = 0xFF;
            pixels[off + 2] = 0xFF;
        }
    }
    SDL_UnlockTexture(sdlTexture);
    
    SDL_Rect src = { .x = 0, .y = 0, .w = 160, .h = 144 };
    SDL_Rect dst = { .x = 0, .y = 0, .w = 160 * SCALE, .h = 144 * SCALE };
    
    SDL_RenderCopy(sdlRenderer, sdlTexture, &src, &dst);
    SDL_RenderPresent(sdlRenderer);
}

void lcdTerm()
{
    SDL_DestroyTexture(sdlTexture);

    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
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

    uint8_t oldMode = STAT.Mode;
    bool requestInterrupt = false;

    if (LY >= 144) {
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
        scanlineCounter = 0;

        ++LY;
        if (LY > 153) {
            LY = 0;
        }

        if (LY == 144) {
            STAT.Mode = MODE_VBLANK;
            // Interrupt 0
        }
        else if (LY < 144) {
            drawScanline();
        }
    }

    //SDL_RenderClear(sdlRenderer);
    //SDL_RenderPresent(sdlRenderer);
}