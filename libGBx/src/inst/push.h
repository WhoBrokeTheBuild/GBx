#ifndef PUSH_H
#define PUSH_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

static void _PUSH_AF()
{
    LogInst("PUSH AF");
    PushWord(R.AF);
    Tick(16);
}

static void _PUSH_BC()
{
    LogInst("PUSH BC");
    PushWord(R.BC);
    Tick(16);
}

static void _PUSH_DE()
{
    LogInst("PUSH DE");
    PushWord(R.DE);
    Tick(16);
}

static void _PUSH_HL()
{
    LogInst("PUSH HL");
    PushWord(R.HL);
    Tick(16);
}

#endif // PUSH_H