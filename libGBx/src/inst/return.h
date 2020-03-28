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

    word nn = PopWord();
    Tick(8);

    R.PC = nn;
    Tick(8);
}

static void _RET_NZ()
{
    LogInst("RET NZ");

    Tick(8);

    if (!R.FZ) {
        Tick(4);

        word nn = PopWord();

        R.PC = nn;
        Tick(8);
    }
}

static void _RET_Z()
{
    LogInst("RET Z");

    Tick(8);

    if (R.FZ) {
        Tick(4);

        word nn = PopWord();

        R.PC = nn;
        Tick(8);
    }
}

static void _RET_NC()
{
    LogInst("RET NC");

    Tick(8);

    if (!R.FC) {
        Tick(4);

        word nn = PopWord();

        R.PC = nn;
        Tick(8);
    }
}

static void _RET_C()
{
    LogInst("RET C");

    Tick(8);

    if (R.FC) {
        Tick(4);

        word nn = PopWord();

        R.PC = nn;
        Tick(8);
    }
}

static void _RETI()
{
    LogInst("RETI");
    
    word nn = PopWord();
    Tick(8);

    R.PC = nn;
    Tick(8);

    IME = true;
}

#endif // RET_H