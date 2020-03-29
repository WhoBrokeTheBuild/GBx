#ifndef INST_RES_H
#define INST_RES_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _RES_b_A(int bit)
{
    LogInst("RES %d,A", bit);
    R.A = ResetBit(R.A, bit);
}

static void _RES_b_B(int bit)
{
    LogInst("RES %d,B", bit);
    R.B = ResetBit(R.B, bit);
}

static void _RES_b_C(int bit)
{
    LogInst("RES %d,C", bit);
    R.C = ResetBit(R.C, bit);
}

static void _RES_b_D(int bit)
{
    LogInst("RES %d,D", bit);
    R.D = ResetBit(R.D, bit);
}

static void _RES_b_E(int bit)
{
    LogInst("RES %d,E", bit);
    R.E = ResetBit(R.E, bit);
}

static void _RES_b_H(int bit)
{
    LogInst("RES %d,H", bit);
    R.H = ResetBit(R.H, bit);
}

static void _RES_b_L(int bit)
{
    LogInst("RES %d,L", bit);
    R.L = ResetBit(R.L, bit);
}

static void _RES_b_pHL(int bit)
{
    LogInst("RES %d,(HL)", bit);
    byte n = ReadByte(R.HL);
    Tick(4);

    n = ResetBit(n, bit);

    WriteByte(R.HL, n);
    Tick(4);
}

#endif // INST_RES_H