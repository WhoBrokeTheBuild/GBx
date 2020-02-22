#ifndef RET_H
#define RET_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"
#include "../interrupt.h"

static void _RET()
{
    LogVerbose(3, "RET");

    uint16_t nn = popWord();
    tick(8);

    R.PC = nn;
    tick(8);
}

static void _RET_NZ()
{
    LogVerbose(3, "RET NZ");

    uint16_t nn = popWord();
    tick(8);

    if (!R.FZ) {
        tick(4);

        R.PC = nn;
        tick(8);
    }
}

static void _RET_Z()
{
    LogVerbose(3, "RET Z");

    uint16_t nn = popWord();
    tick(8);

    if (R.FZ) {
        tick(4);

        R.PC = nn;
        tick(8);
    }
}

static void _RET_NC()
{
    LogVerbose(3, "RET NC");

    uint16_t nn = popWord();
    tick(8);

    if (!R.FC) {
        tick(4);

        R.PC = nn;
        tick(8);
    }
}

static void _RET_C()
{
    LogVerbose(3, "RET C");

    uint16_t nn = popWord();
    tick(8);

    if (R.FC) {
        tick(4);

        R.PC = nn;
        tick(8);
    }
}

static void _RETI()
{
    LogVerbose(3, "RETI");
    
    uint16_t nn = popWord();
    tick(8);

    R.PC = nn;
    tick(8);

    IME = true;
}

#endif // RET_H