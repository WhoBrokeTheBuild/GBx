#ifndef POP_H
#define POP_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

static void _POP_AF()
{
    LogInst("POP AF");
    R.AF = PopWord();
    Tick(12);
}

static void _POP_BC()
{
    LogInst("POP BC");
    R.BC = PopWord();
    Tick(12);
}

static void _POP_DE()
{
    LogInst("POP DE");
    R.DE = PopWord();
    Tick(12);
}

static void _POP_HL()
{
    LogInst("POP HL");
    R.HL = PopWord();
    Tick(12);
}

#endif // POP_H