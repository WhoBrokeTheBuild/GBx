#ifndef PUSH_H
#define PUSH_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _PUSH_AF()
{
    LogDebug("PUSH AF");
    pushWord(R.AF);
    tick(16);
}

static void _PUSH_BC()
{
    LogDebug("PUSH BC");
    pushWord(R.BC);
    tick(16);
}

static void _PUSH_DE()
{
    LogDebug("PUSH DE");
    pushWord(R.DE);
    tick(16);
}

static void _PUSH_HL()
{
    LogDebug("PUSH HL");
    pushWord(R.HL);
    tick(16);
}

#endif // PUSH_H