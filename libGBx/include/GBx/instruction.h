#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>
#include <stddef.h>

uint16_t disassemble(char * str, size_t len, uint16_t addr);

#endif // INSTRUCTION_H