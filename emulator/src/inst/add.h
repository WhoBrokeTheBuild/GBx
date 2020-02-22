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
    LogVerbose(3, "ADD A,A");
    _ADD(R.A);
}

static void _ADD_B()
{
    LogVerbose(3, "ADD A,B");
    _ADD(R.B);
}

static void _ADD_C()
{
    LogVerbose(3, "ADD A,C");
    _ADD(R.C);
}

static void _ADD_D()
{
    LogVerbose(3, "ADD A,D");
    _ADD(R.D);
}

static void _ADD_E()
{
    LogVerbose(3, "ADD A,E");
    _ADD(R.E);
}

static void _ADD_H()
{
    LogVerbose(3, "ADD A,H");
    _ADD(R.H);
}

static void _ADD_L()
{
    LogVerbose(3, "ADD A,L");
    _ADD(R.L);
}

static void _ADD_pHL()
{
    LogVerbose(3, "ADD A,(HL)");
    uint8_t pHL = readByte(R.HL);
    tick(4);

    _ADD(pHL);
}

static void _ADD_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "ADD A,%02Xh", u);

    _ADD(u);
}

static void _ADC_A()
{
    LogVerbose(3, "ADC A,A");
    _ADC(R.A);
}

static void _ADC_B()
{
    LogVerbose(3, "ADC A,B");
    _ADC(R.B);
}

static void _ADC_C()
{
    LogVerbose(3, "ADC A,C");
    _ADC(R.C);
}

static void _ADC_D()
{
    LogVerbose(3, "ADC A,D");
    _ADC(R.D);
}

static void _ADC_E()
{
    LogVerbose(3, "ADC A,E");
    _ADC(R.E);
}

static void _ADC_H()
{
    LogVerbose(3, "ADC A,H");
    _ADC(R.H);
}

static void _ADC_L()
{
    LogVerbose(3, "ADC A,L");
    _ADC(R.L);
}

static void _ADC_pHL()
{
    LogVerbose(3, "ADC A,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _ADC(n);
}

static void _ADC_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "ADC A,%02Xh", u);

    _ADC(u);
}

static void _ADD_HL_BC()
{
    LogVerbose(3, "ADD HL,BC");
    _ADDHL(R.BC);
    tick(4);
}

static void _ADD_HL_DE()
{
    LogVerbose(3, "ADD HL,DE");
    _ADDHL(R.DE);
    tick(4);
}

static void _ADD_HL_HL()
{
    LogVerbose(3, "ADD HL,HL");
    _ADDHL(R.HL);
    tick(4);
}

static void _ADD_HL_SP()
{
    LogVerbose(3, "ADD HL,SP");
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

    LogVerbose(3, "ADD SP,%d", s);

    R.SP = add16s(R.SP, s);
    tick(8);
}

#endif // ADD_H