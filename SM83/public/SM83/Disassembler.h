#ifndef SM83_DISASSEMBLER_H
#define SM83_DISASSEMBLER_H

#include <SM83/Types.h>

typedef struct sm83 sm83_t;

uint16_t SM83_Disassemble(sm83_t * ctx, char * str, size_t len, uint16_t addr);

#endif // SM83_DISASSEMBLER_H