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
    LogVerbose(3, "DEC A");
    _DEC(R.A);
    tick(4);
}

static void _DEC_B()
{
    LogVerbose(3, "DEC B");
    _DEC(R.B);
    tick(4);
}

static void _DEC_C()
{
    LogVerbose(3, "DEC C");
    _DEC(R.C);
    tick(4);
}

static void _DEC_D()
{
    LogVerbose(3, "DEC D");
    _DEC(R.D);
    tick(4);
}

static void _DEC_E()
{
    LogVerbose(3, "DEC E");
    _DEC(R.E);
    tick(4);
}

static void _DEC_H()
{
    LogVerbose(3, "DEC H");
    _DEC(R.H);
    tick(4);
}

static void _DEC_L()
{
    LogVerbose(3, "DEC L");
    _DEC(R.L);
    tick(4);
}

static void _DEC_pHL()
{
    LogVerbose(3, "DEC (HL)");
    uint8_t n = readByte(R.HL);
    tick(8);

    _DEC(n);
    
    writeByte(R.HL, n);
    tick(4);
}

static void _DEC_BC()
{
    LogVerbose(3, "DEC BC");
    --R.BC;
    tick(8);
}

static void _DEC_DE()
{
    LogVerbose(3, "DEC DE");
    --R.DE;
    tick(8);
}

static void _DEC_HL()
{
    LogVerbose(3, "DEC HL");
    --R.HL;
    tick(8);
}

static void _DEC_SP()
{
    LogVerbose(3, "DEC SP");
    --R.SP;
    tick(8);
}

#endif // DEC_H