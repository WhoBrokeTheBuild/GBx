#ifndef SET_H
#define SET_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _SET(x, b) \
    (x) = ((x) | (1 << (b)))

static void _SET_b_A(int bit)
{
    LogVerbose(3, "SET %d,A", bit);
    _SET(R.A, bit);
}

static void _SET_b_B(int bit)
{
    LogVerbose(3, "SET %d,B", bit);
    _SET(R.B, bit);
}

static void _SET_b_C(int bit)
{
    LogVerbose(3, "SET %d,C", bit);
    _SET(R.C, bit);
}

static void _SET_b_D(int bit)
{
    LogVerbose(3, "SET %d,D", bit);
    _SET(R.D, bit);
}

static void _SET_b_E(int bit)
{
    LogVerbose(3, "SET %d,E", bit);
    _SET(R.E, bit);
}

static void _SET_b_H(int bit)
{
    LogVerbose(3, "SET %d,H", bit);
    _SET(R.H, bit);
}

static void _SET_b_L(int bit)
{
    LogVerbose(3, "SET %d,L", bit);
    _SET(R.L, bit);
}

static void _SET_b_pHL(int bit)
{
    LogVerbose(3, "SET %d,(HL)", bit);
    uint8_t n = readByte(R.HL);
    tick(4);

    _SET(n, bit);

    writeByte(R.HL, n);
    tick(4);
}

#endif // SET_H