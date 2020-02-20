#ifndef LOAD_H
#define LOAD_H

#include "../alu.h"
#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _LD_A_A()
{
    LogDebug("LD A,A");
    R.A = R.A;
}

static void _LD_A_B()
{
    LogDebug("LD A,B");
    R.A = R.B;
}

static void _LD_A_C()
{
    LogDebug("LD A,C");
    R.A = R.C;
}

static void _LD_A_D()
{
    LogDebug("LD A,D");
    R.A = R.D;
}

static void _LD_A_E()
{
    LogDebug("LD A,E");
    R.A = R.E;
}

static void _LD_A_H()
{
    LogDebug("LD A,H");
    R.A = R.H;
}

static void _LD_A_L()
{
    LogDebug("LD A,L");
    R.A = R.L;
}

static void _LD_A_pHL()
{
    LogDebug("LD A,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.A = n;
}

static void _LD_B_A()
{
    LogDebug("LD B,A");
    R.B = R.A;
}

static void _LD_B_B()
{
    LogDebug("LD B,B");
    R.B = R.B;
}

static void _LD_B_C()
{
    LogDebug("LD B,C");
    R.B = R.C;
}

static void _LD_B_D()
{
    LogDebug("LD B,D");
    R.B = R.D;
}

static void _LD_B_E()
{
    LogDebug("LD B,E");
    R.B = R.E;
}

static void _LD_B_H()
{
    LogDebug("LD B,H");
    R.B = R.H;
}

static void _LD_B_L()
{
    LogDebug("LD B,L");
    R.B = R.L;
}

static void _LD_B_pHL()
{
    LogDebug("LD B,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.B = n;
}

static void _LD_C_A()
{
    LogDebug("LD C,A");
    R.C = R.A;
}

static void _LD_C_B()
{
    LogDebug("LD C,B");
    R.C = R.B;
}

static void _LD_C_C()
{
    LogDebug("LD C,C");
    R.C = R.C;
}

static void _LD_C_D()
{
    LogDebug("LD C,D");
    R.C = R.D;
}

static void _LD_C_E()
{
    LogDebug("LD C,E");
    R.C = R.E;
}

static void _LD_C_H()
{
    LogDebug("LD C,H");
    R.C = R.H;
}

static void _LD_C_L()
{
    LogDebug("LD C,L");
    R.C = R.L;
}

static void _LD_C_pHL()
{
    LogDebug("LD C,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.C = n;
}

static void _LD_D_A()
{
    LogDebug("LD D,A");
    R.D = R.A;
}

static void _LD_D_B()
{
    LogDebug("LD D,B");
    R.D = R.B;
}

static void _LD_D_C()
{
    LogDebug("LD D,C");
    R.D = R.C;
}

static void _LD_D_D()
{
    LogDebug("LD D,D");
    R.D = R.D;
}

static void _LD_D_E()
{
    LogDebug("LD D,E");
    R.D = R.E;
}

static void _LD_D_H()
{
    LogDebug("LD D,H");
    R.D = R.H;
}

static void _LD_D_L()
{
    LogDebug("LD D,L");
    R.D = R.L;
}

static void _LD_D_pHL()
{
    LogDebug("LD D,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.D = n;
}

static void _LD_E_A()
{
    LogDebug("LD E,A");
    R.E = R.A;
}

static void _LD_E_B()
{
    LogDebug("LD E,B");
    R.E = R.B;
}

static void _LD_E_C()
{
    LogDebug("LD E,C");
    R.E = R.C;
}

static void _LD_E_D()
{
    LogDebug("LD E,D");
    R.E = R.D;
}

static void _LD_E_E()
{
    LogDebug("LD E,E");
    R.E = R.E;
}

static void _LD_E_H()
{
    LogDebug("LD E,H");
    R.E = R.H;
}

static void _LD_E_L()
{
    LogDebug("LD E,L");
    R.E = R.L;
}

static void _LD_E_pHL()
{
    LogDebug("LD E,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.E = n;
}

static void _LD_H_A()
{
    LogDebug("LD H,A");
    R.H = R.A;
}

static void _LD_H_B()
{
    LogDebug("LD H,B");
    R.H = R.B;
}

static void _LD_H_C()
{
    LogDebug("LD H,C");
    R.H = R.C;
}

static void _LD_H_D()
{
    LogDebug("LD H,D");
    R.H = R.D;
}

static void _LD_H_E()
{
    LogDebug("LD H,E");
    R.H = R.E;
}

static void _LD_H_H()
{
    LogDebug("LD H,H");
    R.H = R.H;
}

static void _LD_H_L()
{
    LogDebug("LD H,L");
    R.H = R.L;
}

static void _LD_H_pHL()
{
    LogDebug("LD H,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.H = n;
}

static void _LD_L_A()
{
    LogDebug("LD L,A");
    R.L = R.A;
}

static void _LD_L_B()
{
    LogDebug("LD L,B");
    R.L = R.B;
}

static void _LD_L_C()
{
    LogDebug("LD L,C");
    R.L = R.C;
}

static void _LD_L_D()
{
    LogDebug("LD L,D");
    R.L = R.D;
}

static void _LD_L_E()
{
    LogDebug("LD L,E");
    R.L = R.E;
}

static void _LD_L_H()
{
    LogDebug("LD L,H");
    R.L = R.H;
}

static void _LD_L_L()
{
    LogDebug("LD L,L");
    R.L = R.L;
}

static void _LD_L_pHL()
{
    LogDebug("LD L,(HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    R.L = n;
}

static void _LD_pHL_A()
{
    LogDebug("LD (HL),A");
    writeByte(R.HL, R.A);
    tick(8);
}

static void _LD_pHL_B()
{
    LogDebug("LD (HL),B");
    writeByte(R.HL, R.B);
    tick(8);
}

static void _LD_pHL_C()
{
    LogDebug("LD (HL),C");
    writeByte(R.HL, R.C);
    tick(8);
}

static void _LD_pHL_D()
{
    LogDebug("LD (HL),D");
    writeByte(R.HL, R.D);
    tick(8);
}

static void _LD_pHL_E()
{
    LogDebug("LD (HL),E");
    writeByte(R.HL, R.E);
    tick(8);
}

static void _LD_pHL_H()
{
    LogDebug("LD (HL),H");
    writeByte(R.HL, R.H);
    tick(8);
}

static void _LD_pHL_L()
{
    LogDebug("LD (HL),L");
    writeByte(R.HL, R.L);
    tick(8);
}

static void _LD_pBC_A()
{
    LogDebug("LD (BC),A");
    writeByte(R.BC, R.A);
    tick(8);
}

static void _LD_pDE_A()
{
    LogDebug("LD (DE),A");
    writeByte(R.DE, R.A);
    tick(8);
}

static void _LD_A_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LD A,%02Xh", u);

    R.A = u;
}

static void _LD_B_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LD B,%02Xh", u);

    R.B = u;
}

static void _LD_C_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LD C,%02Xh", u);

    R.C = u;
}

static void _LD_D_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LD D,%02Xh", u);

    R.D = u;
}

static void _LD_E_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LD E,%02Xh", u);

    R.E = u;
}

static void _LD_H_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LD H,%02Xh", u);

    R.H = u;
}

static void _LD_L_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LD L,%02Xh", u);

    R.L = u;
}

static void _LD_pHL_u()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LD (HL),%02Xh", u);

    writeByte(R.HL, u);
    tick(4);
}

static void _LD_A_pBC()
{
    LogDebug("LD A,(BC)");
    uint8_t n = readByte(R.BC);
    tick(4);

    R.A = n;
}

static void _LD_A_pDE()
{
    LogDebug("LD A,(DE)");
    uint8_t n = readByte(R.DE);
    tick(4);

    R.A = n;
}

static void _LD_BC_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogDebug("LD BC,%04Xh", uu);
    R.BC = uu;
}

static void _LD_DE_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogDebug("LD DE,%04Xh", uu);
    R.DE = uu;
}

static void _LD_HL_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogDebug("LD HL,%04Xh", uu);
    R.HL = uu;
}

static void _LD_SP_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogDebug("LD SP,%04Xh", uu);
    R.SP = uu;
}

static void _LD_SP_HL()
{
    tick(4);
    
    LogDebug("LD SP,HL");
    R.SP = R.HL;
}

static void _LD_puu_SP()
{
    uint16_t uu = nextWord();
    tick(8);

    LogDebug("LD (%04Xh),SP", uu);
    
    writeWord(uu, R.SP);
    tick(8);
}

static void _LD_puu_A()
{
    uint16_t uu = nextWord();
    tick(8);

    LogDebug("LD (%04Xh),A", uu);

    writeByte(uu, R.A);
    tick(4);
}

static void _LD_A_puu()
{
    uint16_t uu = nextWord();
    tick(4);

    LogDebug("LD A,(%04Xh)", uu);

    uint8_t n = readByte(uu);
    tick(4);

    R.A = n;
    tick(4);
}

static void _LDI_pHL_A()
{
    LogDebug("LDI (HL),A");
    writeByte(R.HL, R.A);
    ++R.HL;
    tick(4);
}

static void _LDD_pHL_A()
{
    LogDebug("LDD (HL),A");
    writeByte(R.HL, R.A);
    --R.HL;
    tick(4);
}

static void _LDI_A_pHL()
{
    LogDebug("LDI A,(HL)");
    R.A = readByte(R.HL);
    ++R.HL;
    tick(4);
}

static void _LDD_A_pHL()
{
    LogDebug("LDD A,(HL)");
    R.A = readByte(R.HL);;
    --R.HL;
    tick(4);
}

static void _LDH_pu_A()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LDH (%02Xh),A", u);

    writeByte(0xFF00 + u, R.A);
    tick(4);
}

static void _LDH_A_pu()
{
    uint8_t u = nextByte();
    tick(4);

    LogDebug("LDH A,(%02Xh)", u);

    R.A = readByte(0xFF00 + u);
    tick(4);
}

static void _LDH_pC_A()
{
    LogDebug("LDH (C),A");
    R.A = readByte(0xFF00 + R.C);
    tick(4);
}

static void _LDH_A_pC()
{
    LogDebug("LDH A,(C)");
    writeByte(0xFF00 + R.C, R.A);
    tick(4);
}

static void _LD_HL_SP_s()
{
    int8_t s = nextByte();
    tick(4);

    LogDebug("LD HL,SP%+d", s);

    R.HL = add16s(R.SP, s);
    R.FZ = false;
    tick(4);
}

#endif // LOAD_H