#ifndef GBX_TYPES_JOYPAD_H
#define GBX_TYPES_JOYPAD_H

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
        bool    Right:1;
        bool    Left:1;
        bool    Up:1;
        bool    Down:1;
        uint8_t :4;
    });

    GBX_PACK(struct
    {
        bool    A:1;
        bool    B:1;
        bool    Select:1;
        bool    Start:1;
        uint8_t :4;
    });

    GBX_PACK(struct
    {
        uint8_t InputStates:4;
        bool    SelectDPad:1; 
        bool    SelectButtons:1;
        uint8_t :2;
    });

    // clang-format on

    uint8_t raw;

} gbx_joypad_state_t;

static_assert(sizeof(gbx_joypad_state_t) == 1,
    "sizeof(gbx_joypad_state_t) != 1");

#define GBX_JOYP_WRITE_MASK (0b00110000)

#endif // GBX_TYPES_JOYPAD_H