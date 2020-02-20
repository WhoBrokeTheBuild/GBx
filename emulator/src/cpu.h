#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>

extern bool CPUEnabled;

typedef void(* inst_t)();

uint8_t fetch();

void execute(uint8_t op);

void nextInstruction();

#endif // CPU_H