#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "types.h"

#define INSTRUCTION_LOG_LENGTH (32)
#define INSTRUCTION_LOG_ENTRY_SIZE (20)

word Disassemble(char * str, uint len, word addr);

void AddInstructionToLog(const char * inst);

int GetInstructionLogSize();

char * GetInstructionLogEntry(int index);

#endif // INSTRUCTION_H