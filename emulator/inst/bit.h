#ifndef BIT_H
#define BIT_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _BIT(x, b) \
    R.FZ = (((x) & (1 << (b))) == 0); \
    R.FN = false; \
    R.FH = true

static void _BIT_b_A(int bit)
{
    LogDebug("BIT %d,A", bit);
    _BIT(R.A, bit);
}

static void _BIT_b_B(int bit)
{
    LogDebug("BIT %d,B", bit);
    _BIT(R.B, bit);
}

static void _BIT_b_C(int bit)
{
    LogDebug("BIT %d,C", bit);
    _BIT(R.C, bit);
}

static void _BIT_b_D(int bit)
{
    LogDebug("BIT %d,D", bit);
    _BIT(R.D, bit);
}

static void _BIT_b_E(int bit)
{
    LogDebug("BIT %d,E", bit);
    _BIT(R.E, bit);
}

static void _BIT_b_H(int bit)
{
    LogDebug("BIT %d,H", bit);
    _BIT(R.H, bit);
}

static void _BIT_b_L(int bit)
{
    LogDebug("BIT %d,L", bit);
    _BIT(R.L, bit);
}

static void _BIT_b_HL(int bit)
{
    LogDebug("BIT %d,(HL)", bit);
    uint8_t n = readByte(R.HL);
    TickCounter += 4;

    _BIT(n, bit);
}

#undef _BIT

#endif // BIT_H