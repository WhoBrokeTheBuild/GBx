#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

extern bool CPUEnabled;

extern uint32_t ClockSpeed;
extern uint64_t TickCounter;

typedef void(* inst_t)();

void tick(uint8_t cycles);

uint8_t fetch();
void execute(uint8_t op);

void cpuTick();

#endif // CPU_H