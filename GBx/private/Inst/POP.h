#ifndef INST_POP_H
#define INST_POP_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

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

#endif // INST_POP_H