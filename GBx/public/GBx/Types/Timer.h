#ifndef GBX_TYPES_TIMER_H
#define GBX_TYPES_TIMER_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <GBx/Macros.h>

typedef union gbx_timer_control
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t Type:2;
        bool Enabled:1;
        uint8_t :5;
    });

    // clang-format on

    uint8_t raw;

} gbx_timer_control_t;

static_assert(sizeof(gbx_timer_control_t) == 1,
    "sizeof(gbx_timer_control_t) != 1");

#endif // GBX_TYPES_TIMER_H