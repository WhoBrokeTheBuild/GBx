#include <SM83/SM83.h>

#include <string.h>

void SM83_AddInstructionLogEntry(sm83_t * cpu, const char * inst)
{
    if (!cpu->InstructionLoggingEnabled) {
        return;
    }

    strncpy(cpu->InstructionLog[cpu->InstructionLogIndex], inst,
        INSTRUCTION_LOG_ENTRY_SIZE);

    ++cpu->InstructionLogIndex;
    cpu->InstructionLogIndex %= INSTRUCTION_LOG_LENGTH;
}

const char * SM83_GetInstructionLogEntry(sm83_t * cpu, int index)
{
    if (!cpu->InstructionLoggingEnabled) {
        NULL;
    }

    index = cpu->InstructionLogIndex - index - 1;
    if (index < 0) {
        index += INSTRUCTION_LOG_LENGTH;
    }
    return cpu->InstructionLog[index];
}