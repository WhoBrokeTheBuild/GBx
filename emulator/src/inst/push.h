#ifndef PUSH_H
#define PUSH_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _PUSH_AF()
{
    LogVerbose(3, "PUSH AF");
    pushWord(R.AF);
    tick(16);
}

static void _PUSH_BC()
{
    LogVerbose(3, "PUSH BC");
    pushWord(R.BC);
    tick(16);
}

static void _PUSH_DE()
{
    LogVerbose(3, "PUSH DE");
    pushWord(R.DE);
    tick(16);
}

static void _PUSH_HL()
{
    LogVerbose(3, "PUSH HL");
    pushWord(R.HL);
    tick(16);
}

#endif // PUSH_H