#ifndef GBX_DEBUG_H
#define GBX_DEBUG_H

#include <GBx/Types.h>

#include <assert.h>

extern int VerboseLevel;

extern bool InstructionLoggingEnabled;

extern bool MemoryTrackingEnabled;

typedef struct 
{
    byte Read:4;
    byte Write:4;
    
} memory_tracker_entry_t;

static_assert(sizeof(memory_tracker_entry_t) == 1, 
    "sizeof(memory_tracker_entry_t) != 1");

extern memory_tracker_entry_t MemoryTracker[0x10000];

#define INSTRUCTION_LOG_LENGTH     (32)
#define INSTRUCTION_LOG_ENTRY_SIZE (24)

void AddInstructionLogEntry(const char * inst);

const char * GetInstructionLogEntry(int index);

void AgeMemoryTracker();

#endif // GBX_DEBUG_H