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
    LogDebug("OR A");
    _OR(R.A);
}

static void _OR_B()
{
    LogDebug("OR B");
    _OR(R.B);
}

static void _OR_C()
{
    LogDebug("OR C");
    _OR(R.C);
}

static void _OR_D()
{
    LogDebug("OR D");
    _OR(R.D);
}

static void _OR_E()
{
    LogDebug("OR E");
    _OR(R.E);
}

static void _OR_H()
{
    LogDebug("OR H");
    _OR(R.H);
}

static void _OR_L()
{
    LogDebug("OR L");
    _OR(R.L);
}

static void _OR_pHL()
{
    LogDebug("OR HL");
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _OR(n);
}

static void _OR_u()
{
    uint8_t u = nextByte();
    cpuTick(4);

    LogDebug("OR %02Xh", u);

    _OR(u);
}

#undef _OR

#endif // OR_H