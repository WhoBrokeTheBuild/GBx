#ifndef OR_H
#define OR_H

#include "../alu.h"
#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _OR(x) \
    R.A = or8(R.A, (x))

static void _OR_A()
{
    LogVerbose(3, "OR A");
    _OR(R.A);
}

static void _OR_B()
{
    LogVerbose(3, "OR B");
    _OR(R.B);
}

static void _OR_C()
{
    LogVerbose(3, "OR C");
    _OR(R.C);
}

static void _OR_D()
{
    LogVerbose(3, "OR D");
    _OR(R.D);
}

static void _OR_E()
{
    LogVerbose(3, "OR E");
    _OR(R.E);
}

static void _OR_H()
{
    LogVerbose(3, "OR H");
    _OR(R.H);
}

static void _OR_L()
{
    LogVerbose(3, "OR L");
    _OR(R.L);
}

static void _OR_pHL()
{
    LogVerbose(3, "OR HL");
    uint8_t n = readByte(R.HL);
    tick(4);

    _OR(n);
}

static void _OR_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "OR %02Xh", u);

    _OR(u);
}

#undef _OR

#endif // OR_H