#ifndef INST_XOR_H
#define INST_XOR_H

#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _XOR_A()
{
    LogInst("XOR A");
    R.A = XorBytes(R.A, R.A);
}

static void _XOR_B()
{
    LogInst("XOR B");
    R.A = XorBytes(R.A, R.B);
}

static void _XOR_C()
{
    LogInst("XOR C");
    R.A = XorBytes(R.A, R.C);
}

static void _XOR_D()
{
    LogInst("XOR D");
    R.A = XorBytes(R.A, R.D);
}

static void _XOR_E()
{
    LogInst("XOR E");
    R.A = XorBytes(R.A, R.E);
}

static void _XOR_H()
{
    LogInst("XOR H");
    R.A = XorBytes(R.A, R.H);
}

static void _XOR_L()
{
    LogInst("XOR L");
    R.A = XorBytes(R.A, R.L);
}

static void _XOR_pHL()
{
    LogInst("XOR HL");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.A = XorBytes(R.A, n);
}

static void _XOR_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("XOR $%02X", u);

    R.A = XorBytes(R.A, u);
}

#endif // INST_XOR_H