#ifndef RET_H
#define RET_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"
#include "interrupt.h"

static void _RET()
{
    LogInst("RET");

    uint16_t nn = popWord();
    tick(8);

    R.PC = nn;
    tick(8);
}

static void _RET_NZ()
{
    LogInst("RET NZ");

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
    LogInst("RET Z");

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
    LogInst("RET NC");

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
    LogInst("RET C");

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
    LogInst("RETI");
    
    uint16_t nn = popWord();
    tick(8);

    R.PC = nn;
    tick(8);

    IME = true;
}

#endif // RET_H