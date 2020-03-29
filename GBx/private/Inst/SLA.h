#ifndef INST_SLA_H
#define INST_SLA_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _SLA_A()
{
    LogInst("SLA A");
    R.A = ShiftLeftWithCarry(R.A);
}

static void _SLA_B()
{
    LogInst("SLA B");
    R.B = ShiftLeftWithCarry(R.B);
}

static void _SLA_C()
{
    LogInst("SLA C");
    R.C = ShiftLeftWithCarry(R.C);
}

static void _SLA_D()
{
    LogInst("SLA D");
    R.D = ShiftLeftWithCarry(R.D);
}

static void _SLA_E()
{
    LogInst("SLA E");
    R.E = ShiftLeftWithCarry(R.E);
}

static void _SLA_H()
{
    LogInst("SLA H");
    R.H = ShiftLeftWithCarry(R.H);
}

static void _SLA_L()
{
    LogInst("SLA L");
    R.L = ShiftLeftWithCarry(R.L);
}

static void _SLA_pHL()
{
    LogInst("SLA (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    n = ShiftLeftWithCarry(n);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_SLA_H