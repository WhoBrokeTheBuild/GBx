#ifndef RET_H
#define RET_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"
#include "../interrupt.h"

static void _RET()
{
    LogDebug("RET");

    uint16_t nn = popWord();
    cpuTick(8);

    R.PC = nn;
    cpuTick(8);
}

static void _RET_NZ()
{
    LogDebug("RET NZ");

    cpuTick(8);

    if (!R.FZ) {
        uint16_t nn = popWord();
        cpuTick(8);

        R.PC = nn;
        cpuTick(4);
    }
}

static void _RET_Z()
{
    LogDebug("RET Z");

    cpuTick(8);

    if (R.FZ) {
        uint16_t nn = popWord();
        cpuTick(8);

        R.PC = nn;
        cpuTick(4);
    }
}

static void _RET_NC()
{
    LogDebug("RET NC");

    cpuTick(8);

    if (!R.FC) {
        uint16_t nn = popWord();
        cpuTick(8);

        R.PC = nn;
        cpuTick(4);
    }
}

static void _RET_C()
{
    LogDebug("RET C");

    cpuTick(8);

    if (R.FC) {
        uint16_t nn = popWord();
        cpuTick(8);

        R.PC = nn;
        cpuTick(4);
    }
}

static void _RETI()
{
    LogDebug("RETI");
    
    uint16_t nn = popWord();
    cpuTick(8);

    R.PC = nn;
    cpuTick(8);

    IME = true;
}

#endif // RET_H