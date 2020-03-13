#ifndef LOAD_H
#define LOAD_H

#include "alu.h"
#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

static void _LD_A_A()
{
    LogInst("LD A,A");
    R.A = R.A;
}

static void _LD_A_B()
{
    LogInst("LD A,B");
    R.A = R.B;
}

static void _LD_A_C()
{
    LogInst("LD A,C");
    R.A = R.C;
}

static void _LD_A_D()
{
    LogInst("LD A,D");
    R.A = R.D;
}

static void _LD_A_E()
{
    LogInst("LD A,E");
    R.A = R.E;
}

static void _LD_A_H()
{
    LogInst("LD A,H");
    R.A = R.H;
}

static void _LD_A_L()
{
    LogInst("LD A,L");
    R.A = R.L;
}

static void _LD_A_pHL()
{
    LogInst("LD A,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.A = n;
}

static void _LD_B_A()
{
    LogInst("LD B,A");
    R.B = R.A;
}

static void _LD_B_B()
{
    LogInst("LD B,B");
    R.B = R.B;
}

static void _LD_B_C()
{
    LogInst("LD B,C");
    R.B = R.C;
}

static void _LD_B_D()
{
    LogInst("LD B,D");
    R.B = R.D;
}

static void _LD_B_E()
{
    LogInst("LD B,E");
    R.B = R.E;
}

static void _LD_B_H()
{
    LogInst("LD B,H");
    R.B = R.H;
}

static void _LD_B_L()
{
    LogInst("LD B,L");
    R.B = R.L;
}

static void _LD_B_pHL()
{
    LogInst("LD B,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.B = n;
}

static void _LD_C_A()
{
    LogInst("LD C,A");
    R.C = R.A;
}

static void _LD_C_B()
{
    LogInst("LD C,B");
    R.C = R.B;
}

static void _LD_C_C()
{
    LogInst("LD C,C");
    R.C = R.C;
}

static void _LD_C_D()
{
    LogInst("LD C,D");
    R.C = R.D;
}

static void _LD_C_E()
{
    LogInst("LD C,E");
    R.C = R.E;
}

static void _LD_C_H()
{
    LogInst("LD C,H");
    R.C = R.H;
}

static void _LD_C_L()
{
    LogInst("LD C,L");
    R.C = R.L;
}

static void _LD_C_pHL()
{
    LogInst("LD C,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.C = n;
}

static void _LD_D_A()
{
    LogInst("LD D,A");
    R.D = R.A;
}

static void _LD_D_B()
{
    LogInst("LD D,B");
    R.D = R.B;
}

static void _LD_D_C()
{
    LogInst("LD D,C");
    R.D = R.C;
}

static void _LD_D_D()
{
    LogInst("LD D,D");
    R.D = R.D;
}

static void _LD_D_E()
{
    LogInst("LD D,E");
    R.D = R.E;
}

static void _LD_D_H()
{
    LogInst("LD D,H");
    R.D = R.H;
}

static void _LD_D_L()
{
    LogInst("LD D,L");
    R.D = R.L;
}

static void _LD_D_pHL()
{
    LogInst("LD D,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.D = n;
}

static void _LD_E_A()
{
    LogInst("LD E,A");
    R.E = R.A;
}

static void _LD_E_B()
{
    LogInst("LD E,B");
    R.E = R.B;
}

static void _LD_E_C()
{
    LogInst("LD E,C");
    R.E = R.C;
}

static void _LD_E_D()
{
    LogInst("LD E,D");
    R.E = R.D;
}

static void _LD_E_E()
{
    LogInst("LD E,E");
    R.E = R.E;
}

static void _LD_E_H()
{
    LogInst("LD E,H");
    R.E = R.H;
}

static void _LD_E_L()
{
    LogInst("LD E,L");
    R.E = R.L;
}

static void _LD_E_pHL()
{
    LogInst("LD E,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.E = n;
}

static void _LD_H_A()
{
    LogInst("LD H,A");
    R.H = R.A;
}

static void _LD_H_B()
{
    LogInst("LD H,B");
    R.H = R.B;
}

static void _LD_H_C()
{
    LogInst("LD H,C");
    R.H = R.C;
}

static void _LD_H_D()
{
    LogInst("LD H,D");
    R.H = R.D;
}

static void _LD_H_E()
{
    LogInst("LD H,E");
    R.H = R.E;
}

static void _LD_H_H()
{
    LogInst("LD H,H");
    R.H = R.H;
}

static void _LD_H_L()
{
    LogInst("LD H,L");
    R.H = R.L;
}

static void _LD_H_pHL()
{
    LogInst("LD H,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.H = n;
}

static void _LD_L_A()
{
    LogInst("LD L,A");
    R.L = R.A;
}

static void _LD_L_B()
{
    LogInst("LD L,B");
    R.L = R.B;
}

static void _LD_L_C()
{
    LogInst("LD L,C");
    R.L = R.C;
}

static void _LD_L_D()
{
    LogInst("LD L,D");
    R.L = R.D;
}

static void _LD_L_E()
{
    LogInst("LD L,E");
    R.L = R.E;
}

static void _LD_L_H()
{
    LogInst("LD L,H");
    R.L = R.H;
}

static void _LD_L_L()
{
    LogInst("LD L,L");
    R.L = R.L;
}

static void _LD_L_pHL()
{
    LogInst("LD L,(HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    R.L = n;
}

static void _LD_pHL_A()
{
    LogInst("LD (HL),A");
    WriteByte(R.HL, R.A);
    Tick(8);
}

static void _LD_pHL_B()
{
    LogInst("LD (HL),B");
    WriteByte(R.HL, R.B);
    Tick(8);
}

static void _LD_pHL_C()
{
    LogInst("LD (HL),C");
    WriteByte(R.HL, R.C);
    Tick(8);
}

static void _LD_pHL_D()
{
    LogInst("LD (HL),D");
    WriteByte(R.HL, R.D);
    Tick(8);
}

static void _LD_pHL_E()
{
    LogInst("LD (HL),E");
    WriteByte(R.HL, R.E);
    Tick(8);
}

static void _LD_pHL_H()
{
    LogInst("LD (HL),H");
    WriteByte(R.HL, R.H);
    Tick(8);
}

static void _LD_pHL_L()
{
    LogInst("LD (HL),L");
    WriteByte(R.HL, R.L);
    Tick(8);
}

static void _LD_pBC_A()
{
    LogInst("LD (BC),A");
    WriteByte(R.BC, R.A);
    Tick(8);
}

static void _LD_pDE_A()
{
    LogInst("LD (DE),A");
    WriteByte(R.DE, R.A);
    Tick(8);
}

static void _LD_A_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LD A,%02Xh", u);

    R.A = u;
}

static void _LD_B_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LD B,%02Xh", u);

    R.B = u;
}

static void _LD_C_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LD C,%02Xh", u);

    R.C = u;
}

static void _LD_D_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LD D,%02Xh", u);

    R.D = u;
}

static void _LD_E_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LD E,%02Xh", u);

    R.E = u;
}

static void _LD_H_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LD H,%02Xh", u);

    R.H = u;
}

static void _LD_L_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LD L,%02Xh", u);

    R.L = u;
}

static void _LD_pHL_u()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LD (HL),%02Xh", u);

    WriteByte(R.HL, u);
    Tick(4);
}

static void _LD_A_pBC()
{
    LogInst("LD A,(BC)");
    byte n = ReadByte(R.BC);
    Tick(4);

    R.A = n;
}

static void _LD_A_pDE()
{
    LogInst("LD A,(DE)");
    byte n = ReadByte(R.DE);
    Tick(4);

    R.A = n;
}

static void _LD_BC_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("LD BC,%04Xh", uu);
    R.BC = uu;
}

static void _LD_DE_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("LD DE,%04Xh", uu);
    R.DE = uu;
}

static void _LD_HL_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("LD HL,%04Xh", uu);
    R.HL = uu;
}

static void _LD_SP_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("LD SP,%04Xh", uu);
    R.SP = uu;
}

static void _LD_SP_HL()
{
    LogInst("LD SP,HL");
    R.SP = R.HL;
    
    Tick(4);
}

static void _LD_puu_SP()
{
    word uu = NextWord();
    Tick(8);

    LogInst("LD (%04Xh),SP", uu);
    
    WriteWord(uu, R.SP);
    Tick(8);
}

static void _LD_puu_A()
{
    word uu = NextWord();
    Tick(8);

    LogInst("LD (%04Xh),A", uu);

    WriteByte(uu, R.A);
    Tick(4);
}

static void _LD_A_puu()
{
    word uu = NextWord();
    Tick(4);

    LogInst("LD A,(%04Xh)", uu);

    byte n = ReadByte(uu);
    Tick(4);

    R.A = n;
    Tick(4);
}

static void _LDI_pHL_A()
{
    LogInst("LDI (HL),A");
    WriteByte(R.HL, R.A);
    ++R.HL;
    Tick(4);
}

static void _LDD_pHL_A()
{
    LogInst("LDD (HL),A");
    WriteByte(R.HL, R.A);
    --R.HL;
    Tick(4);
}

static void _LDI_A_pHL()
{
    LogInst("LDI A,(HL)");
    R.A = ReadByte(R.HL);
    ++R.HL;
    Tick(4);
}

static void _LDD_A_pHL()
{
    LogInst("LDD A,(HL)");
    R.A = ReadByte(R.HL);;
    --R.HL;
    Tick(4);
}

static void _LDH_pu_A()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LDH (%02Xh),A", u);

    WriteByte(0xFF00 + u, R.A);
    Tick(4);
}

static void _LDH_A_pu()
{
    byte u = NextByte();
    Tick(4);

    LogInst("LDH A,(%02Xh)", u);

    R.A = ReadByte(0xFF00 + u);
    Tick(4);
}

static void _LDH_pC_A()
{
    LogInst("LDH (C),A");
    WriteByte(0xFF00 + R.C, R.A);
    Tick(4);
}

static void _LDH_A_pC()
{
    LogInst("LDH A,(C)");
    R.A = ReadByte(0xFF00 + R.C);
    Tick(4);
}

static void _LD_HL_SP_s()
{
    sbyte s = NextByte();
    Tick(4);

    LogInst("LD HL,SP%+d", s);

    R.HL = add16s(R.SP, s);
    Tick(4);
}

#endif // LOAD_H