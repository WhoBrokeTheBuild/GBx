#ifndef ADD_H
#define ADD_H

#include "alu.h"
#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

#define _ADD(x) \
    R.A = add8(R.A, (x))

#define _ADC(x) \
    R.A = add8(R.A, (x) + R.FC)

#define _ADDHL(x) \
    R.HL = add16(R.HL, (x))
    
static void _ADD_A()
{
    LogInst("ADD A,A");
    _ADD(R.A);
}

static void _ADD_B()
{
    LogInst("ADD A,B");
    _ADD(R.B);
}

static void _ADD_C()
{
    LogInst("ADD A,C");
    _ADD(R.C);
}

static void _ADD_D()
{
    LogInst("ADD A,D");
    _ADD(R.D);
}

static void _ADD_E()
{
    LogInst("ADD A,E");
    _ADD(R.E);
}

static void _ADD_H()
{
    LogInst("ADD A,H");
    _ADD(R.H);
}

static void _ADD_L()
{
    LogInst("ADD A,L");
    _ADD(R.L);
}

static void _ADD_pHL()
{
    LogInst("ADD A,(HL)");
    uint8_t pHL = readByte(R.HL);
    tick(4);

    _ADD(pHL);
}

static void _ADD_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogInst("ADD A,%02Xh", u);

    _ADD(u);
}

static void _ADC_A()
{
    LogInst("ADC A,A");
    _ADC(R.A);
}

static void _ADC_B()
{
    LogInst("ADC A,B");
    _ADC(R.B);
}

static void _ADC_C()
{
    LogInst("ADC A,C");
    _ADC(R.C);
}

static void _ADC_D()
{
    LogInst("ADC A,D");
    _ADC(R.D);
}

static void _ADC_E()
{
    LogInst("ADC A,E");
    _ADC(R.E);
}

static void _ADC_H()
{
    LogInst("ADC A,H");
    _ADC(R.H);
}

static void _ADC_L()
{
    LogInst("ADC A,L");
    _ADC(R.L);
}

static void _ADC_pHL()
{
    LogInst("ADC A,(HL)");
    uint8_t pHL = readByte(R.HL);
    tick(4);

    _ADC(pHL);
}

static void _ADC_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogInst("ADC A,%02Xh", u);

    _ADC(u);
}

static void _ADD_HL_BC()
{
    LogInst("ADD HL,BC");
    _ADDHL(R.BC);
    tick(4);
}

static void _ADD_HL_DE()
{
    LogInst("ADD HL,DE");
    _ADDHL(R.DE);
    tick(4);
}

static void _ADD_HL_HL()
{
    LogInst("ADD HL,HL");
    _ADDHL(R.HL);
    tick(4);
}

static void _ADD_HL_SP()
{
    LogInst("ADD HL,SP");
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

    LogInst("ADD SP,%d", s);

    R.SP = add16s(R.SP, s);
    tick(4);
}

#endif // ADD_H