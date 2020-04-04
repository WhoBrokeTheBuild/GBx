#ifndef SERIAL_H
#define SERIAL_H

#include <GBx/Macros.h>
#include <GBx/Types.h>

typedef union
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

} serial_control_t;

#define SHIFT_CLOCK_EXTERNAL (0)
#define SHIFT_CLOCK_INTERNAL (1)

// FF01 - Serial Data

extern uint8_t SB;

// FF02 - Serial Control

extern serial_control_t SC;

#endif // SERIAL_H