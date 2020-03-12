#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

typedef union
{
    struct {
        unsigned ShiftClock:1;
        bool     FastClockEnable:1; // CGB
        unsigned :5;
        bool     TransferStart:1;
    };
    uint8_t raw;

} serial_control_t;

#define SHIFT_CLOCK_EXTERNAL (0)
#define SHIFT_CLOCK_INTERNAL (1)

// FF01 - Serial Data

extern uint8_t SB;

// FF02 - Serial Control

extern serial_control_t SC;

#endif // SERIAL_H