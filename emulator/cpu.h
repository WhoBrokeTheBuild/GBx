#ifndef CPU_H
#define CPU_H

#include <stdint.h>

extern uint32_t ClockSpeed;
extern uint64_t TickCounter;

void CPUTick();

#endif // CPU_H