#ifndef RET_H
#define RET_H

#include "../memory.h"
#include "../register.h"

static void _RET()
{
    LogVerbose("RET");

    uint8_t nn = popWord();
    TickCounter += 4;

    R.PC = nn;
    TickCounter += 4;
}

static void _RET_NZ()
{
    LogVerbose("RET NZ");

    uint8_t nn = popWord();
    TickCounter += 4;

    if (!R.FZ) {
        R.PC = nn;
        TickCounter += 4;
    }
}

static void _RET_Z()
{
    LogVerbose("RET Z");
    
    uint8_t nn = popWord();
    TickCounter += 4;

    if (R.FZ) {
        R.PC = nn;
        TickCounter += 4;
    }
}

static void _RET_NC()
{
    LogVerbose("RET NC");
    
    uint8_t nn = popWord();
    TickCounter += 4;

    if (!R.FC) {
        R.PC = nn;
        TickCounter += 4;
    }
}

static void _RET_C()
{
    LogVerbose("RET C");
    
    uint8_t nn = popWord();
    TickCounter += 4;

    if (R.FC) {
        R.PC = nn;
        TickCounter += 4;
    }
}

static void _RETI()
{
    LogVerbose("RETI");
    
    uint8_t nn = popWord();
    TickCounter += 4;

    R.PC = nn;
    TickCounter += 4;
}

#endif // RET_H