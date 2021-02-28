#ifndef GBXASM_INSTRUCTION_H
#define GBXASM_INSTRUCTION_H

#include <stdlib.h>

typedef enum _ArgumentType
{
    ARG_TYPE_NONE,
    ARG_TYPE_LITERAL,
    ARG_TYPE_U8,
    ARG_TYPE_U16,
    ARG_TYPE_S8,
    ARG_TYPE_ADDR_U16,  // ($xxxx)
    ARG_TYPE_FF00_U8,   // ($FF00+$xx)
    ARG_TYPE_SP_S8,     // SP+xx

} ArgumentType;

typedef struct _Instruction
{
    const char * Name;

    int Opcode1;

    int Opcode2;

    ArgumentType Type1;

    const char * Literal1;

    ArgumentType Type2;

    const char * Literal2;

} Instruction;

extern Instruction _InstructionList[];

int GetInstructionSize(Instruction * inst);

#endif // GBXASM_INSTRUCTION_H