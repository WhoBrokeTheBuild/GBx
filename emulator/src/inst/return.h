#ifndef RET_H
#define RET_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"
#include "../interrupt.h"

static void _RET()
{
    LogDebug("RET");

    uint16_t nn = popWord();
    tick(8);

    R.PC = nn;
    tick(8);
}

static void _RET_NZ()
{
    LogDebug("RET NZ");

    tick(8);

    if (!R.FZ) {
        uint16_t nn = popWord();
        tick(8);

        R.PC = nn;
        tick(4);
    }
}

static void _RET_Z()
{
    LogDebug("RET Z");

    tick(8);

    if (R.FZ) {
        uint16_t nn = popWord();
        tick(8);

        R.PC = nn;
        tick(4);
    }
}

static void _RET_NC()
{
    LogDebug("RET NC");

    tick(8);

    if (!R.FC) {
        uint16_t nn = popWord();
        tick(8);

        R.PC = nn;
        tick(4);
    }
}

static void _RET_C()
{
    LogDebug("RET C");

    tick(8);

    if (R.FC) {
        uint16_t nn = popWord();
        tick(8);

        R.PC = nn;
        tick(4);
    }
}

static void _RETI()
{
    LogDebug("RETI");
    
    uint16_t nn = popWord();
    tick(8);

    R.PC = nn;
    tick(8);

    IME = true;
}

#endif // RET_H