#ifndef XOR_H
#define XOR_H

#include "../alu.h"
#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _XOR(x) \
    R.A = xor8(R.A, (x))

static void _XOR_A()
{
    LogDebug("XOR A");
    _XOR(R.A);
}

static void _XOR_B()
{
    LogDebug("XOR B");
    _XOR(R.B);
}

static void _XOR_C()
{
    LogDebug("XOR C");
    _XOR(R.C);
}

static void _XOR_D()
{
    LogDebug("XOR D");
    _XOR(R.D);
}

static void _XOR_E()
{
    LogDebug("XOR E");
    _XOR(R.E);
}

static void _XOR_H()
{
    LogDebug("XOR H");
    _XOR(R.H);
}

static void _XOR_L()
{
    LogDebug("XOR L");
    _XOR(R.L);
}

static void _XOR_pHL()
{
    LogDebug("XOR HL");
    uint8_t n = readByte(R.HL);
    tick(4);

    _XOR(n);
}

static void _XOR_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("XOR %02Xh", u);

    _XOR(u);
}

#undef _XOR

#endif // XOR_H