#ifndef SERIAL_H
#define SERIAL_H

#include <GBx/Types.h>

typedef union
{
    struct {
        unsigned ShiftClock:1;
        bool     FastClockEnable:1; // CGB
        unsigned :5;
        bool     TransferStart:1;
    };
    byte raw;

} serial_control_t;

#define SHIFT_CLOCK_EXTERNAL (0)
#define SHIFT_CLOCK_INTERNAL (1)

// FF01 - Serial Data

extern byte SB;

// FF02 - Serial Control

extern serial_control_t SC;

#endif // SERIAL_H