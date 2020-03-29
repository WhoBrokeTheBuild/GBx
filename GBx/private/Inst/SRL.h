#ifndef INST_SRL_H
#define INST_SRL_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _SRL_A()
{
    LogInst("SRL A");
    R.A = ShiftRight(R.A);
}

static void _SRL_B()
{
    LogInst("SRL B");
    R.B = ShiftRight(R.B);
}

static void _SRL_C()
{
    LogInst("SRL C");
    R.C = ShiftRight(R.C);
}

static void _SRL_D()
{
    LogInst("SRL D");
    R.D = ShiftRight(R.D);
}

static void _SRL_E()
{
    LogInst("SRL E");
    R.E = ShiftRight(R.E);
}

static void _SRL_H()
{
    LogInst("SRL H");
    R.H = ShiftRight(R.H);
}

static void _SRL_L()
{
    LogInst("SRL L");
    R.L = ShiftRight(R.L);
}

static void _SRL_pHL()
{
    LogInst("_SRL (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    n = ShiftRight(n);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_SRL_H