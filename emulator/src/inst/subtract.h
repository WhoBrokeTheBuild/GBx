#ifndef SUBTRACT_H
#define SUBTRACT_H

#include "../alu.h"
#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _SUB(x) \
    R.A = sub8(R.A, (x))

#define _SBC(x) \
    R.A = sub8(R.A, (x) + (int)R.FC)

static void _SUB_A()
{
    LogVerbose(3, "SUB A,A");
    _SUB(R.A);
}

static void _SUB_B()
{
    LogVerbose(3, "SUB A,B");
    _SUB(R.B);
}

static void _SUB_C()
{
    LogVerbose(3, "SUB A,C");
    _SUB(R.C);
}

static void _SUB_D()
{
    LogVerbose(3, "SUB A,D");
    _SUB(R.D);
}

static void _SUB_E()
{
    LogVerbose(3, "SUB A,E");
    _SUB(R.E);
}

static void _SUB_H()
{
    LogVerbose(3, "SUB A,H");
    _SUB(R.H);
}

static void _SUB_L()
{
    LogVerbose(3, "SUB A,L");
    _SUB(R.L);
}

static void _SUB_pHL()
{
    LogVerbose(3, "SUB A,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SUB(n);
}

static void _SUB_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "SUB A,%02Xh", u);

    _SUB(u);
}

static void _SBC_A()
{
    LogVerbose(3, "SBC A,A");
    _SBC(R.A);
}

static void _SBC_B()
{
    LogVerbose(3, "SBC A,B");
    _SBC(R.B);
}

static void _SBC_C()
{
    LogVerbose(3, "SBC A,C");
    _SBC(R.C);
}

static void _SBC_D()
{
    LogVerbose(3, "SBC A,D");
    _SBC(R.D);
}

static void _SBC_E()
{
    LogVerbose(3, "SBC A,E");
    _SBC(R.E);
}

static void _SBC_H()
{
    LogVerbose(3, "SBC A,H");
    _SBC(R.H);
}

static void _SBC_L()
{
    LogVerbose(3, "SBC A,L");
    _SBC(R.L);
}

static void _SBC_pHL()
{
    LogVerbose(3, "SBC A,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SBC(n);
}

static void _SBC_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "SBC A,%02Xh", u);

    _SBC(u);
}

#undef _SBC
#undef _SUB

#endif // SUBTRACT_H