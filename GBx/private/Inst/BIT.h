#ifndef INST_BIT_H
#define INST_BIT_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _BIT_b_A(int bit)
{
    LogInst("BIT %d,A", bit);
    TestBit(R.A, bit);
}

static void _BIT_b_B(int bit)
{
    LogInst("BIT %d,B", bit);
    TestBit(R.B, bit);
}

static void _BIT_b_C(int bit)
{
    LogInst("BIT %d,C", bit);
    TestBit(R.C, bit);
}

static void _BIT_b_D(int bit)
{
    LogInst("BIT %d,D", bit);
    TestBit(R.D, bit);
}

static void _BIT_b_E(int bit)
{
    LogInst("BIT %d,E", bit);
    TestBit(R.E, bit);
}

static void _BIT_b_H(int bit)
{
    LogInst("BIT %d,H", bit);
    TestBit(R.H, bit);
}

static void _BIT_b_L(int bit)
{
    LogInst("BIT %d,L", bit);
    TestBit(R.L, bit);
}

static void _BIT_b_pHL(int bit)
{
    LogInst("BIT %d,(HL)", bit);
    byte n = ReadByte(R.HL);
    Tick(4);

    TestBit(n, bit);
}

#endif // INST_BIT_H