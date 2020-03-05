#ifndef PUSH_H
#define PUSH_H

#include "../clock.h"
#include "../cpu.h"
#include "../log.h"
#include "../memory.h"

static void _PUSH_AF()
{
    LogInst("PUSH AF");
    pushWord(R.AF);
    tick(16);
}

static void _PUSH_BC()
{
    LogInst("PUSH BC");
    pushWord(R.BC);
    tick(16);
}

static void _PUSH_DE()
{
    LogInst("PUSH DE");
    pushWord(R.DE);
    tick(16);
}

static void _PUSH_HL()
{
    LogInst("PUSH HL");
    pushWord(R.HL);
    tick(16);
}

#endif // PUSH_H