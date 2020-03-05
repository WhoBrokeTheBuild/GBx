#ifndef DEC_H
#define DEC_H

#include "../clock.h"
#include "../cpu.h"
#include "../log.h"
#include "../memory.h"

#define _DEC(x)                     \
    R.FH = (((x) & 0xF) == 0);      \
    --(x);                          \
    R.FZ = ((x) == 0);              \
    R.FN = true

static void _DEC_A()
{
    LogInst("DEC A");
    _DEC(R.A);
    tick(4);
}

static void _DEC_B()
{
    LogInst("DEC B");
    _DEC(R.B);
    tick(4);
}

static void _DEC_C()
{
    LogInst("DEC C");
    _DEC(R.C);
    tick(4);
}

static void _DEC_D()
{
    LogInst("DEC D");
    _DEC(R.D);
    tick(4);
}

static void _DEC_E()
{
    LogInst("DEC E");
    _DEC(R.E);
    tick(4);
}

static void _DEC_H()
{
    LogInst("DEC H");
    _DEC(R.H);
    tick(4);
}

static void _DEC_L()
{
    LogInst("DEC L");
    _DEC(R.L);
    tick(4);
}

static void _DEC_pHL()
{
    LogInst("DEC (HL)");
    uint8_t n = readByte(R.HL);
    tick(8);

    _DEC(n);
    
    writeByte(R.HL, n);
    tick(4);
}

static void _DEC_BC()
{
    LogInst("DEC BC");
    --R.BC;
    tick(8);
}

static void _DEC_DE()
{
    LogInst("DEC DE");
    --R.DE;
    tick(8);
}

static void _DEC_HL()
{
    LogInst("DEC HL");
    --R.HL;
    tick(8);
}

static void _DEC_SP()
{
    LogInst("DEC SP");
    --R.SP;
    tick(8);
}

#endif // DEC_H