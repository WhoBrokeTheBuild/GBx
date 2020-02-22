#ifndef LOAD_H
#define LOAD_H

#include "../alu.h"
#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _LD_A_A()
{
    LogVerbose(3, "LD A,A");
    R.A = R.A;
}

static void _LD_A_B()
{
    LogVerbose(3, "LD A,B");
    R.A = R.B;
}

static void _LD_A_C()
{
    LogVerbose(3, "LD A,C");
    R.A = R.C;
}

static void _LD_A_D()
{
    LogVerbose(3, "LD A,D");
    R.A = R.D;
}

static void _LD_A_E()
{
    LogVerbose(3, "LD A,E");
    R.A = R.E;
}

static void _LD_A_H()
{
    LogVerbose(3, "LD A,H");
    R.A = R.H;
}

static void _LD_A_L()
{
    LogVerbose(3, "LD A,L");
    R.A = R.L;
}

static void _LD_A_pHL()
{
    LogVerbose(3, "LD A,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.A = n;
}

static void _LD_B_A()
{
    LogVerbose(3, "LD B,A");
    R.B = R.A;
}

static void _LD_B_B()
{
    LogVerbose(3, "LD B,B");
    R.B = R.B;
}

static void _LD_B_C()
{
    LogVerbose(3, "LD B,C");
    R.B = R.C;
}

static void _LD_B_D()
{
    LogVerbose(3, "LD B,D");
    R.B = R.D;
}

static void _LD_B_E()
{
    LogVerbose(3, "LD B,E");
    R.B = R.E;
}

static void _LD_B_H()
{
    LogVerbose(3, "LD B,H");
    R.B = R.H;
}

static void _LD_B_L()
{
    LogVerbose(3, "LD B,L");
    R.B = R.L;
}

static void _LD_B_pHL()
{
    LogVerbose(3, "LD B,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.B = n;
}

static void _LD_C_A()
{
    LogVerbose(3, "LD C,A");
    R.C = R.A;
}

static void _LD_C_B()
{
    LogVerbose(3, "LD C,B");
    R.C = R.B;
}

static void _LD_C_C()
{
    LogVerbose(3, "LD C,C");
    R.C = R.C;
}

static void _LD_C_D()
{
    LogVerbose(3, "LD C,D");
    R.C = R.D;
}

static void _LD_C_E()
{
    LogVerbose(3, "LD C,E");
    R.C = R.E;
}

static void _LD_C_H()
{
    LogVerbose(3, "LD C,H");
    R.C = R.H;
}

static void _LD_C_L()
{
    LogVerbose(3, "LD C,L");
    R.C = R.L;
}

static void _LD_C_pHL()
{
    LogVerbose(3, "LD C,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.C = n;
}

static void _LD_D_A()
{
    LogVerbose(3, "LD D,A");
    R.D = R.A;
}

static void _LD_D_B()
{
    LogVerbose(3, "LD D,B");
    R.D = R.B;
}

static void _LD_D_C()
{
    LogVerbose(3, "LD D,C");
    R.D = R.C;
}

static void _LD_D_D()
{
    LogVerbose(3, "LD D,D");
    R.D = R.D;
}

static void _LD_D_E()
{
    LogVerbose(3, "LD D,E");
    R.D = R.E;
}

static void _LD_D_H()
{
    LogVerbose(3, "LD D,H");
    R.D = R.H;
}

static void _LD_D_L()
{
    LogVerbose(3, "LD D,L");
    R.D = R.L;
}

static void _LD_D_pHL()
{
    LogVerbose(3, "LD D,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.D = n;
}

static void _LD_E_A()
{
    LogVerbose(3, "LD E,A");
    R.E = R.A;
}

static void _LD_E_B()
{
    LogVerbose(3, "LD E,B");
    R.E = R.B;
}

static void _LD_E_C()
{
    LogVerbose(3, "LD E,C");
    R.E = R.C;
}

static void _LD_E_D()
{
    LogVerbose(3, "LD E,D");
    R.E = R.D;
}

static void _LD_E_E()
{
    LogVerbose(3, "LD E,E");
    R.E = R.E;
}

static void _LD_E_H()
{
    LogVerbose(3, "LD E,H");
    R.E = R.H;
}

static void _LD_E_L()
{
    LogVerbose(3, "LD E,L");
    R.E = R.L;
}

static void _LD_E_pHL()
{
    LogVerbose(3, "LD E,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.E = n;
}

static void _LD_H_A()
{
    LogVerbose(3, "LD H,A");
    R.H = R.A;
}

static void _LD_H_B()
{
    LogVerbose(3, "LD H,B");
    R.H = R.B;
}

static void _LD_H_C()
{
    LogVerbose(3, "LD H,C");
    R.H = R.C;
}

static void _LD_H_D()
{
    LogVerbose(3, "LD H,D");
    R.H = R.D;
}

static void _LD_H_E()
{
    LogVerbose(3, "LD H,E");
    R.H = R.E;
}

static void _LD_H_H()
{
    LogVerbose(3, "LD H,H");
    R.H = R.H;
}

static void _LD_H_L()
{
    LogVerbose(3, "LD H,L");
    R.H = R.L;
}

static void _LD_H_pHL()
{
    LogVerbose(3, "LD H,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.H = n;
}

static void _LD_L_A()
{
    LogVerbose(3, "LD L,A");
    R.L = R.A;
}

static void _LD_L_B()
{
    LogVerbose(3, "LD L,B");
    R.L = R.B;
}

static void _LD_L_C()
{
    LogVerbose(3, "LD L,C");
    R.L = R.C;
}

static void _LD_L_D()
{
    LogVerbose(3, "LD L,D");
    R.L = R.D;
}

static void _LD_L_E()
{
    LogVerbose(3, "LD L,E");
    R.L = R.E;
}

static void _LD_L_H()
{
    LogVerbose(3, "LD L,H");
    R.L = R.H;
}

static void _LD_L_L()
{
    LogVerbose(3, "LD L,L");
    R.L = R.L;
}

static void _LD_L_pHL()
{
    LogVerbose(3, "LD L,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.L = n;
}

static void _LD_pHL_A()
{
    LogVerbose(3, "LD (HL),A");
    writeByte(R.HL, R.A);
    tick(8);
}

static void _LD_pHL_B()
{
    LogVerbose(3, "LD (HL),B");
    writeByte(R.HL, R.B);
    tick(8);
}

static void _LD_pHL_C()
{
    LogVerbose(3, "LD (HL),C");
    writeByte(R.HL, R.C);
    tick(8);
}

static void _LD_pHL_D()
{
    LogVerbose(3, "LD (HL),D");
    writeByte(R.HL, R.D);
    tick(8);
}

static void _LD_pHL_E()
{
    LogVerbose(3, "LD (HL),E");
    writeByte(R.HL, R.E);
    tick(8);
}

static void _LD_pHL_H()
{
    LogVerbose(3, "LD (HL),H");
    writeByte(R.HL, R.H);
    tick(8);
}

static void _LD_pHL_L()
{
    LogVerbose(3, "LD (HL),L");
    writeByte(R.HL, R.L);
    tick(8);
}

static void _LD_pBC_A()
{
    LogVerbose(3, "LD (BC),A");
    writeByte(R.BC, R.A);
    tick(8);
}

static void _LD_pDE_A()
{
    LogVerbose(3, "LD (DE),A");
    writeByte(R.DE, R.A);
    tick(8);
}

static void _LD_A_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LD A,%02Xh", u);

    R.A = u;
}

static void _LD_B_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LD B,%02Xh", u);

    R.B = u;
}

static void _LD_C_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LD C,%02Xh", u);

    R.C = u;
}

static void _LD_D_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LD D,%02Xh", u);

    R.D = u;
}

static void _LD_E_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LD E,%02Xh", u);

    R.E = u;
}

static void _LD_H_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LD H,%02Xh", u);

    R.H = u;
}

static void _LD_L_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LD L,%02Xh", u);

    R.L = u;
}

static void _LD_pHL_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LD (HL),%02Xh", u);

    writeByte(R.HL, u);
    tick(4);
}

static void _LD_A_pBC()
{
    LogVerbose(3, "LD A,(BC)");
    uint8_t n = readByte(R.BC);
    tick(4);

    R.A = n;
}

static void _LD_A_pDE()
{
    LogVerbose(3, "LD A,(DE)");
    uint8_t n = readByte(R.DE);
    tick(4);

    R.A = n;
}

static void _LD_BC_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "LD BC,%04Xh", uu);
    R.BC = uu;
}

static void _LD_DE_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "LD DE,%04Xh", uu);
    R.DE = uu;
}

static void _LD_HL_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "LD HL,%04Xh", uu);
    R.HL = uu;
}

static void _LD_SP_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "LD SP,%04Xh", uu);
    R.SP = uu;
}

static void _LD_SP_HL()
{
    tick(4);
    
    LogVerbose(3, "LD SP,HL");
    R.SP = R.HL;
}

static void _LD_puu_SP()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "LD (%04Xh),SP", uu);
    
    writeWord(uu, R.SP);
    tick(8);
}

static void _LD_puu_A()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "LD (%04Xh),A", uu);

    writeByte(uu, R.A);
    tick(4);
}

static void _LD_A_puu()
{
    uint16_t uu = nextWord();
    tick(4);

    LogVerbose(3, "LD A,(%04Xh)", uu);

    uint8_t n = readByte(uu);
    tick(4);

    R.A = n;
    tick(4);
}

static void _LDI_pHL_A()
{
    LogVerbose(3, "LDI (HL),A");
    writeByte(R.HL, R.A);
    ++R.HL;
    tick(4);
}

static void _LDD_pHL_A()
{
    LogVerbose(3, "LDD (HL),A");
    writeByte(R.HL, R.A);
    --R.HL;
    tick(4);
}

static void _LDI_A_pHL()
{
    LogVerbose(3, "LDI A,(HL)");
    R.A = readByte(R.HL);
    ++R.HL;
    tick(4);
}

static void _LDD_A_pHL()
{
    LogVerbose(3, "LDD A,(HL)");
    R.A = readByte(R.HL);;
    --R.HL;
    tick(4);
}

static void _LDH_pu_A()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LDH (%02Xh),A", u);

    writeByte(0xFF00 + u, R.A);
    tick(4);
}

static void _LDH_A_pu()
{
    uint8_t u = nextByte();
    tick(4);

    LogVerbose(3, "LDH A,(%02Xh)", u);

    R.A = readByte(0xFF00 + u);
    tick(4);
}

static void _LDH_pC_A()
{
    LogVerbose(3, "LDH (C),A");
    R.A = readByte(0xFF00 + R.C);
    tick(4);
}

static void _LDH_A_pC()
{
    LogVerbose(3, "LDH A,(C)");
    writeByte(0xFF00 + R.C, R.A);
    tick(4);
}

static void _LD_HL_SP_s()
{
    int8_t s = nextByte();
    tick(4);

    LogVerbose(3, "LD HL,SP%+d", s);

    R.HL = add16s(R.SP, s);
    R.FZ = false;
    tick(4);
}

#endif // LOAD_H