#ifndef PUSH_H
#define PUSH_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _PUSH_AF()
{
    LogDebug("PUSH AF");
    pushWord(R.AF);
    cpuTick(16);
}

static void _PUSH_BC()
{
    LogDebug("PUSH BC");
    pushWord(R.BC);
    cpuTick(16);
}

static void _PUSH_DE()
{
    LogDebug("PUSH DE");
    pushWord(R.DE);
    cpuTick(16);
}

static void _PUSH_HL()
{
    LogDebug("PUSH HL");
    pushWord(R.HL);
    cpuTick(16);
}

#endif // PUSH_H