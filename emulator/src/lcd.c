#include "lcd.h"

#include "cartridge.h"
#include "cpu.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"

#include <SDL.h>

LCDC_t LCDC;

STAT_t STAT;

uint8_t SCY = 0x00;
uint8_t SCX = 0x00;
uint8_t LY  = 0x00;
uint8_t LYC = 0x00;
uint8_t WX = 0x00;
uint8_t WY = 0x00;

palette_t BGP = { .data = 0b11100100 };
palette_t OBP0 = { .data = 0b11100100 };
palette_t OBP1 = { .data = 0b11100100 };

uint8_t VideoRAM0[0x1FFF];
uint8_t VideoRAM1[0x1FFF];
uint8_t OAM[0xA0];

unsigned int LCDModeTicks = 0;

const char * LCDModeStr[4] = {
    "HBlank",
    "VBlank",
    "SearchSprite",
    "DataTransfer",
};

SDL_Window * sdlWindow = NULL;
SDL_Renderer * sdlRenderer = NULL;
SDL_Texture * sdlTexture = NULL;

bool FPSLimit = true;

uint8_t PixelData[256*256*3];

const int SCALE = 4;

void drawTiles()
{
    int y = 0;
    if (LCDC.WindowDisplayEnable) {
        y = LY - WY;
    } else {
        y = SCY + LY;
    }

    uint16_t row = (y / 8) * 32;

    for (unsigned pixel = 0; pixel < 160; ++pixel) {
        uint8_t x = pixel + SCX;
        if (LCDC.WindowDisplayEnable) {
            if (pixel >= WX - 7) {
                x = pixel - (WX - 7);
            }
        }


        uint16_t tileBaseAddress;
        if (LCDC.WindowDisplayEnable) {
            if (LCDC.WindowTileMapSelect) {
                tileBaseAddress = 0x9C00;
            } else {
                tileBaseAddress = 0x9800;
            }
        } else {
            if (LCDC.TileMapSelect) {
                tileBaseAddress = 0x9C00;
            } else {
                tileBaseAddress = 0x9800;
            }
        }

        uint16_t col = x / 8;

        uint16_t tileOffset;
        if (LCDC.TileDataSelect) {
            uint8_t tileIndex = readByte(tileBaseAddress + row + col);
            tileOffset = 0x8000 + (tileIndex * 16);
        } else {
            int8_t tileIndex = (int8_t)readByte(tileBaseAddress + row + col);
            tileOffset = 0x8800 + ((tileIndex + 128) * 16);
        }

        uint8_t line = (y % 8) * 2;
        uint8_t data1 = readByte(tileOffset + line);
        uint8_t data2 = readByte(tileOffset + line + 1);

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

        unsigned off = (LY * 256*3) + (x * 3);
        PixelData[off + 0] = color;
        PixelData[off + 1] = color;
        PixelData[off + 2] = color;
    }
}

void drawSprites()
{
    sprite_t s;
    for (int sprite = 0; sprite < 40; ++sprite) {
        uint8_t index = sprite * 4;
        s.Y = readWord(0xFE00 + index) - 16;
        s.X = readWord(0xFE00 + index + 1) - 8;
        s.Pattern = readWord(0xFE00 + index + 2);
        s.Attributes = readWord(0xFE00 + index + 3);

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
                
                palette_t * pal = (s.Palette ? &OBP1 : &OBP0);

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

                unsigned off = (LY * 256*3) + (x * 3);
                PixelData[off + 0] = 0x00;
                PixelData[off + 1] = 0x00;
                PixelData[off + 2] = 0x00;
            }
        }
    }
}

void pollEvents() 
{
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            exit(0);
            break;
        }
    }
}

void render() 
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlTexture, NULL, (void **)&pixels, &pitch);
    memcpy(pixels, PixelData, sizeof(PixelData));
    SDL_UnlockTexture(sdlTexture);

    SDL_Rect src = { .x = 0, .y = 0, .w = 160, .h = 144 };
    SDL_Rect dst = { .x = 0, .y = 0, .w = 160 * SCALE, .h = 144 * SCALE, };
    
    SDL_RenderCopy(sdlRenderer, sdlTexture, &src, &dst);
    SDL_RenderPresent(sdlRenderer);

    pollEvents();

    if (FPSLimit) {
        SDL_Delay((1.0 / 59.9) * 1000.0);
    }
}

void lcdTick(unsigned cycles)
{
    const unsigned HBLANK_TICK_COUNT = 204;
    const unsigned VBLANK_TICK_COUNT = 456;
    const unsigned SEARCH_SPRITE_TICK_COUNT = 80;
    const unsigned DATA_TRANSFER_TICK_COUNT = 172;

    LCDModeTicks += cycles;

    bool modeChanged = false;

    switch (STAT.Mode) {
        case STAT_MODE_HBLANK:
            if (LCDModeTicks >= 204) {
                LCDModeTicks = 0;

                ++LY;
                if (LY == 144) {
                    STAT.Mode = STAT_MODE_VBLANK;
                    render();
                }
                else {
                    STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                    modeChanged = true;
                }
            }
        break;
        case STAT_MODE_VBLANK:
            if (LCDModeTicks >= 456) {
                LCDModeTicks = 0;
                
                ++LY;
                if (LY > 153) {
                    STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                    modeChanged = true;
                    LY = 0;
                }
            }
        break;
        case STAT_MODE_SEARCH_SPRITE:
            if (LCDModeTicks >= 80) {
                LCDModeTicks = 0;
                STAT.Mode = STAT_MODE_DATA_TRANSFER;
                modeChanged = true;
            }
        break;
        case STAT_MODE_DATA_TRANSFER:
            if (LCDModeTicks >= 172) {
                LCDModeTicks = 0;
                STAT.Mode = STAT_MODE_HBLANK;
                modeChanged = true;

                if (LCDC.TileDisplayEnable) {
                    drawTiles();
                }

                if (LCDC.SpriteDisplayEnable) {
                    drawSprites();
                }
            }
        break;
    }

    if (LY == 144) {
        IF.VBlank = true;
    }

    if ((STAT.IntCoincidence && STAT.LYCLY && LY == LYC) ||
        (STAT.IntCoincidence && !STAT.LYCLY && LY != LYC) ||
        (STAT.IntHBlank && modeChanged && STAT.Mode == STAT_MODE_HBLANK) ||
        (STAT.IntVBlank && modeChanged && STAT.Mode == STAT_MODE_VBLANK) ||
        (STAT.IntSearchSprite && modeChanged && STAT.Mode == STAT_MODE_SEARCH_SPRITE)) {
        IF.STAT = true;
    }

    if (modeChanged) {
        LogVerbose(3, "LCD Mode Changed: %s", LCDModeStr[STAT.Mode]);
    }
}

void lcdInit()
{

    char windowTitle[21];
    snprintf(windowTitle, sizeof(windowTitle), "GBx - %s", CartridgeTitle);

    sdlWindow = SDL_CreateWindow(windowTitle, -1, -1, 160 * SCALE, 144 * SCALE, 0);
    if (!sdlWindow) {
        LogFatal("failed to create SDL2 window, %s", SDL_GetError());
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, 0, SDL_RENDERER_ACCELERATED);
    if (!sdlRenderer) {
        LogFatal("failed to create SDL2 renderer, %s", SDL_GetError());
    }
    
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 256, 256);

    for (unsigned y = 0; y < 144; ++y) {
        for (unsigned x = 0; x < 160; ++x) {
            unsigned off = (y * 256*3) + (x * 3);
            PixelData[off + 0] = 0xFF;
            PixelData[off + 1] = 0xFF;
            PixelData[off + 2] = 0xFF;
        }
    }

    render();
}

void lcdTerm()
{
    SDL_DestroyTexture(sdlTexture);

    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
}

void printLCDC()
{
    LogInfo("BGWinDisp=%d OBJDisp=%d OBJSize=%s BGTileMap=%s TileData=%s WinDisp=%d WinTileMap=%s LCDEnab=%d",
        LCDC.TileDisplayEnable, LCDC.SpriteDisplayEnable, 
        (LCDC.SpriteSize ? "8x16" : "8x8"),
        (LCDC.TileMapSelect ? "9C00h-9FFFh" : "9800h-9BFFh"),
        (LCDC.TileDataSelect ? "8000h-8FFFh" : "8800h-97FFh"),
        LCDC.WindowDisplayEnable,
        (LCDC.WindowTileMapSelect ? "9800h-9BFFh" : "9C00h-9FFFh"),
        LCDC.LCDEnable);
}

void printSTAT() 
{

    LogInfo("Mode=%s IntCoinc=%d IntHBlank=%d IntVBlank=%d IntSearchSprite=%d LYCLY=%d",
        LCDModeStr[STAT.Mode], STAT.IntCoincidence, STAT.IntHBlank, STAT.IntVBlank, STAT.IntSearchSprite, STAT.LYCLY);
}

void printLCDInfo()
{
    LogInfo("SCY=%d SCX=%d LY=%d LYC=%d WX=%d WY=%d",
        SCY, SCX, LY, LYC, WX, WY);
}