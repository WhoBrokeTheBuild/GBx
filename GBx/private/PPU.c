#include <GBx/PPU.h>

#include <GBx/Context.h>
#include <GBx/Macros.h>
#include <GBx/MMU.h>

#include <string.h>
#include <time.h>

#include "Internal.h"
#include "Log.h"

void GBx_ResetPPU(gbx_t * ctx)
{
    ctx->VRAMBank = 0;
    for (unsigned i = 0; i < GBX_VRAM_BANK_COUNT; ++i) {
        memset(ctx->VRAM[i], 0, sizeof(ctx->VRAM[i]));
    }

    memset(ctx->OAM, 0, sizeof(ctx->OAM));

    memset(ctx->Backbuffer, 0, sizeof(ctx->Backbuffer));

    ctx->SCY = 0x00;
    ctx->SCX = 0x00;
    ctx->LY = 0x00;
    ctx->LYC = 0x00;
    ctx->WY = 0x00;
    ctx->WX = 0x00;

    ctx->BGP.raw = 0b11100100;
    ctx->OBP0.raw = 0b11100100;
    ctx->OBP1.raw = 0b11100100;
    
    ctx->LCDC.raw = 0x91;
    ctx->STAT.raw = 0x00;
}

struct timespec timediff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

void GBx_TickPPU(gbx_t * ctx, unsigned cycles)
{
    const int MAX_LY = (GBX_LCD_HEIGHT + 10);

    const int STAT_MODE_HBLANK        = 0;
    const int STAT_MODE_VBLANK        = 1;
    const int STAT_MODE_SEARCH_SPRITE = 2;
    const int STAT_MODE_DATA_TRANSFER = 3;

    const int HBLANK_TICKS = 204;
    const int SEARCH_SPRITE_TICKS = 80;
    const int DATA_TRANSFER_TICKS = 172;

    // 1/10th of VBlank period, or one line worth of ticks
    const int VBLANK_TICKS
        = (HBLANK_TICKS + SEARCH_SPRITE_TICKS + DATA_TRANSFER_TICKS);

    gbx_internal_t * inctx = ctx->internal;

    inctx->PPUModeTicks += cycles;

    switch (ctx->STAT.Mode) {
    case STAT_MODE_SEARCH_SPRITE:
        if (inctx->PPUModeTicks >= SEARCH_SPRITE_TICKS) {
            inctx->PPUModeTicks -= SEARCH_SPRITE_TICKS;

            ctx->STAT.Mode = STAT_MODE_DATA_TRANSFER;
        }
        break;
    case STAT_MODE_DATA_TRANSFER:
        if (inctx->PPUModeTicks >= DATA_TRANSFER_TICKS) {
            inctx->PPUModeTicks -= DATA_TRANSFER_TICKS;

            if (ctx->LCDC.TileDisplayEnabled) {
                GBx_DrawTiles(ctx);
            }

            if (ctx->LCDC.SpriteDisplayEnabled) {
                GBx_DrawSprites(ctx);
            }

            ctx->STAT.Mode = STAT_MODE_HBLANK;
        }
        break;
    case STAT_MODE_HBLANK:
        if (inctx->PPUModeTicks >= HBLANK_TICKS) {
            inctx->PPUModeTicks -= HBLANK_TICKS;

            ++ctx->LY;
            ctx->STAT.Coincidence = (ctx->LY == ctx->LYC);
            if (ctx->STAT.IntCoincidence && ctx->STAT.Coincidence) {
                ctx->CPU->IF.Int48 = true;
            }

            if (ctx->LY == GBX_LCD_HEIGHT) {
                ctx->STAT.Mode = STAT_MODE_VBLANK;

                ctx->CPU->IF.Int40 = true;
                if (ctx->STAT.IntVBlank) {
                    ctx->CPU->IF.Int48 = true;
                }

                static struct timespec last;
                // static double fpsTotal = 0.0;
                // static long fpsCount = 0;

                // if (last.tv_nsec == 0) {
                //     clock_gettime(CLOCK_MONOTONIC, &last);
                // }
                // else {
                struct timespec now;

                clock_gettime(CLOCK_MONOTONIC, &now);
                struct timespec delta = timediff(last, now);
                last = now;

                //     float fps = 1.0 / (delta.tv_nsec / 1000000000.0);
                //     // LogInfo("VBL %ld %f", delta.tv_nsec, fps);

                //     fpsTotal += fps;
                //     ++fpsCount;

                //     if (fpsCount == 386) {
                //         LogInfo("Average FPS %f", fpsTotal / fpsCount);
                //     }
                // }

                struct timespec wait;
                wait.tv_sec = 0;
                wait.tv_nsec = 16750419 - delta.tv_nsec;
                clock_nanosleep(CLOCK_MONOTONIC, 0, &wait, NULL);
            }
            else {
                ctx->STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                if (ctx->STAT.IntSearchSprite) {
                    ctx->CPU->IF.Int48 = true;
                }
            }
        }
        break;
    case STAT_MODE_VBLANK:
        if (ctx->internal->PPUModeTicks >= VBLANK_TICKS) {
            ctx->internal->PPUModeTicks -= VBLANK_TICKS;

            ctx->LY = (ctx->LY + 1) % MAX_LY;
            ctx->STAT.Coincidence = (ctx->LY == ctx->LYC);
            if (ctx->STAT.IntCoincidence && ctx->STAT.Coincidence) {
                ctx->CPU->IF.Int48 = true;
            }

            if (ctx->LY == 0) {
                ctx->STAT.Mode = STAT_MODE_SEARCH_SPRITE;
                if (ctx->STAT.IntSearchSprite) {
                    ctx->CPU->IF.Int48 = true;
                }
            }
        }
        break;
    }
}

void GBx_DrawTiles(gbx_t * ctx)
{
    const int TILE_WIDTH = 8;
    const int TILE_HEIGHT = 8;
    const int TILE_DATA_SIZE = 16;
    const int TILES_PER_ROW = 32;

    bool usingWindow = (ctx->LCDC.WindowDisplayEnabled && ctx->WY <= ctx->LY);

    int mapSelect
        = (usingWindow ? ctx->LCDC.WindowTileMapSelect : ctx->LCDC.BGTileMapSelect);

    uint16_t mapBaseAddress = GBx_GetTileMapAddress(mapSelect);
    uint16_t dataBaseAddress = GBx_GetTileDataAddress(ctx->LCDC.TileDataSelect);

    uint8_t WXM7 = ctx->WX - 7;

    uint8_t yPos = 0;
    if (usingWindow) {
        yPos = ctx->LY - ctx->WY;
    }
    else {
        yPos = ctx->SCY + ctx->LY;
    }

    int tileRow = (uint8_t)(yPos / TILE_HEIGHT) * TILES_PER_ROW;

    for (int pixel = 0; pixel < GBX_LCD_WIDTH; ++pixel) {
        uint8_t xPos = pixel + ctx->SCX;
        if (usingWindow && pixel >= WXM7) {
            xPos = pixel - WXM7;
        }

        int tileCol = xPos / TILE_WIDTH;

        int tileIndex = GBx_ReadByte(ctx, mapBaseAddress + tileRow + tileCol);
        if (ctx->LCDC.TileDataSelect == 0) {
            tileIndex = (int8_t)tileIndex; // Convert to [-128, 127]
        }

        uint16_t dataOffset = dataBaseAddress + (tileIndex * TILE_DATA_SIZE);

        uint8_t line = (yPos % TILE_HEIGHT) * 2;
        uint8_t data1 = GBx_ReadByte(ctx, dataOffset + line);
        uint8_t data2 = GBx_ReadByte(ctx, dataOffset + line + 1);

        int bit = (xPos % TILE_WIDTH);
        const uint8_t * color = GBx_GetPixelColor(ctx, &ctx->BGP, bit, data1, data2);

        unsigned off = (ctx->LY * GBX_BACKBUFFER_WIDTH * GBX_BACKBUFFER_COMPONENTS)
                       + (pixel * GBX_BACKBUFFER_COMPONENTS);

        ctx->Backbuffer[off + 0] = color[0];
        ctx->Backbuffer[off + 1] = color[1];
        ctx->Backbuffer[off + 2] = color[2];
    }
}

void GBx_DrawSprites(gbx_t * ctx)
{
    const int OAM_ADDRESS = 0xFE00;
    const int OAM_ENTRY_DATA_SIZE = 4;
    const int SPRITE_Y_OFFSET = 16;
    const int SPRITE_X_OFFSET = 8;

    gbx_sprite_attrib_t attrib;
    for (unsigned sprite = 0; sprite < 40; ++sprite) {
        unsigned offset = sprite * OAM_ENTRY_DATA_SIZE;

        int y = GBx_ReadByte(ctx, OAM_ADDRESS + offset) - SPRITE_Y_OFFSET;

        ++offset;
        int x = GBx_ReadByte(ctx, OAM_ADDRESS + offset) - SPRITE_X_OFFSET;

        ++offset;
        int pattern = GBx_ReadByte(ctx, OAM_ADDRESS + offset);

        ++offset;
        attrib.raw = GBx_ReadByte(ctx, OAM_ADDRESS + offset);

        int height = 8;
        if (ctx->LCDC.SpriteSize == 1) {
            height = 16;
        }

        if (ctx->LY >= y && ctx->LY < y + height) {
            int line = ctx->LY - y;
            if (attrib.YFlip) {
                line -= height;
                line *= -1;
            }
            line *= 2;

            uint16_t tileAddress = 0x8000 + (pattern * 16);
            uint8_t data1 = GBx_ReadByte(ctx, tileAddress + line);
            uint8_t data2 = GBx_ReadByte(ctx, tileAddress + line + 1);

            for (int pixel = 7; pixel >= 0; --pixel) {
                int bit = (1 << pixel);
                if (attrib.XFlip) {
                    bit = (0x80 >> pixel);
                }

                gbx_palette_t * pal = (attrib.Palette ? &ctx->OBP1 : &ctx->OBP0);
                const uint8_t * color = GBx_GetPixelColor(ctx, pal, bit, data1, data2);

                int x = -pixel + 7;

                unsigned off = (ctx->LY * GBX_BACKBUFFER_WIDTH * GBX_BACKBUFFER_COMPONENTS)
                            + (x * GBX_BACKBUFFER_COMPONENTS);

                ctx->Backbuffer[off + 0] = color[0];
                ctx->Backbuffer[off + 1] = color[1];
                ctx->Backbuffer[off + 2] = color[2];
            }
        }
    }
}

void GBx_DMATransferOAM(gbx_t * ctx, uint8_t data)
{
    ctx->CPU->Enabled = false;

    uint16_t addr = data << 8;
    for (uint8_t i = 0; i < 0xA0; ++i) {
        ctx->OAM[i] = GBx_ReadByte(ctx, addr + i);
        SM83_Step(ctx->CPU);
    }

    ctx->CPU->Enabled = true;
}

const uint8_t * GBx_GetPixelColor(gbx_t * ctx,
    gbx_palette_t * pal,
    int bit,
    uint8_t data1,
    uint8_t data2)
{
    static const uint8_t COLORS_ORIG[][3] = {
        { 0x9B, 0xBC, 0x0F },
        { 0x8B, 0xAC, 0x0F },
        { 0x30, 0x62, 0x30 },
        { 0x0F, 0x38, 0x0F },
    };

    static const uint8_t COLORS_CLEAN[][3] = {
        { 0xFF, 0xFF, 0xFF },
        { 0xCC, 0xCC, 0xCC },
        { 0x77, 0x77, 0x77 },
        { 0x00, 0x00, 0x00 },
    };

    bit = 0x80 >> bit;
    bool high = (data2 & bit);
    bool low = (data1 & bit);
    uint8_t colorIndex = (high ? 0b10 : 0b00) | (low ? 0b01 : 0b00);

    switch (colorIndex) {
    case 0b00:
        colorIndex = pal->Color0;
        break;
    case 0b01:
        colorIndex = pal->Color1;
        break;
    case 0b10:
        colorIndex = pal->Color2;
        break;
    case 0b11:
        colorIndex = pal->Color3;
        break;
    }

    return (ctx->UseOriginalColors 
        ? COLORS_ORIG[colorIndex]
        : COLORS_CLEAN[colorIndex]);
}

uint16_t GBx_GetTileMapAddress(unsigned index)
{
    static const uint16_t ADDR[] = {
        0x9800,
        0x9C00,
    };

    return ADDR[index];
}

uint16_t GBx_GetTileDataAddress(unsigned index)
{
    static const uint16_t ADDR[] = {
        0x9000,
        0x8000,
    };

    return ADDR[index];
}

const char * GBx_GetPPUModeString(unsigned mode)
{
    static const char * STR[] = {
        "HBlank",
        "VBlank",
        "SearchSprite",
        "DataTransfer",
    };

    return STR[mode];
}

const char * GBx_GetTileMapRangeString(unsigned index)
{
    static const char * STR[] = {
        "$9800-$9BFF",
        "$9C00-$9FFF",
    };

    return STR[index];
}

const char * GBx_GetTileDataRangeString(unsigned index)
{
    static const char * STR[] = {
        "$8800-$97FF",
        "$8000-$8FFF",
    };

    return STR[index];
}

const char * GBx_GetSpriteSizeString(unsigned index)
{
    static const char * STR[] = {
        "8x8",
        "8x16",
    };

    return STR[index];
}

void GBx_PrintLCDC(gbx_t * ctx)
{
    LogInfo("LCDC: %s OBJ=%s OBJSize=%s BG=%s BGTileMap=%s TileData=%s Win=%s "
            "WinTileMap=%s",
        GBx_GetEnabledString(ctx->LCDC.Enabled),
        GBx_GetEnabledString(ctx->LCDC.SpriteDisplayEnabled),
        GBx_GetSpriteSizeString(ctx->LCDC.SpriteSize),
        GBx_GetEnabledString(ctx->LCDC.TileDisplayEnabled),
        GBx_GetTileMapRangeString(ctx->LCDC.BGTileMapSelect),
        GBx_GetTileDataRangeString(ctx->LCDC.TileDataSelect),
        GBx_GetEnabledString(ctx->LCDC.WindowDisplayEnabled),
        GBx_GetTileMapRangeString(ctx->LCDC.WindowTileMapSelect));
}

void GBx_PrintSTAT(gbx_t * ctx)
{
    LogInfo("STAT: Mode=%d (%s) Coincidence=%d IntHBlank=%d IntVBlank=%d "
            "IntSearchSprite=%d IntCoincidence=%d",
        ctx->STAT.Mode,
        GBx_GetPPUModeString(ctx->STAT.Mode),
        ctx->STAT.Coincidence,
        ctx->STAT.IntHBlank,
        ctx->STAT.IntVBlank,
        ctx->STAT.IntSearchSprite,
        ctx->STAT.IntCoincidence);
}

void GBx_PrintPalettes(gbx_t * ctx)
{
    LogInfo("BGP: Color0=%d Color1=%d Color2=%d Color3=%d",
        ctx->BGP.Color0,
        ctx->BGP.Color1,
        ctx->BGP.Color2,
        ctx->BGP.Color3);

    LogInfo("OBP0: Color0=%d Color1=%d Color2=%d Color3=%d",
        ctx->OBP0.Color0,
        ctx->OBP0.Color1,
        ctx->OBP0.Color2,
        ctx->OBP0.Color3);

    LogInfo("OBP1: Color0=%d Color1=%d Color2=%d Color3=%d",
        ctx->OBP1.Color0,
        ctx->OBP1.Color1,
        ctx->OBP1.Color2,
        ctx->OBP1.Color3);
}

void GBx_PrintLCDCoordinates(gbx_t * ctx)
{
    LogInfo("SCY=%d SCX=%d LY=%d LYC=%d WX=%d WY=%d", 
        ctx->SCY, ctx->SCX, ctx->LY, ctx->LYC, ctx->WX, ctx->WY);
}