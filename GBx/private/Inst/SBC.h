#ifndef INST_SBC_H
#define INST_SBC_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _SBC_A()
{
    LogInst("SBC A,A");
    R.A = SubtractBytes(R.A, R.A + (int)R.FC);
}

static void _SBC_B()
{
    LogInst("SBC A,B");
    R.A = SubtractBytes(R.A, R.B + (int)R.FC);
}

static void _SBC_C()
{
    LogInst("SBC A,C");
    R.A = SubtractBytes(R.A, R.C + (int)R.FC);
}

static void _SBC_D()
{
    LogInst("SBC A,D");
    R.A = SubtractBytes(R.A, R.D + (int)R.FC);
}

static void _SBC_E()
{
    LogInst("SBC A,E");
    R.A = SubtractBytes(R.A, R.E + (int)R.FC);
}

static void _SBC_H()
{
    LogInst("SBC A,H");
    R.A = SubtractBytes(R.A, R.H + (int)R.FC);
}

static void _SBC_L()
{
    LogInst("SBC A,L");
    R.A = SubtractBytes(R.A, R.L + (int)R.FC);
}

static void _SBC_pHL()
{
    LogInst("SBC A,(HL)");
    byte pHL = ReadByte(R.HL);
    Tick(4);

    R.A = SubtractBytes(R.A, pHL + (int)R.FC);
}

static void _SBC_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("SBC A,$%02X", u);

    R.A = SubtractBytes(R.A, u + (int)R.FC);
}

#endif // INST_SBC_H