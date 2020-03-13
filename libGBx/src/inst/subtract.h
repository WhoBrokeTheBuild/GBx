#ifndef SUBTRACT_H
#define SUBTRACT_H

#include "alu.h"
#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _SUB(x) \
    R.A = sub8(R.A, (x))

#define _SBC(x) \
    R.A = sub8(R.A, (x) + (int)R.FC)

static void _SUB_A()
{
    LogInst("SUB A,A");
    _SUB(R.A);
}

static void _SUB_B()
{
    LogInst("SUB A,B");
    _SUB(R.B);
}

static void _SUB_C()
{
    LogInst("SUB A,C");
    _SUB(R.C);
}

static void _SUB_D()
{
    LogInst("SUB A,D");
    _SUB(R.D);
}

static void _SUB_E()
{
    LogInst("SUB A,E");
    _SUB(R.E);
}

static void _SUB_H()
{
    LogInst("SUB A,H");
    _SUB(R.H);
}

static void _SUB_L()
{
    LogInst("SUB A,L");
    _SUB(R.L);
}

static void _SUB_pHL()
{
    LogInst("SUB A,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    _SUB(n);
}

static void _SUB_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("SUB A,%02Xh", u);

    _SUB(u);
}

static void _SBC_A()
{
    LogInst("SBC A,A");
    _SBC(R.A);
}

static void _SBC_B()
{
    LogInst("SBC A,B");
    _SBC(R.B);
}

static void _SBC_C()
{
    LogInst("SBC A,C");
    _SBC(R.C);
}

static void _SBC_D()
{
    LogInst("SBC A,D");
    _SBC(R.D);
}

static void _SBC_E()
{
    LogInst("SBC A,E");
    _SBC(R.E);
}

static void _SBC_H()
{
    LogInst("SBC A,H");
    _SBC(R.H);
}

static void _SBC_L()
{
    LogInst("SBC A,L");
    _SBC(R.L);
}

static void _SBC_pHL()
{
    LogInst("SBC A,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    _SBC(n);
}

static void _SBC_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("SBC A,%02Xh", u);

    _SBC(u);
}

#undef _SBC
#undef _SUB

#endif // SUBTRACT_H