#ifndef INST_RL_H
#define INST_RL_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _RL_A()
{
    LogInst("RL A");
    R.A = RotateLeft(R.A);
}

static void _RL_B()
{
    LogInst("RL B");
    R.B = RotateLeft(R.B);
}

static void _RL_C()
{
    LogInst("RL C");
    R.C = RotateLeft(R.C);
}

static void _RL_D()
{
    LogInst("RL D");
    R.D = RotateLeft(R.D);
}

static void _RL_E()
{
    LogInst("RL E");
    R.E = RotateLeft(R.E);
}

static void _RL_H()
{
    LogInst("RL H");
    R.H = RotateLeft(R.H);
}

static void _RL_L()
{
    LogInst("RL L");
    R.L = RotateLeft(R.L);
}

static void _RL_pHL()
{
    LogInst("RL (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    n = RotateLeft(n);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_RL_H