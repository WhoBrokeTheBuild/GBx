#ifndef XOR_H
#define XOR_H

#include "alu.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _XOR(x) \
    R.A = xor8(R.A, (x))

static void _XOR_A()
{
    LogInst("XOR A");
    _XOR(R.A);
}

static void _XOR_B()
{
    LogInst("XOR B");
    _XOR(R.B);
}

static void _XOR_C()
{
    LogInst("XOR C");
    _XOR(R.C);
}

static void _XOR_D()
{
    LogInst("XOR D");
    _XOR(R.D);
}

static void _XOR_E()
{
    LogInst("XOR E");
    _XOR(R.E);
}

static void _XOR_H()
{
    LogInst("XOR H");
    _XOR(R.H);
}

static void _XOR_L()
{
    LogInst("XOR L");
    _XOR(R.L);
}

static void _XOR_pHL()
{
    LogInst("XOR HL");
    byte n = ReadByte(R.HL);
    Tick(4);

    _XOR(n);
}

static void _XOR_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("XOR %02Xh", u);

    _XOR(u);
}

#undef _XOR

#endif // XOR_H