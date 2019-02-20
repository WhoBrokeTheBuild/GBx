#ifndef POP_H
#define POP_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _POP_AF()
{
    LogDebug("POP AF");
    R.AF = popWord();
}

static void _POP_BC()
{
    LogDebug("POP BC");
    R.BC = popWord();
}

static void _POP_DE()
{
    LogDebug("POP DE");
    R.DE = popWord();
}

static void _POP_HL()
{
    LogDebug("POP HL");
    R.HL = popWord();
}

#endif // POP_H