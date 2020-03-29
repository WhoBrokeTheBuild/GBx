#ifndef INST_SRA_H
#define INST_SRA_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _SRA_A()
{
    LogInst("SRA A");
    R.A = ShiftRightWithCarry(R.A);
}

static void _SRA_B()
{
    LogInst("SRA B");
    R.B = ShiftRightWithCarry(R.B);
}

static void _SRA_C()
{
    LogInst("SRA C");
    R.C = ShiftRightWithCarry(R.C);
}

static void _SRA_D()
{
    LogInst("SRA D");
    R.D = ShiftRightWithCarry(R.D);
}

static void _SRA_E()
{
    LogInst("SRA E");
    R.E = ShiftRightWithCarry(R.E);
}

static void _SRA_H()
{
    LogInst("SRA H");
    R.H = ShiftRightWithCarry(R.H);
}

static void _SRA_L()
{
    LogInst("SRA L");
    R.L = ShiftRightWithCarry(R.L);
}

static void _SRA_pHL()
{
    LogInst("_SRA (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    n = ShiftRightWithCarry(n);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_SRA_H