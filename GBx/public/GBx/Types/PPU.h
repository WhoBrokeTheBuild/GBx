#ifndef GBX_TYPES_PPU_H
#define GBX_TYPES_PPU_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <GBx/Macros.h>

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        bool TileDisplayEnabled:1; // TODO: Research
        bool SpriteDisplayEnabled:1;
        uint8_t SpriteSize:1;
        uint8_t BGTileMapSelect:1;
        uint8_t TileDataSelect:1;
        bool WindowDisplayEnabled:1;
        uint8_t WindowTileMapSelect:1;
        bool Enabled:1;
    });

    // clang-format on

    uint8_t raw;

} gbx_lcd_control_t;

static_assert(sizeof(gbx_lcd_control_t) == 1,
    "sizeof(gbx_lcd_control_t) != 1");

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t Mode:2;
        bool Coincidence:1;
        bool IntHBlank:1;
        bool IntVBlank:1;
        bool IntSearchSprite:1;
        bool IntCoincidence:1;
        uint8_t :1;
    });

    // clang-format on

    uint8_t raw;

} gbx_lcd_status_t;

static_assert(sizeof(gbx_lcd_status_t) == 1,
    "sizeof(gbx_lcd_status_t) != 1");

#define GBX_STAT_WRITE_MASK (0b01111000)

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t Color0:2;
        uint8_t Color1:2;
        uint8_t Color2:2;
        uint8_t Color3:2;
    });

    // clang-format on

    uint8_t raw;

} gbx_palette_t;

static_assert(sizeof(gbx_palette_t) == 1,
    "sizeof(gbx_palette_t) != 1");

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        bool Priority:1;
        bool YFlip:1;
        bool XFlip:1;
        bool Palette:1;
        uint8_t :4;
    });

    // clang-format on

    uint8_t raw;

} gbx_sprite_attrib_t;

static_assert(sizeof(gbx_sprite_attrib_t) == 1,
    "sizeof(gbx_sprite_attrib_t) != 1");

#endif // GBX_TYPES_PPU_H