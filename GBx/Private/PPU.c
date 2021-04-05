#include <GBx/PPU.h>
#include <GBx/Context.h>
#include <GBx/Log.h>

#include <string.h>

void GBx_PPU_Reset(GBx * ctx)
{
    ctx->VRAMBank = 0;
    for (unsigned i = 0; i < GBX_VRAM_BANK_COUNT; ++i) {
        memset(ctx->VRAM[i], 0, GBX_VRAM_BANK_SIZE);
    }

    memset(ctx->OAM, 0, sizeof(ctx->OAM));

    memset(ctx->PixelData, 32, sizeof(ctx->PixelData));

    ctx->SCY = 0x00;
    ctx->SCX = 0x00;
    ctx->LY = 0x00;
    ctx->LYC = 0x00;
    ctx->WY = 0x00;
    ctx->WX = 0x00;

    ctx->BGP._raw = 0b11100100;
    ctx->OBP0._raw = 0b11100100;
    ctx->OBP1._raw = 0b11100100;
    
    ctx->LCDC._raw = 0x91;
    ctx->STAT._raw = 0x00;
}

void GBx_PPU_Tick(GBx * ctx, unsigned cycles)
{
    for (unsigned i = 0; i < cycles; ++i) {
        ++ctx->PPUModeCycles;

        switch (ctx->STAT.Mode) {
        case GBX_STAT_MODE_SEARCH:
            if (ctx->PPUModeCycles == 20) {
                ctx->PPUModeCycles = 0;

                // TODO:
                ctx->STAT.Mode = GBX_STAT_MODE_DRAW;
            }
            break;
        case GBX_STAT_MODE_DRAW:
            if (ctx->PPUModeCycles == 43) {
                ctx->PPUModeCycles = 0;

                // TODO:
                ctx->STAT.Mode = GBX_STAT_MODE_HBLANK;
            }
            break;
        case GBX_STAT_MODE_HBLANK:
            if (ctx->PPUModeCycles == 51) {
                ctx->PPUModeCycles = 0;
                
                // TODO:

                ++ctx->LY;

                ctx->STAT.Coincidence = (ctx->LY == ctx->LYC);

                if (ctx->STAT.IntCoincidence && ctx->STAT.Coincidence) {
                    ctx->IF.Int48 = 1;
                }

                if (ctx->LY == GBX_SCREEN_HEIGHT) {
                    ctx->STAT.Mode = GBX_STAT_MODE_VBLANK;

                    if (ctx->STAT.IntVBlank) {
                        ctx->IF.Int48 = 1;
                    }

                }
                else {
                    ctx->STAT.Mode = GBX_STAT_MODE_SEARCH;
                    
                    if (ctx->STAT.IntSearchSprite) {
                        ctx->IF.Int48 = 1;
                    }
                }
            }
            break;
        case GBX_STAT_MODE_VBLANK:
            // One Line
            if (ctx->PPUModeCycles == 114) {
                ctx->PPUModeCycles = 0;
                
                // TODO:

                ctx->LY = (ctx->LY + 1) % (GBX_SCREEN_HEIGHT + 10);

                ctx->STAT.Coincidence = (ctx->LY == ctx->LYC);

                if (ctx->STAT.IntCoincidence && ctx->STAT.Coincidence) {
                    ctx->IF.Int48 = 1;
                }

                if (ctx->LY == 0) {
                    ctx->STAT.Mode = GBX_STAT_MODE_SEARCH;
                    
                    if (ctx->STAT.IntSearchSprite) {
                        ctx->IF.Int48 = 1;
                    }
                }
            }
            break;
        }
    }
}

uint8_t * GBx_PPU_GetPixelData(GBx * ctx)
{
    return ctx->PixelData;
}

// void GBx_PPU_OAM_DMA(GBx * ctx, uint8_t data)
// {
//     uint16_t address = data << 8;
//     for (uint8_t i = 0; i < GBX_OAM_SIZE; ++i) {
//         ctx->OAM[i] = GBx_ReadByte()
//     }
// }
