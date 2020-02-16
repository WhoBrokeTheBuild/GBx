#ifndef DEC_H
#define DEC_H

#include "../cpu.h"
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
    cpuTick(4);
}

static void _DEC_B()
{
    LogDebug("DEC B");
    _DEC(R.B);
    cpuTick(4);
}

static void _DEC_C()
{
    LogDebug("DEC C");
    _DEC(R.C);
    cpuTick(4);
}

static void _DEC_D()
{
    LogDebug("DEC D");
    _DEC(R.D);
    cpuTick(4);
}

static void _DEC_E()
{
    LogDebug("DEC E");
    _DEC(R.E);
    cpuTick(4);
}

static void _DEC_H()
{
    LogDebug("DEC H");
    _DEC(R.H);
    cpuTick(4);
}

static void _DEC_L()
{
    LogDebug("DEC L");
    _DEC(R.L);
    cpuTick(4);
}

static void _DEC_pHL()
{
    LogDebug("DEC (HL)");
    uint8_t n = readByte(R.HL);
    cpuTick(8);

    _DEC(n);
    
    writeByte(R.HL, n);
    cpuTick(4);
}

static void _DEC_BC()
{
    LogDebug("DEC BC");
    --R.BC;
    cpuTick(8);
}

static void _DEC_DE()
{
    LogDebug("DEC DE");
    --R.DE;
    cpuTick(8);
}

static void _DEC_HL()
{
    LogDebug("DEC HL");
    --R.HL;
    cpuTick(8);
}

static void _DEC_SP()
{
    LogDebug("DEC SP");
    --R.SP;
    cpuTick(8);
}

#endif // DEC_H