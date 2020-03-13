#ifndef INC_H
#define INC_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _INC(x)                     \
    R.FH = (((x) & 0x0F) == 0x0F);  \
    ++(x);                          \
    R.FZ = ((x) == 0);              \
    R.FN = false

static void _INC_A()
{
    LogInst("INC A");
    _INC(R.A);
    Tick(4);
}

static void _INC_B()
{
    LogInst("INC B");
    _INC(R.B);
    Tick(4);
}

static void _INC_C()
{
    LogInst("INC C");
    _INC(R.C);
    Tick(4);
}

static void _INC_D()
{
    LogInst("INC D");
    _INC(R.D);
    Tick(4);
}

static void _INC_E()
{
    LogInst("INC E");
    _INC(R.E);
    Tick(4);
}

static void _INC_H()
{
    LogInst("INC H");
    _INC(R.H);
    Tick(4);
}

static void _INC_L()
{
    LogInst("INC L");
    _INC(R.L);
    Tick(4);
}

static void _INC_pHL()
{
    LogInst("INC (HL)");
    byte n = ReadByte(R.HL);
    Tick(8);

    _INC(n);
    
    WriteByte(R.HL, n);
    Tick(4);
}

static void _INC_BC()
{
    LogInst("INC BC");
    ++R.BC;
    Tick(8);
}

static void _INC_DE()
{
    LogInst("INC DE");
    ++R.DE;
    Tick(8);
}

static void _INC_HL()
{
    LogInst("INC HL");
    ++R.HL;
    Tick(8);
}

static void _INC_SP()
{
    LogInst("INC SP");
    ++R.SP;
    Tick(8);
}

#endif // INC_H