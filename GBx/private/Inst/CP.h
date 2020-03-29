#ifndef INST_CP_H
#define INST_CP_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _CP_A()
{
    LogInst("CP A");
    SubtractBytes(R.A, R.A);
}

static void _CP_B()
{
    LogInst("CP B");
    SubtractBytes(R.A, R.B);
}

static void _CP_C()
{
    LogInst("CP C");
    SubtractBytes(R.A, R.C);
}

static void _CP_D()
{
    LogInst("CP D");
    SubtractBytes(R.A, R.D);
}

static void _CP_E()
{
    LogInst("CP E");
    SubtractBytes(R.A, R.E);
}

static void _CP_H()
{
    LogInst("CP H");
    SubtractBytes(R.A, R.H);
}

static void _CP_L()
{
    LogInst("CP L");
    SubtractBytes(R.A, R.L);
}

static void _CP_pHL()
{
    LogInst("CP (HL)");
    byte pHL = ReadByte(R.HL);
    Tick(4);

    SubtractBytes(R.A, pHL);
}

static void _CP_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("CP $%02X", u);

    SubtractBytes(R.A, u);
}

#undef _CP

#endif // COMPARE_H