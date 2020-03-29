#ifndef SUBTRACT_H
#define SUBTRACT_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _SUB_A()
{
    LogInst("SUB A,A");
    R.A = SubtractBytes(R.A, R.A);
}

static void _SUB_B()
{
    LogInst("SUB A,B");
    R.A = SubtractBytes(R.A, R.B);
}

static void _SUB_C()
{
    LogInst("SUB A,C");
    R.A = SubtractBytes(R.A, R.C);
}

static void _SUB_D()
{
    LogInst("SUB A,D");
    R.A = SubtractBytes(R.A, R.D);
}

static void _SUB_E()
{
    LogInst("SUB A,E");
    R.A = SubtractBytes(R.A, R.E);
}

static void _SUB_H()
{
    LogInst("SUB A,H");
    R.A = SubtractBytes(R.A, R.H);
}

static void _SUB_L()
{
    LogInst("SUB A,L");
    R.A = SubtractBytes(R.A, R.L);
}

static void _SUB_pHL()
{
    LogInst("SUB A,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.A = SubtractBytes(R.A, n);
}

static void _SUB_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("SUB A,$%02X", u);

    R.A = SubtractBytes(R.A, u);
}

#endif // SUBTRACT_H