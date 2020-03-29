#ifndef GBX_DEBUG_H
#define GBX_DEBUG_H

extern int VerboseLevel;

#define INSTRUCTION_LOG_LENGTH (32)
#define INSTRUCTION_LOG_ENTRY_SIZE (21)

void AddInstructionToLog(const char * inst);

int GetInstructionLogSize();

char * GetInstructionLogEntry(int index);

#endif // GBX_DEBUG_H