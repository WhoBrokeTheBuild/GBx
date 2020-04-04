#ifndef GBX_DEBUG_H
#define GBX_DEBUG_H

#include <GBx/Types.h>

#include <assert.h>

extern int VerboseLevel;

extern bool MemoryTrackingEnabled;

typedef struct
{
    uint8_t Read : 4;
    uint8_t Write : 4;

} memory_tracker_entry_t;

static_assert(
    sizeof(memory_tracker_entry_t) == 1, "sizeof(memory_tracker_entry_t) != 1");

extern memory_tracker_entry_t MemoryTracker[0x10000];

void AgeMemoryTracker();

#endif // GBX_DEBUG_H