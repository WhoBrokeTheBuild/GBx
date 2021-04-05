#ifndef GBX_PPU_H
#define GBX_PPU_H

#include <GBx/GBx.h>
// #include <GBx/Types/PPU.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union GBx_PaletteMonochrome
{
    GBX_PACK(struct {
        uint8_t Color0:2;
        uint8_t Color1:2;
        uint8_t Color2:2;
        uint8_t Color3:2;
    });
    
    uint8_t _raw;

} GBx_PaletteMonochrome;

static_assert(
    sizeof(GBx_PaletteMonochrome) == 1,
    "sizeof(GBx_PaletteMonochrome) != 1"
);

typedef union GBx_PaletteIndex
{
    GBX_PACK(struct {
        uint8_t Index:6;

        uint8_t :1;

        // 0 = Disabled, 1 = Increment after write
        uint8_t AutoIncrement:1;
    });

} GBx_PaletteIndex;

typedef union GBx_PaletteColor
{
    GBX_PACK(struct {
        uint16_t Red:5;
        uint16_t Green:5;
        uint16_t Blue:5;
        uint16_t :1;
    });

    uint8_t _raw[2];

} GBx_PaletteColor;

typedef union GBx_SpriteAttribute
{
    GBX_PACK(struct {
        uint8_t Priority:1;
        uint8_t FlipY:1;
        uint8_t FlipX:1;
        uint8_t Palette:1;
        uint8_t :4;
    });

    uint8_t _raw;

} GBx_SpriteAttribute;

static_assert(
    sizeof(GBx_SpriteAttribute) == 1,
    "sizeof(GBx_SpriteAttribute) != 1"
);

typedef union GBx_LCDControl
{
    // CGB
    GBX_PACK(struct {
        // 0 = Background/Window tiles rendered behind sprites
        // 1 = Background/Window tiles rendered normally
        uint8_t TilePriority:1;

        uint8_t :7;
    });

    // DMG
    GBX_PACK(struct {
        // Background/Window Tiles
        // 0 = Disabled, 1 = Enabled
        uint8_t TileDisplayEnabled:1;

        uint8_t :7;
    });

    GBX_PACK(struct {
        // See above
        uint8_t :1;

        // 0 = Disabled, 1 = Enabled
        uint8_t SpriteDisplayEnabled:1;

        // 0 = 8x8, 1 = 16x16
        uint8_t SpriteSize:1;

        // 0 = $9800, 1 = $9C00
        uint8_t BackgroundTileMapSelect:1;

        // 0 = $8000, 1 = $8800
        uint8_t TileDataSelect:1;

        // 0 = Disabled, 1 = Enabled
        uint8_t WindowDisplayEnabled:1;

        // 0 = $9800, 1 = $9C00
        uint8_t WindowTileMapSelect:1;

        // 0 = Disabled, 1 = Enabled
        uint8_t Enabled:1;
    });

    uint8_t _raw;

} GBx_LCDControl;

static_assert(
    sizeof(GBx_LCDControl) == 1,
    "sizeof(GBx_LCDControl) != 1"
);

typedef union GBx_LCDStatus
{
    GBX_PACK(struct {
        uint8_t Mode:2;
        uint8_t Coincidence:1;
        uint8_t IntHBlank:1;
        uint8_t IntVBlank:1;
        uint8_t IntSearchSprite:1;
        uint8_t IntCoincidence:1;
        uint8_t :1;
    });

    uint8_t _raw;

} GBx_LCDStatus;

static_assert(
    sizeof(GBx_LCDStatus) == 1,
    "sizeof(GBx_LCDStatus) != 1"
);

#define GBX_STAT_WRITE_MASK     0b01111000

void GBx_PPU_Reset(GBx * ctx);

void GBx_PPU_Tick(GBx * ctx, unsigned cycles);

uint8_t * GBx_PPU_GetPixelData(GBx * ctx);

// void GBx_PPU_OAM_DMA(GBx * ctx, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif // GBX_PPU_H