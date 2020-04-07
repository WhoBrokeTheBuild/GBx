#ifndef SM83_TYPES_H
#define SM83_TYPES_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    SM83_MODE_DMG,
    SM83_MODE_SGB,
    SM83_MODE_CGB,

} sm83_mode_t;

typedef void (*sm83_tick_func_t)(void *, unsigned);
typedef uint8_t (*sm83_read_byte_func_t)(void *, uint16_t);
typedef void (*sm83_write_byte_func_t)(void *, uint16_t, uint8_t);

typedef union
{
    // clang-format off

    SM83_PACK(struct
    {
        bool    Int40:1;
        bool    Int48:1;
        bool    Int50:1;
        bool    Int58:1;
        bool    Int60:1;
        uint8_t :3;
    });

    // clang-format on

    uint8_t raw;

} sm83_interrupt_flags_t;

static_assert(sizeof(sm83_interrupt_flags_t) == 1,
    "sizeof(sm83_interrupt_flags_t) != 1");

#endif // SM83_TYPES_H