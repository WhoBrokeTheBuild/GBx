#ifndef GBX_TYPES_H
#define GBX_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <GBx/Macros.h>

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        bool TileDisplayEnabled:1;
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

} lcd_control_t;

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

} lcd_status_t;

#define STAT_WRITE_MASK (0b01111000)

#define STAT_MODE_HBLANK        (0)
#define STAT_MODE_VBLANK        (1)
#define STAT_MODE_SEARCH_SPRITE (2)
#define STAT_MODE_DATA_TRANSFER (3)

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

} palette_t;

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

} sprite_attrib_t;

#endif // GBX_TYPES_H