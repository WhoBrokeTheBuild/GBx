#ifndef SM83_INTERRUPTS_H
#define SM83_INTERRUPTS_H

#include <SM83/Context.h>
#include <SM83/Types.h>

void SM83_CallInterrupt(sm83_t * cpu, uint16_t addr);

void SM83_CheckInterrupts(sm83_t * cpu);

void SM83_PrintInterrupts(sm83_t * cpu);

#endif // SM83_INTERRUPTS_H