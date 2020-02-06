#ifndef INC_H
#define INC_H

#include "../alu.h"
#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _INC(x) \
    (x) = inc8((x))

static void _INC_A()
{
    LogDebug("INC A");
    _INC(R.A);
}

static void _INC_B()
{
    LogDebug("INC B");
    _INC(R.B);
}

static void _INC_C()
{
    LogDebug("INC C");
    _INC(R.C);
}

static void _INC_D()
{
    LogDebug("INC D");
    _INC(R.D);
}

static void _INC_E()
{
    LogDebug("INC E");
    _INC(R.E);
}

static void _INC_H()
{
    LogDebug("INC H");
    _INC(R.H);
}

static void _INC_L()
{
    LogDebug("INC L");
    _INC(R.L);
}

static void _INC_pHL()
{
    LogDebug("INC (HL)");
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _INC(n);
    
    writeByte(R.HL, n);
    cpuTick(4);
}

static void _INC_BC()
{
    LogDebug("INC BC");
    ++R.BC;
    cpuTick(4);
}

static void _INC_DE()
{
    LogDebug("INC DE");
    ++R.DE;
    cpuTick(4);
}

static void _INC_HL()
{
    LogDebug("INC HL");
    ++R.HL;
    cpuTick(4);
}

static void _INC_SP()
{
    LogDebug("INC SP");
    ++R.SP;
    cpuTick(4);
}

#endif // INC_H