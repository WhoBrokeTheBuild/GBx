#ifndef INST_RRC_H
#define INST_RRC_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _RRC_A()
{
    LogInst("RRC A");
    R.A = RotateRightWithCarry(R.A);
}

static void _RRC_B()
{
    LogInst("RRC B");
    R.B = RotateRightWithCarry(R.B);
}

static void _RRC_C()
{
    LogInst("RRC C");
    R.C = RotateRightWithCarry(R.C);
}

static void _RRC_D()
{
    LogInst("RRC D");
    R.D = RotateRightWithCarry(R.D);
}

static void _RRC_E()
{
    LogInst("RRC E");
    R.E = RotateRightWithCarry(R.E);
}

static void _RRC_H()
{
    LogInst("RRC H");
    R.H = RotateRightWithCarry(R.H);
}

static void _RRC_L()
{
    LogInst("RRC L");
    R.L = RotateRightWithCarry(R.L);
}

static void _RRC_pHL()
{
    LogInst("RRC (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    n = RotateRightWithCarry(n);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_RRC_H