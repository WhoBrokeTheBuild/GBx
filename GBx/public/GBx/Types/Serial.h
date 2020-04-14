#ifndef GBX_TYPES_SERIAL_H
#define GBX_TYPES_SERIAL_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <GBx/Macros.h>

typedef union gbx_serial_control
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t ShiftClock:1;
        bool    FastClockEnable:1; // CGB
        uint8_t :5;
        bool    TransferStart:1;
    });

    // clang-format on

    uint8_t raw;

} gbx_serial_control_t;

#define GBX_SHIFT_CLOCK_EXTERNAL (0)
#define GBX_SHIFT_CLOCK_INTERNAL (1)

typedef struct gbx gbx_t;

void GBx_InitSerial(gbx_t * ctx);

#endif // GBX_TYPES_SERIAL_H