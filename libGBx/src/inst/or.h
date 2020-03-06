#ifndef OR_H
#define OR_H

#include "alu.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _OR(x) \
    R.A = or8(R.A, (x))

static void _OR_A()
{
    LogInst("OR A");
    _OR(R.A);
}

static void _OR_B()
{
    LogInst("OR B");
    _OR(R.B);
}

static void _OR_C()
{
    LogInst("OR C");
    _OR(R.C);
}

static void _OR_D()
{
    LogInst("OR D");
    _OR(R.D);
}

static void _OR_E()
{
    LogInst("OR E");
    _OR(R.E);
}

static void _OR_H()
{
    LogInst("OR H");
    _OR(R.H);
}

static void _OR_L()
{
    LogInst("OR L");
    _OR(R.L);
}

static void _OR_pHL()
{
    LogInst("OR HL");
    uint8_t n = readByte(R.HL);
    tick(4);

    _OR(n);
}

static void _OR_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogInst("OR %02Xh", u);

    _OR(u);
}

#undef _OR

#endif // OR_H