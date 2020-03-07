#ifndef SWAP_H
#define SWAP_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _SWAP(x)                        \
    (x) = (((x) & 0x0F) << 4)           \
        | (((x) & 0xF0) >> 4);          \
    R.FZ = ((x) == 0);                  \
    R.FN = false;                       \
    R.FH = false;                       \
    R.FC = false;

static void _SWAP_A()
{
    LogInst("SWAP A");
    _SWAP(R.A);
    tick(4);
}

static void _SWAP_B()
{
    LogInst("SWAP B");
    _SWAP(R.B);
    tick(4);
}

static void _SWAP_C()
{
    LogInst("SWAP C");
    _SWAP(R.C);
    tick(4);
}

static void _SWAP_D()
{
    LogInst("SWAP D");
    _SWAP(R.D);
    tick(4);
}

static void _SWAP_E()
{
    LogInst("SWAP E");
    _SWAP(R.E);
    tick(4);
}

static void _SWAP_H()
{
    LogInst("SWAP H");
    _SWAP(R.H);
    tick(4);
}

static void _SWAP_L()
{
    LogInst("SWAP L");
    _SWAP(R.L);
    tick(4);
}

static void _SWAP_pHL()
{
    LogInst("SWAP (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SWAP(n);
    tick(4);

    writeByte(R.HL, n);
    tick(4);
}

#undef _SWAP

#endif // SWAP_H