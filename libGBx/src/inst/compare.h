#ifndef COMPARE_H
#define COMPARE_H

#include "alu.h"
#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _CP(x) \
    sub8(R.A, (x))

static void _CP_A()
{
    LogInst("CP A");
    _CP(R.A);
}

static void _CP_B()
{
    LogInst("CP B");
    _CP(R.B);
}

static void _CP_C()
{
    LogInst("CP C");
    _CP(R.C);
}

static void _CP_D()
{
    LogInst("CP D");
    _CP(R.D);
}

static void _CP_E()
{
    LogInst("CP E");
    _CP(R.E);
}

static void _CP_H()
{
    LogInst("CP H");
    _CP(R.H);
}

static void _CP_L()
{
    LogInst("CP L");
    _CP(R.L);
}

static void _CP_pHL()
{
    LogInst("CP (HL)");
    uint8_t pHL = readByte(R.HL);
    tick(4);

    _CP(pHL);
}

static void _CP_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogInst("CP %02Xh", u);

    _CP(u);
}

#undef _CP

#endif // COMPARE_H