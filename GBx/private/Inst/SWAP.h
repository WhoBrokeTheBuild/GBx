#ifndef INST_SWAP_H
#define INST_SWAP_H

#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

#define _SWAP(x) \
    (x) = SwapNibbles((x))
    
static void _SWAP_A()
{
    LogInst("SWAP A");
    _SWAP(R.A);
    Tick(4);
}

static void _SWAP_B()
{
    LogInst("SWAP B");
    _SWAP(R.B);
    Tick(4);
}

static void _SWAP_C()
{
    LogInst("SWAP C");
    _SWAP(R.C);
    Tick(4);
}

static void _SWAP_D()
{
    LogInst("SWAP D");
    _SWAP(R.D);
    Tick(4);
}

static void _SWAP_E()
{
    LogInst("SWAP E");
    _SWAP(R.E);
    Tick(4);
}

static void _SWAP_H()
{
    LogInst("SWAP H");
    _SWAP(R.H);
    Tick(4);
}

static void _SWAP_L()
{
    LogInst("SWAP L");
    _SWAP(R.L);
    Tick(4);
}

static void _SWAP_pHL()
{
    LogInst("SWAP (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    _SWAP(n);
    Tick(4);

    WriteByte(R.HL, n);
    Tick(4);
}

#undef _SWAP

#endif // INST_SWAP_H