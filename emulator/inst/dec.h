#ifndef DEC_H
#define DEC_H

#include "../alu.h"
#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _DEC(x) \
    (x) = dec8((x))

static void _DEC_A()
{
    LogDebug("DEC A");
    _DEC(R.A);
}

static void _DEC_B()
{
    LogDebug("DEC B");
    _DEC(R.B);
}

static void _DEC_C()
{
    LogDebug("DEC C");
    _DEC(R.C);
}

static void _DEC_D()
{
    LogDebug("DEC D");
    _DEC(R.D);
}

static void _DEC_E()
{
    LogDebug("DEC E");
    _DEC(R.E);
}

static void _DEC_H()
{
    LogDebug("DEC H");
    _DEC(R.H);
}

static void _DEC_L()
{
    LogDebug("DEC L");
    _DEC(R.L);
}

static void _DEC_pHL()
{
    LogDebug("DEC (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _DEC(n);
    
    writeByte(R.HL, n);
    tick(4);
}

static void _DEC_BC()
{
    LogDebug("DEC BC");
    --R.BC;
    tick(4);
}

static void _DEC_DE()
{
    LogDebug("DEC DE");
    --R.DE;
    tick(4);
}

static void _DEC_HL()
{
    LogDebug("DEC HL");
    --R.HL;
    tick(4);
}

static void _DEC_SP()
{
    LogDebug("DEC SP");
    --R.SP;
    tick(4);
}

#endif // DEC_H