#ifndef INST_RLC_H
#define INST_RLC_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _RLC_A()
{
    LogInst("RLC A");
    R.A = RotateLeftWithCarry(R.A);
}

static void _RLC_B()
{
    LogInst("RLC B");
    R.B = RotateLeftWithCarry(R.B);
}

static void _RLC_C()
{
    LogInst("RLC C");
    R.C = RotateLeftWithCarry(R.C);
}

static void _RLC_D()
{
    LogInst("RLC D");
    R.D = RotateLeftWithCarry(R.D);
}

static void _RLC_E()
{
    LogInst("RLC E");
    R.E = RotateLeftWithCarry(R.E);
}

static void _RLC_H()
{
    LogInst("RLC H");
    R.H = RotateLeftWithCarry(R.H);
}

static void _RLC_L()
{
    LogInst("RLC L");
    R.L = RotateLeftWithCarry(R.L);
}

static void _RLC_pHL()
{
    LogInst("RLC (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    n = RotateLeftWithCarry(n);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_RLC_H