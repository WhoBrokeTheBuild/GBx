#ifndef RESET_H
#define RESET_H

#include "../cpu.h"
#include "../log.h"
#include "../register.h"

#define _RES(x, b) \
    (((x) & ~(1 << (b))) == 0)

static void _RES_b_A(int bit)
{
    LogDebug("RES %d,A", bit);
    _RES(R.A, bit);
}

static void _RES_b_B(int bit)
{
    LogDebug("RES %d,B", bit);
    _RES(R.B, bit);
}

static void _RES_b_C(int bit)
{
    LogDebug("RES %d,C", bit);
    _RES(R.C, bit);
}

static void _RES_b_D(int bit)
{
    LogDebug("RES %d,D", bit);
    _RES(R.D, bit);
}

static void _RES_b_E(int bit)
{
    LogDebug("RES %d,E", bit);
    _RES(R.E, bit);
}

static void _RES_b_H(int bit)
{
    LogDebug("RES %d,H", bit);
    _RES(R.H, bit);
}

static void _RES_b_L(int bit)
{
    LogDebug("RES %d,L", bit);
    _RES(R.L, bit);
}

static void _RES_b_HL(int bit)
{
    LogDebug("RES %d,(HL)", bit);
    uint8_t n = readByte(R.HL);
    tick(4);

    _RES(n, bit);

    writeByte(R.HL, n);
    tick(4);
}

#endif // RESET_H