#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

extern bool CPUEnabled;
extern bool CPULimit;

extern uint32_t ClockSpeed;
extern uint64_t CPUTicks;

typedef void(* inst_t)();

void cpuTick(unsigned cycles);

uint8_t fetch();

void execute(uint8_t op);

#endif // CPU_H