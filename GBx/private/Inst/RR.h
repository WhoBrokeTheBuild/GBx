#ifndef INST_RR_H
#define INST_RR_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _RR_A()
{
    LogInst("RR A");
    R.A = RotateRight(R.A);
}

static void _RR_B()
{
    LogInst("RR B");
    R.B = RotateRight(R.B);
}

static void _RR_C()
{
    LogInst("RR C");
    R.C = RotateRight(R.C);
}

static void _RR_D()
{
    LogInst("RR D");
    R.D = RotateRight(R.D);
}

static void _RR_E()
{
    LogInst("RR E");
    R.E = RotateRight(R.E);
}

static void _RR_H()
{
    LogInst("RR H");
    R.H = RotateRight(R.H);
}

static void _RR_L()
{
    LogInst("RR L");
    R.L = RotateRight(R.L);
}

static void _RR_pHL()
{
    LogInst("RR (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    n = RotateRight(n);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_RR_H