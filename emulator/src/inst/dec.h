#ifndef DEC_H
#define DEC_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _DEC(x)                     \
    R.FH = (((x) & 0xF) == 0);      \
    --(x);                          \
    R.FZ = ((x) == 0);              \
    R.FN = true

static void _DEC_A()
{
    LogDebug("DEC A");
    _DEC(R.A);
    tick(4);
}

static void _DEC_B()
{
    LogDebug("DEC B");
    _DEC(R.B);
    tick(4);
}

static void _DEC_C()
{
    LogDebug("DEC C");
    _DEC(R.C);
    tick(4);
}

static void _DEC_D()
{
    LogDebug("DEC D");
    _DEC(R.D);
    tick(4);
}

static void _DEC_E()
{
    LogDebug("DEC E");
    _DEC(R.E);
    tick(4);
}

static void _DEC_H()
{
    LogDebug("DEC H");
    _DEC(R.H);
    tick(4);
}

static void _DEC_L()
{
    LogDebug("DEC L");
    _DEC(R.L);
    tick(4);
}

static void _DEC_pHL()
{
    LogDebug("DEC (HL)");
    uint8_t n = readByte(R.HL);
    tick(8);

    _DEC(n);
    
    writeByte(R.HL, n);
    tick(4);
}

static void _DEC_BC()
{
    LogDebug("DEC BC");
    --R.BC;
    tick(8);
}

static void _DEC_DE()
{
    LogDebug("DEC DE");
    --R.DE;
    tick(8);
}

static void _DEC_HL()
{
    LogDebug("DEC HL");
    --R.HL;
    tick(8);
}

static void _DEC_SP()
{
    LogDebug("DEC SP");
    --R.SP;
    tick(8);
}

#endif // DEC_H