#ifndef INC_H
#define INC_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _INC(x)                     \
    R.FH = (((x) & 0x0F) == 0x0F);  \
    ++(x);                          \
    R.FZ = ((x) == 0);              \
    R.FN = false

static void _INC_A()
{
    LogVerbose(3, "INC A");
    _INC(R.A);
    tick(4);
}

static void _INC_B()
{
    LogVerbose(3, "INC B");
    _INC(R.B);
    tick(4);
}

static void _INC_C()
{
    LogVerbose(3, "INC C");
    _INC(R.C);
    tick(4);
}

static void _INC_D()
{
    LogVerbose(3, "INC D");
    _INC(R.D);
    tick(4);
}

static void _INC_E()
{
    LogVerbose(3, "INC E");
    _INC(R.E);
    tick(4);
}

static void _INC_H()
{
    LogVerbose(3, "INC H");
    _INC(R.H);
    tick(4);
}

static void _INC_L()
{
    LogVerbose(3, "INC L");
    _INC(R.L);
    tick(4);
}

static void _INC_pHL()
{
    LogVerbose(3, "INC (HL)");
    uint8_t n = readByte(R.HL);
    tick(8);

    _INC(n);
    
    writeByte(R.HL, n);
    tick(4);
}

static void _INC_BC()
{
    LogVerbose(3, "INC BC");
    ++R.BC;
    tick(8);
}

static void _INC_DE()
{
    LogVerbose(3, "INC DE");
    ++R.DE;
    tick(8);
}

static void _INC_HL()
{
    LogVerbose(3, "INC HL");
    ++R.HL;
    tick(8);
}

static void _INC_SP()
{
    LogVerbose(3, "INC SP");
    ++R.SP;
    tick(8);
}

#endif // INC_H