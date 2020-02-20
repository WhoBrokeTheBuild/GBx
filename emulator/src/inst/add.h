#ifndef ADD_H
#define ADD_H

#include "../alu.h"
#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _ADD(x) \
    R.A = add8(R.A, (x))

#define _ADC(x) \
    R.A = add8(R.A, (x) + (uint8_t)R.FC)

#define _ADDHL(x) \
    R.HL = add16(R.HL, (x))
    
static void _ADD_A()
{
    LogDebug("ADD A,A");
    _ADD(R.A);
}

static void _ADD_B()
{
    LogDebug("ADD A,B");
    _ADD(R.B);
}

static void _ADD_C()
{
    LogDebug("ADD A,C");
    _ADD(R.C);
}

static void _ADD_D()
{
    LogDebug("ADD A,D");
    _ADD(R.D);
}

static void _ADD_E()
{
    LogDebug("ADD A,E");
    _ADD(R.E);
}

static void _ADD_H()
{
    LogDebug("ADD A,H");
    _ADD(R.H);
}

static void _ADD_L()
{
    LogDebug("ADD A,L");
    _ADD(R.L);
}

static void _ADD_pHL()
{
    LogDebug("ADD A,(HL)");
    uint8_t pHL = readByte(R.HL);
    tick(4);

    _ADD(pHL);
}

static void _ADD_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("ADD A,%02Xh", u);

    _ADD(u);
}

static void _ADC_A()
{
    LogDebug("ADC A,A");
    _ADC(R.A);
}

static void _ADC_B()
{
    LogDebug("ADC A,B");
    _ADC(R.B);
}

static void _ADC_C()
{
    LogDebug("ADC A,C");
    _ADC(R.C);
}

static void _ADC_D()
{
    LogDebug("ADC A,D");
    _ADC(R.D);
}

static void _ADC_E()
{
    LogDebug("ADC A,E");
    _ADC(R.E);
}

static void _ADC_H()
{
    LogDebug("ADC A,H");
    _ADC(R.H);
}

static void _ADC_L()
{
    LogDebug("ADC A,L");
    _ADC(R.L);
}

static void _ADC_pHL()
{
    LogDebug("ADC A,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _ADC(n);
}

static void _ADC_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("ADC A,%02Xh", u);

    _ADC(u);
}

static void _ADD_HL_BC()
{
    LogDebug("ADD HL,BC");
    _ADDHL(R.BC);
    tick(4);
}

static void _ADD_HL_DE()
{
    LogDebug("ADD HL,DE");
    _ADDHL(R.DE);
    tick(4);
}

static void _ADD_HL_HL()
{
    LogDebug("ADD HL,HL");
    _ADDHL(R.HL);
    tick(4);
}

static void _ADD_HL_SP()
{
    LogDebug("ADD HL,SP");
    _ADDHL(R.SP);
    tick(4);
}

#undef _ADDHL
#undef _ADC
#undef _ADD

static void _ADD_SP_s()
{
    int8_t s = nextByte();
    tick(4);

    LogDebug("ADD SP,%d", s);

    R.SP = add16s(R.SP, s);
    tick(8);
}

#endif // ADD_H