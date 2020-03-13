#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

#define GB_CLOCK_SPEED  (4194304) // Hz
#define SGB_CLOCK_SPEED (4295454) // Hz
#define CGB_CLOCK_SPEED (8388608) // Hz

extern uint64_t TotalTicks;

extern unsigned ClockSpeed;

void Tick(unsigned ticks);

#endif // CLOCK_H