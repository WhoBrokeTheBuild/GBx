#ifndef DEC_H
#define DEC_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _DEC(x)                     \
    R.FH = (((x) & 0xF) == 0);      \
    --(x);                          \
    R.FZ = ((x) == 0);              \
    R.FN = true

static void _DEC_A()
{
    LogInst("DEC A");
    _DEC(R.A);
    Tick(4);
}

static void _DEC_B()
{
    LogInst("DEC B");
    _DEC(R.B);
    Tick(4);
}

static void _DEC_C()
{
    LogInst("DEC C");
    _DEC(R.C);
    Tick(4);
}

static void _DEC_D()
{
    LogInst("DEC D");
    _DEC(R.D);
    Tick(4);
}

static void _DEC_E()
{
    LogInst("DEC E");
    _DEC(R.E);
    Tick(4);
}

static void _DEC_H()
{
    LogInst("DEC H");
    _DEC(R.H);
    Tick(4);
}

static void _DEC_L()
{
    LogInst("DEC L");
    _DEC(R.L);
    Tick(4);
}

static void _DEC_pHL()
{
    LogInst("DEC (HL)");
    byte n = ReadByte(R.HL);
    Tick(8);

    _DEC(n);
    
    WriteByte(R.HL, n);
    Tick(4);
}

static void _DEC_BC()
{
    LogInst("DEC BC");
    --R.BC;
    Tick(8);
}

static void _DEC_DE()
{
    LogInst("DEC DE");
    --R.DE;
    Tick(8);
}

static void _DEC_HL()
{
    LogInst("DEC HL");
    --R.HL;
    Tick(8);
}

static void _DEC_SP()
{
    LogInst("DEC SP");
    --R.SP;
    Tick(8);

    StackBaseAddress = R.SP;
}

#endif // DEC_H