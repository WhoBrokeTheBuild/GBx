#ifndef SM83_DISASSEMBLER_H
#define SM83_DISASSEMBLER_H

#include <SM83/SM83.h>

uint16_t SM83_Disassemble(sm83_t * cpu, char * str, size_t len, uint16_t addr);

#endif // SM83_DISASSEMBLER_H