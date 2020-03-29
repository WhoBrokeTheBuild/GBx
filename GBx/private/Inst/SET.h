#ifndef INST_SET_H
#define INST_SET_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

#define _SET(x, bit) \
    (x) = SetBit((x), bit)

static void _SET_b_A(int bit)
{
    LogInst("SET %d,A", bit);
    _SET(R.A, bit);
}

static void _SET_b_B(int bit)
{
    LogInst("SET %d,B", bit);
    _SET(R.B, bit);
}

static void _SET_b_C(int bit)
{
    LogInst("SET %d,C", bit);
    _SET(R.C, bit);
}

static void _SET_b_D(int bit)
{
    LogInst("SET %d,D", bit);
    _SET(R.D, bit);
}

static void _SET_b_E(int bit)
{
    LogInst("SET %d,E", bit);
    _SET(R.E, bit);
}

static void _SET_b_H(int bit)
{
    LogInst("SET %d,H", bit);
    _SET(R.H, bit);
}

static void _SET_b_L(int bit)
{
    LogInst("SET %d,L", bit);
    _SET(R.L, bit);
}

static void _SET_b_pHL(int bit)
{
    LogInst("SET %d,(HL)", bit);
    byte n = ReadByte(R.HL);
    Tick(4);

    _SET(n, bit);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_SET_H