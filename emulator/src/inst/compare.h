#ifndef COMPARE_H
#define COMPARE_H

#include "../alu.h"
#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _CP(x) \
    sub8(R.A, (x))

static void _CP_A()
{
    LogDebug("CP A");
    _CP(R.A);
}

static void _CP_B()
{
    LogDebug("CP B");
    _CP(R.B);
}

static void _CP_C()
{
    LogDebug("CP C");
    _CP(R.C);
}

static void _CP_D()
{
    LogDebug("CP D");
    _CP(R.D);
}

static void _CP_E()
{
    LogDebug("CP E");
    _CP(R.E);
}

static void _CP_H()
{
    LogDebug("CP H");
    _CP(R.H);
}

static void _CP_L()
{
    LogDebug("CP L");
    _CP(R.L);
}

static void _CP_pHL()
{
    LogDebug("CP (HL)");
    uint8_t pHL = readByte(R.HL);
    tick(4);

    _CP(pHL);
}

static void _CP_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("CP %02Xh", u);

    _CP(u);
}

#undef _CP

#endif // COMPARE_H