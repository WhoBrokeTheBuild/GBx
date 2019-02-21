#ifndef AND_H
#define AND_H

#include "../alu.h"
#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _AND(x) \
    (x) = and8(R.A, (x))

static void _AND_A()
{
    LogDebug("AND A");
    _AND(R.A);
}

static void _AND_B()
{
    LogDebug("AND B");
    _AND(R.B);
}

static void _AND_C()
{
    LogDebug("AND C");
    _AND(R.C);
}

static void _AND_D()
{
    LogDebug("AND D");
    _AND(R.D);
}

static void _AND_E()
{
    LogDebug("AND E");
    _AND(R.E);
}

static void _AND_H()
{
    LogDebug("AND H");
    _AND(R.H);
}

static void _AND_L()
{
    LogDebug("AND L");
    _AND(R.L);
}

static void _AND_pHL()
{
    LogDebug("AND HL");
    uint8_t n = readByte(R.HL);
    tick(4);

    _AND(n);
}

static void _AND_n()
{
    uint8_t n = nextByte();
    tick(4);

    LogDebug("AND %02Xh", n);

    _AND(n);
}

#undef _AND

#endif // AND_H