#ifndef INST_AND_H
#define INST_AND_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _AND_A()
{
    LogInst("AND A");
    R.A = AndBytes(R.A, R.A);
}

static void _AND_B()
{
    LogInst("AND B");
    R.A = AndBytes(R.A, R.B);
}

static void _AND_C()
{
    LogInst("AND C");
    R.A = AndBytes(R.A, R.C);
}

static void _AND_D()
{
    LogInst("AND D");
    R.A = AndBytes(R.A, R.D);
}

static void _AND_E()
{
    LogInst("AND E");
    R.A = AndBytes(R.A, R.E);
}

static void _AND_H()
{
    LogInst("AND H");
    R.A = AndBytes(R.A, R.H);
}

static void _AND_L()
{
    LogInst("AND L");
    R.A = AndBytes(R.A, R.L);
}

static void _AND_pHL()
{
    LogInst("AND HL");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.A = AndBytes(R.A, n);
}

static void _AND_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("AND $%02X", u);

    R.A = AndBytes(R.A, u);
}

#endif // INST_AND_H