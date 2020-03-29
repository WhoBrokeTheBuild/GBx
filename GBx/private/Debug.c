#include <GBx/Debug.h>
#include <GBx/Log.h>

#include <string.h>

int VerboseLevel = 0;

char instructionLog[INSTRUCTION_LOG_LENGTH][INSTRUCTION_LOG_ENTRY_SIZE];
int instructionLogSize = 0;
int instructionLogIndex = 0;

void AddInstructionToLog(const char * inst)
{
    strncpy(instructionLog[instructionLogIndex], inst, INSTRUCTION_LOG_ENTRY_SIZE);

    ++instructionLogIndex;
    instructionLogIndex %= INSTRUCTION_LOG_LENGTH;

    ++instructionLogSize;
    if (instructionLogSize > INSTRUCTION_LOG_LENGTH) {
        instructionLogSize = INSTRUCTION_LOG_LENGTH;
    }
}

int GetInstructionLogSize()
{
    return instructionLogSize;
}

char * GetInstructionLogEntry(int index)
{
    index = instructionLogIndex - index - 1;
    if (index < 0) {
        index += INSTRUCTION_LOG_LENGTH;
    }
    return instructionLog[index];
}