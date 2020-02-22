#ifndef POP_H
#define POP_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _POP_AF()
{
    LogVerbose(3, "POP AF");
    R.AF = popWord();
    tick(12);
}

static void _POP_BC()
{
    LogVerbose(3, "POP BC");
    R.BC = popWord();
    tick(12);
}

static void _POP_DE()
{
    LogVerbose(3, "POP DE");
    R.DE = popWord();
    tick(12);
}

static void _POP_HL()
{
    LogVerbose(3, "POP HL");
    R.HL = popWord();
    tick(12);
}

#endif // POP_H