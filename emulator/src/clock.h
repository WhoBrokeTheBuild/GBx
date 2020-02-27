#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

#define GB_CLOCK_SPEED  (4194304) // Hz
#define SGB_CLOCK_SPEED (4295454) // Hz

extern uint64_t Ticks;

extern unsigned ClockSpeed;

void tick(unsigned cycles);

#endif // CLOCK_H