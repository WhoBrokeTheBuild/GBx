#ifndef GBXASM_INSTRUCTION_H
#define GBXASM_INSTRUCTION_H

#include <stdlib.h>
#include <stdbool.h>

#define INST_ARG_MAX_LEN (512)

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

int GetInstructionSize(Instruction * inst);

bool ParseInstructionArgument(char out[INST_ARG_MAX_LEN], 
    const char * arg, ArgumentType type, const char * literal);

Instruction * GetInstructionList();

#endif // GBXASM_INSTRUCTION_H