#ifndef RET_H
#define RET_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _RET()
{
    LogDebug("RET");

    uint16_t nn = popWord();
    cpuTick(4);

    R.PC = nn;
    cpuTick(4);
}

static void _RET_NZ()
{
    LogDebug("RET NZ");

    uint16_t nn = popWord();
    cpuTick(4);

    if (!R.FZ) {
        R.PC = nn;
        cpuTick(4);
    }
}

static void _RET_Z()
{
    LogDebug("RET Z");
    
    uint16_t nn = popWord();
    cpuTick(4);

    if (R.FZ) {
        R.PC = nn;
        cpuTick(4);
    }
}

static void _RET_NC()
{
    LogDebug("RET NC");
    
    uint16_t nn = popWord();
    cpuTick(4);

    if (!R.FC) {
        R.PC = nn;
        cpuTick(4);
    }
}

static void _RET_C()
{
    LogDebug("RET C");
    
    uint16_t nn = popWord();
    cpuTick(4);

    if (R.FC) {
        R.PC = nn;
        cpuTick(4);
    }
}

static void _RETI()
{
    LogDebug("RETI");
    
    uint16_t nn = popWord();
    cpuTick(4);

    R.PC = nn;
    cpuTick(4);
}

#endif // RET_H