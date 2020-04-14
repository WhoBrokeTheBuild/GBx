#ifndef GBX_INTERNAL_H
#define GBX_INTERNAL_H

#include <GBx/Context.h>

typedef struct gbx_internal
{
    // APU

    int Tone1Timer;
    int Tone1DutyIndex;

    int Tone2Timer;
    int Tone2DutyIndex;

    int WaveTimer;

    int NoiseTimer;

    // Bootstrape

    bool HaveBootstrap;

    // Cartridge

    bool HaveSRAM;

    // PPU

    unsigned PPUModeTicks;

    // Timer

    int TimerCounter;
    int DIVCounter;

    // Debug

    gbx_memory_tracker_entry_t * MemoryTracker;

} gbx_internal_t;

#endif // GBX_INTERNAL_H