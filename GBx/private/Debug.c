#include <GBx/Debug.h>
#include <GBx/Log.h>

#include <string.h>

int VerboseLevel = 0;

bool InstructionLoggingEnabled = false;

bool MemoryTrackingEnabled = false;

memory_tracker_entry_t MemoryTracker[0x10000];

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