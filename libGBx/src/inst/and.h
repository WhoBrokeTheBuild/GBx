#ifndef AND_H
#define AND_H

#include "alu.h"
#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _AND(x) \
    R.A = and8(R.A, (x))

static void _AND_A()
{
    LogInst("AND A");
    _AND(R.A);
}

static void _AND_B()
{
    LogInst("AND B");
    _AND(R.B);
}

static void _AND_C()
{
    LogInst("AND C");
    _AND(R.C);
}

static void _AND_D()
{
    LogInst("AND D");
    _AND(R.D);
}

static void _AND_E()
{
    LogInst("AND E");
    _AND(R.E);
}

static void _AND_H()
{
    LogInst("AND H");
    _AND(R.H);
}

static void _AND_L()
{
    LogInst("AND L");
    _AND(R.L);
}

static void _AND_pHL()
{
    LogInst("AND HL");
    uint8_t n = readByte(R.HL);
    tick(4);

    _AND(n);
}

static void _AND_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogInst("AND %02Xh", u);

    _AND(u);
}

#undef _AND

#endif // AND_H