#ifndef AND_H
#define AND_H

#include "../alu.h"
#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _AND(x) \
    R.A = and8(R.A, (x))

static void _AND_A()
{
    LogVerbose(3, "AND A");
    _AND(R.A);
}

static void _AND_B()
{
    LogVerbose(3, "AND B");
    _AND(R.B);
}

static void _AND_C()
{
    LogVerbose(3, "AND C");
    _AND(R.C);
}

static void _AND_D()
{
    LogVerbose(3, "AND D");
    _AND(R.D);
}

static void _AND_E()
{
    LogVerbose(3, "AND E");
    _AND(R.E);
}

static void _AND_H()
{
    LogVerbose(3, "AND H");
    _AND(R.H);
}

static void _AND_L()
{
    LogVerbose(3, "AND L");
    _AND(R.L);
}

static void _AND_pHL()
{
    LogVerbose(3, "AND HL");
    uint8_t n = readByte(R.HL);
    tick(4);

    _AND(n);
}

static void _AND_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "AND %02Xh", u);

    _AND(u);
}

#undef _AND

#endif // AND_H