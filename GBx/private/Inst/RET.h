#ifndef RET_H
#define RET_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Interrupts.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

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