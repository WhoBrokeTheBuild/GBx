#ifndef INST_ADD_H
#define INST_ADD_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _ADD_A()
{
    LogInst("ADD A,A");
    R.A = AddBytes(R.A, R.A);
}

static void _ADD_B()
{
    LogInst("ADD A,B");
    R.A = AddBytes(R.A, R.B);
}

static void _ADD_C()
{
    LogInst("ADD A,C");
    R.A = AddBytes(R.A, R.C);
}

static void _ADD_D()
{
    LogInst("ADD A,D");
    R.A = AddBytes(R.A, R.D);
}

static void _ADD_E()
{
    LogInst("ADD A,E");
    R.A = AddBytes(R.A, R.E);
}

static void _ADD_H()
{
    LogInst("ADD A,H");
    R.A = AddBytes(R.A, R.H);
}

static void _ADD_L()
{
    LogInst("ADD A,L");
    R.A = AddBytes(R.A, R.L);
}

static void _ADD_pHL()
{
    LogInst("ADD A,(HL)");
    byte pHL = ReadByte(R.HL);
    Tick(4);

    R.A = AddBytes(R.A, pHL);
}

static void _ADD_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("ADD A,$%02X", u);

    R.A = AddBytes(R.A, u);
}

static void _ADD_SP_s()
{
    sbyte s = NextByte();
    Tick(4);

    LogInst("ADD SP,%d", s);

    R.SP = AddWordSignedByte(R.SP, s);
    Tick(4);

    StackBaseAddress = R.SP;
}

#endif // ADD_H