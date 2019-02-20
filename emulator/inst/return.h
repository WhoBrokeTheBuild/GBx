#ifndef RET_H
#define RET_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _RET()
{
    LogDebug("RET");

    uint8_t nn = popWord();
    tick(4);

    R.PC = nn;
    tick(4);
}

static void _RET_NZ()
{
    LogDebug("RET NZ");

    uint8_t nn = popWord();
    tick(4);

    if (!R.FZ) {
        R.PC = nn;
        tick(4);
    }
}

static void _RET_Z()
{
    LogDebug("RET Z");
    
    uint8_t nn = popWord();
    tick(4);

    if (R.FZ) {
        R.PC = nn;
        tick(4);
    }
}

static void _RET_NC()
{
    LogDebug("RET NC");
    
    uint8_t nn = popWord();
    tick(4);

    if (!R.FC) {
        R.PC = nn;
        tick(4);
    }
}

static void _RET_C()
{
    LogDebug("RET C");
    
    uint8_t nn = popWord();
    tick(4);

    if (R.FC) {
        R.PC = nn;
        tick(4);
    }
}

static void _RETI()
{
    LogDebug("RETI");
    
    uint8_t nn = popWord();
    tick(4);

    R.PC = nn;
    tick(4);
}

#endif // RET_H