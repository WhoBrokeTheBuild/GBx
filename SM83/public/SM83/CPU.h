#ifndef SM83_CPU_H
#define SM83_CPU_H

#include <SM83/Context.h>
#include <SM83/Types.h>

typedef void (*sm83_instruction_t)(sm83_t *);
typedef void (*sm83_bit_instruction_t)(sm83_t *, int);

uint8_t SM83_Fetch(sm83_t * cpu);

sm83_instruction_t SM83_Decode(sm83_t * cpu, uint8_t opcode);

void SM83_Execute(sm83_t * cpu, sm83_instruction_t inst);

void SM83_Step(sm83_t * cpu);

void SM83_PrintRegisters(sm83_t * cpu);

#endif // SM83_CPU_H