#ifndef INST_OR_H
#define INST_OR_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _OR_A()
{
    LogInst("OR A");
    R.A = OrBytes(R.A, R.A);
}

static void _OR_B()
{
    LogInst("OR B");
    R.A = OrBytes(R.A, R.B);
}

static void _OR_C()
{
    LogInst("OR C");
    R.A = OrBytes(R.A, R.C);
}

static void _OR_D()
{
    LogInst("OR D");
    R.A = OrBytes(R.A, R.D);
}

static void _OR_E()
{
    LogInst("OR E");
    R.A = OrBytes(R.A, R.E);
}

static void _OR_H()
{
    LogInst("OR H");
    R.A = OrBytes(R.A, R.H);
}

static void _OR_L()
{
    LogInst("OR L");
    R.A = OrBytes(R.A, R.L);
}

static void _OR_pHL()
{
    LogInst("OR HL");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.A = OrBytes(R.A, n);
}

static void _OR_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("OR $%02X", u);

    R.A = OrBytes(R.A, u);
}

#endif // INST_OR_H