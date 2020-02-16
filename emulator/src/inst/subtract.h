#ifndef SUBTRACT_H
#define SUBTRACT_H

#include "../alu.h"
#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _SUB(x) \
    R.A = sub8(R.A, (x))

#define _SBC(x) \
    R.A = sub8(R.A, (x) + (int)R.FC)

static void _SUB_A()
{
    LogDebug("SUB A,A");
    _SUB(R.A);
}

static void _SUB_B()
{
    LogDebug("SUB A,B");
    _SUB(R.B);
}

static void _SUB_C()
{
    LogDebug("SUB A,C");
    _SUB(R.C);
}

static void _SUB_D()
{
    LogDebug("SUB A,D");
    _SUB(R.D);
}

static void _SUB_E()
{
    LogDebug("SUB A,E");
    _SUB(R.E);
}

static void _SUB_H()
{
    LogDebug("SUB A,H");
    _SUB(R.H);
}

static void _SUB_L()
{
    LogDebug("SUB A,L");
    _SUB(R.L);
}

static void _SUB_pHL()
{
    LogDebug("SUB A,(HL)");
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _SUB(n);
}

static void _SUB_u()
{
    uint8_t u = nextByte();
    cpuTick(4);

    LogDebug("SUB A,%02Xh", u);

    _SUB(u);
}

static void _SBC_A()
{
    LogDebug("SBC A,A");
    _SBC(R.A);
}

static void _SBC_B()
{
    LogDebug("SBC A,B");
    _SBC(R.B);
}

static void _SBC_C()
{
    LogDebug("SBC A,C");
    _SBC(R.C);
}

static void _SBC_D()
{
    LogDebug("SBC A,D");
    _SBC(R.D);
}

static void _SBC_E()
{
    LogDebug("SBC A,E");
    _SBC(R.E);
}

static void _SBC_H()
{
    LogDebug("SBC A,H");
    _SBC(R.H);
}

static void _SBC_L()
{
    LogDebug("SBC A,L");
    _SBC(R.L);
}

static void _SBC_pHL()
{
    LogDebug("SBC A,(HL)");
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _SBC(n);
}

static void _SBC_u()
{
    uint8_t u = nextByte();
    cpuTick(4);

    LogDebug("SBC A,%02Xh", u);

    _SBC(u);
}

#undef _SBC
#undef _SUB

#endif // SUBTRACT_H