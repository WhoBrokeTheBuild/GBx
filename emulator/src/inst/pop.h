#ifndef POP_H
#define POP_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _POP_AF()
{
    LogInst("POP AF");
    R.AF = popWord();
    tick(12);
}

static void _POP_BC()
{
    LogInst("POP BC");
    R.BC = popWord();
    tick(12);
}

static void _POP_DE()
{
    LogInst("POP DE");
    R.DE = popWord();
    tick(12);
}

static void _POP_HL()
{
    LogInst("POP HL");
    R.HL = popWord();
    tick(12);
}

#endif // POP_H