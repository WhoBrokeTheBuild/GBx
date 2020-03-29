#ifndef INST_ADC_H
#define INST_ADC_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/MMU.h>

#include <GBx/Log.h>

static void _ADC_A()
{
    LogInst("ADC A,A");
    R.A = AddBytes(R.A, R.A + (int)R.FC);
}

static void _ADC_B()
{
    LogInst("ADC A,B");
    R.A = AddBytes(R.A, R.B + (int)R.FC);
}

static void _ADC_C()
{
    LogInst("ADC A,C");
    R.A = AddBytes(R.A, R.C + (int)R.FC);
}

static void _ADC_D()
{
    LogInst("ADC A,D");
    R.A = AddBytes(R.A, R.D + (int)R.FC);
}

static void _ADC_E()
{
    LogInst("ADC A,E");
    R.A = AddBytes(R.A, R.E + (int)R.FC);
}

static void _ADC_H()
{
    LogInst("ADC A,H");
    R.A = AddBytes(R.A, R.H + (int)R.FC);
}

static void _ADC_L()
{
    LogInst("ADC A,L");
    R.A = AddBytes(R.A, R.L + (int)R.FC);
}

static void _ADC_pHL()
{
    LogInst("ADC A,(HL)");
    byte pHL = ReadByte(R.HL);
    Tick(4);

    R.A = AddBytes(R.A, pHL + (int)R.FC);
}

static void _ADC_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("ADC A,$%02X", u);

    R.A = AddBytes(R.A, u + (int)R.FC);
}

#endif // INST_ADC_H