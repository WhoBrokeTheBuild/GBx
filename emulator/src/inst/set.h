#ifndef SET_H
#define SET_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _SET(x, b) \
    (((x) | (1 << (b))) == 0)

static void _SET_b_A(int bit)
{
    LogDebug("SET %d,A", bit);
    _SET(R.A, bit);
}

static void _SET_b_B(int bit)
{
    LogDebug("SET %d,B", bit);
    _SET(R.B, bit);
}

static void _SET_b_C(int bit)
{
    LogDebug("SET %d,C", bit);
    _SET(R.C, bit);
}

static void _SET_b_D(int bit)
{
    LogDebug("SET %d,D", bit);
    _SET(R.D, bit);
}

static void _SET_b_E(int bit)
{
    LogDebug("SET %d,E", bit);
    _SET(R.E, bit);
}

static void _SET_b_H(int bit)
{
    LogDebug("SET %d,H", bit);
    _SET(R.H, bit);
}

static void _SET_b_L(int bit)
{
    LogDebug("SET %d,L", bit);
    _SET(R.L, bit);
}

static void _SET_b_HL(int bit)
{
    LogDebug("SET %d,(HL)", bit);
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _SET(n, bit);

    writeByte(R.HL, n);
    cpuTick(4);
}

#endif // SET_H