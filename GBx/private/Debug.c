#include <GBx/Debug.h>
#include <GBx/Log.h>

#include <string.h>

int VerboseLevel = 0;

bool InstructionLoggingEnabled = false;

bool MemoryTrackingEnabled = false;

memory_tracker_entry_t MemoryTracker[0x10000];

char instructionLog[INSTRUCTION_LOG_LENGTH][INSTRUCTION_LOG_ENTRY_SIZE];
int instructionLogIndex = 0;

void AddInstructionLogEntry(const char * inst)
{
    if (!InstructionLoggingEnabled) {
        return;
    }

    strncpy(instructionLog[instructionLogIndex], inst, INSTRUCTION_LOG_ENTRY_SIZE);

    ++instructionLogIndex;
    instructionLogIndex %= INSTRUCTION_LOG_LENGTH;
}

const char * GetInstructionLogEntry(int index)
{
    if (!InstructionLoggingEnabled) {
        NULL;
    }

    index = instructionLogIndex - index - 1;
    if (index < 0) {
        index += INSTRUCTION_LOG_LENGTH;
    }
    return instructionLog[index];
}

void AgeMemoryTracker()
{
    for (unsigned i = 0; i < sizeof(MemoryTracker); ++i) {
        if (MemoryTracker[i].Read > 0) {
            --MemoryTracker[i].Read;
        }
        if (MemoryTracker[i].Write > 0) {
            --MemoryTracker[i].Write;
        }
    }
}