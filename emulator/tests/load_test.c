#include "inst/load.h"
#include "clock.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xC100;

void setup() 
{
    Ticks = 0;
    memset(&R, sizeof(R), 0);
    R.HL = RAM_OFFSET;
    R.PC = RAM_OFFSET + 0x0100;
}

#define MAKE_LOAD_TEST_R_R(REG1, REG2)              \
    UNIT_TEST(LD_##REG1##_##REG2)                   \
    {                                               \
        R.REG2 = 0xAF;                              \
        _LD_##REG1##_##REG2();                      \
        unit_assert_hex_eq(0xAF, R.REG1);           \
    }

#define MAKE_LOAD_TEST_R_pHL(REG)                   \
    UNIT_TEST(LD_##REG##_pHL)                       \
    {                                               \
        writeByte(RAM_OFFSET, 0xAF);                \
        _LD_##REG##_pHL();                          \
        unit_assert_hex_eq(0xAF, R.REG);            \
        unit_assert_int_eq(4, Ticks);            \
    }

MAKE_LOAD_TEST_R_R(A, A);
MAKE_LOAD_TEST_R_R(A, B);
MAKE_LOAD_TEST_R_R(A, C);
MAKE_LOAD_TEST_R_R(A, D);
MAKE_LOAD_TEST_R_R(A, E);
MAKE_LOAD_TEST_R_R(A, H);
MAKE_LOAD_TEST_R_R(A, L);

MAKE_LOAD_TEST_R_pHL(A);

MAKE_LOAD_TEST_R_R(B, A);
MAKE_LOAD_TEST_R_R(B, B);
MAKE_LOAD_TEST_R_R(B, C);
MAKE_LOAD_TEST_R_R(B, D);
MAKE_LOAD_TEST_R_R(B, E);
MAKE_LOAD_TEST_R_R(B, H);
MAKE_LOAD_TEST_R_R(B, L);

MAKE_LOAD_TEST_R_pHL(B);

MAKE_LOAD_TEST_R_R(C, A);
MAKE_LOAD_TEST_R_R(C, B);
MAKE_LOAD_TEST_R_R(C, C);
MAKE_LOAD_TEST_R_R(C, D);
MAKE_LOAD_TEST_R_R(C, E);
MAKE_LOAD_TEST_R_R(C, H);
MAKE_LOAD_TEST_R_R(C, L);

MAKE_LOAD_TEST_R_pHL(C);

MAKE_LOAD_TEST_R_R(D, A);
MAKE_LOAD_TEST_R_R(D, B);
MAKE_LOAD_TEST_R_R(D, C);
MAKE_LOAD_TEST_R_R(D, D);
MAKE_LOAD_TEST_R_R(D, E);
MAKE_LOAD_TEST_R_R(D, H);
MAKE_LOAD_TEST_R_R(D, L);

MAKE_LOAD_TEST_R_pHL(D);

MAKE_LOAD_TEST_R_R(E, A);
MAKE_LOAD_TEST_R_R(E, B);
MAKE_LOAD_TEST_R_R(E, C);
MAKE_LOAD_TEST_R_R(E, D);
MAKE_LOAD_TEST_R_R(E, E);
MAKE_LOAD_TEST_R_R(E, H);
MAKE_LOAD_TEST_R_R(E, L);

MAKE_LOAD_TEST_R_pHL(E);

MAKE_LOAD_TEST_R_R(H, A);
MAKE_LOAD_TEST_R_R(H, B);
MAKE_LOAD_TEST_R_R(H, C);
MAKE_LOAD_TEST_R_R(H, D);
MAKE_LOAD_TEST_R_R(H, E);
MAKE_LOAD_TEST_R_R(H, H);
MAKE_LOAD_TEST_R_R(H, L);

MAKE_LOAD_TEST_R_pHL(H);

MAKE_LOAD_TEST_R_R(L, A);
MAKE_LOAD_TEST_R_R(L, B);
MAKE_LOAD_TEST_R_R(L, C);
MAKE_LOAD_TEST_R_R(L, D);
MAKE_LOAD_TEST_R_R(L, E);
MAKE_LOAD_TEST_R_R(L, H);
MAKE_LOAD_TEST_R_R(L, L);

MAKE_LOAD_TEST_R_pHL(L);

#define MAKE_LOAD_TEST_pHL_R(REG)                   \
    UNIT_TEST(LD_pHL_##REG)                         \
    {                                               \
        R.REG = 0xAF;                               \
        _LD_pHL_##REG();                            \
        unit_assert_hex_eq(0xAF, readByte(R.HL));   \
        unit_assert_int_eq(8, Ticks);            \
    }

MAKE_LOAD_TEST_pHL_R(A);
MAKE_LOAD_TEST_pHL_R(B);
MAKE_LOAD_TEST_pHL_R(C);
MAKE_LOAD_TEST_pHL_R(D);
MAKE_LOAD_TEST_pHL_R(E);

UNIT_TEST(LD_pHL_H)
{
    R.H = 0xCF;
    _LD_pHL_H();
    unit_assert_hex_eq(0xCF, readByte(R.HL));
    unit_assert_int_eq(8, Ticks);
}

UNIT_TEST(LD_pHL_L)
{
    R.L = 0xFF;
    _LD_pHL_L();
    unit_assert_hex_eq(0xFF, readByte(R.HL));
    unit_assert_int_eq(8, Ticks);
}

#define MAKE_LOAD_TEST_R_u(REG)                     \
    UNIT_TEST(LD_##REG##_u)                         \
    {                                               \
        writeByte(R.PC, 0xAF);                      \
        _LD_##REG##_u();                            \
        unit_assert_hex_eq(0xAF, R.REG);            \
        unit_assert_int_eq(4, Ticks);            \
    }

MAKE_LOAD_TEST_R_u(A);
MAKE_LOAD_TEST_R_u(B);
MAKE_LOAD_TEST_R_u(C);
MAKE_LOAD_TEST_R_u(D);
MAKE_LOAD_TEST_R_u(E);
MAKE_LOAD_TEST_R_u(H);
MAKE_LOAD_TEST_R_u(L);

UNIT_TEST(LD_pHL_u)
{
    writeByte(R.PC, 0xAF);
    _LD_pHL_u();
    unit_assert_hex_eq(0xAF, readByte(R.HL));
    unit_assert_int_eq(8, Ticks);
}

UNIT_TEST(LD_A_pBC)
{
    R.BC = RAM_OFFSET;
    writeByte(R.BC, 0xAF);
    _LD_A_pBC();
    unit_assert_hex_eq(0xAF, readByte(R.BC));
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LD_A_pDE)
{
    R.DE = RAM_OFFSET;
    writeByte(R.DE, 0xAF);
    _LD_A_pDE();
    unit_assert_hex_eq(0xAF, readByte(R.DE));
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LD_BC_uu)
{
    writeWord(R.PC, 0xAFBC);
    _LD_BC_uu();
    unit_assert_hex_eq(0xAFBC, R.BC);
    unit_assert_int_eq(8, Ticks);
}

UNIT_TEST(LD_DE_uu)
{
    writeWord(R.PC, 0xAFBC);
    _LD_DE_uu();
    unit_assert_hex_eq(0xAFBC, R.DE);
    unit_assert_int_eq(8, Ticks);
}

UNIT_TEST(LD_HL_uu)
{
    writeWord(R.PC, 0xAFBC);
    _LD_HL_uu();
    unit_assert_hex_eq(0xAFBC, R.HL);
    unit_assert_int_eq(8, Ticks);
}

UNIT_TEST(LD_SP_uu)
{
    writeWord(R.PC, 0xAFBC);
    _LD_SP_uu();
    unit_assert_hex_eq(0xAFBC, R.SP);
    unit_assert_int_eq(8, Ticks);
}

UNIT_TEST(LD_SP_HL)
{
    _LD_SP_HL();
    unit_assert_hex_eq(R.HL, R.SP);
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LD_puu_SP)
{
    writeWord(R.PC, RAM_OFFSET);
    R.SP = 0xAFBC;
    _LD_puu_SP();
    unit_assert_hex_eq(0xAFBC, readWord(RAM_OFFSET));
    unit_assert_int_eq(16, Ticks);
}

UNIT_TEST(LD_puu_A)
{
    writeWord(R.PC, RAM_OFFSET);
    R.A = 0xAF;
    _LD_puu_A();
    unit_assert_hex_eq(0xAF, readByte(RAM_OFFSET));
    unit_assert_int_eq(12, Ticks);
}

UNIT_TEST(LD_A_puu)
{
    writeWord(R.PC, RAM_OFFSET);
    writeByte(RAM_OFFSET, 0xAF);
    _LD_A_puu();
    unit_assert_hex_eq(0xAF, R.A);
    unit_assert_int_eq(12, Ticks);
}

UNIT_TEST(LDI_pHL_A)
{
    R.A = 0xAF;
    _LDI_pHL_A();
    unit_assert_hex_eq(0xAF, R.A);
    unit_assert_hex_eq(RAM_OFFSET + 1, R.HL);
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LDD_pHL_A)
{
    R.A = 0xAF;
    _LDD_pHL_A();
    unit_assert_hex_eq(0xAF, R.A);
    unit_assert_hex_eq(RAM_OFFSET - 1, R.HL);
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LDI_A_pHL)
{
    writeByte(R.HL, 0xAF);
    _LDI_A_pHL();
    unit_assert_hex_eq(0xAF, readByte(R.HL - 1));
    unit_assert_hex_eq(RAM_OFFSET + 1, R.HL);
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LDD_A_pHL)
{
    writeByte(R.HL, 0xAF);
    _LDD_A_pHL();
    unit_assert_hex_eq(0xAF, readByte(R.HL + 1));
    unit_assert_hex_eq(RAM_OFFSET - 1, R.HL);
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LDH_pu_A)
{
    writeByte(R.PC, 0x80);
    R.A = 0xAF;
    _LDH_pu_A();
    unit_assert_hex_eq(0xAF, readByte(0xFF80));
    unit_assert_int_eq(8, Ticks);
}

UNIT_TEST(LDH_A_pu)
{
    writeByte(R.PC, 0x80);
    writeByte(0xFF80, 0xAF);
    _LDH_A_pu();
    unit_assert_hex_eq(0xAF, R.A);
    unit_assert_int_eq(8, Ticks);
}

UNIT_TEST(LDH_pC_A)
{
    R.C = 0x80;
    R.A = 0xAF;
    _LDH_pC_A();
    unit_assert_hex_eq(0xAF, readByte(0xFF80));
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LDH_A_pC)
{
    R.C = 0x80;
    writeByte(0xFF80, 0xAF);
    _LDH_A_pC();
    unit_assert_hex_eq(0xAF, R.A);
    unit_assert_int_eq(4, Ticks);
}

UNIT_TEST(LD_HL_SP_s)
{
    R.SP = 0x1234;
    writeByte(R.PC, 2);
    _LD_HL_SP_s();
    unit_assert_hex_eq(0x1236, R.HL);
    unit_assert_false(R.FZ);
    unit_assert_int_eq(8, Ticks);

    R.SP = 0x1234;
    writeByte(R.PC, -2);
    _LD_HL_SP_s();
    unit_assert_hex_eq(0x1232, R.HL);
    unit_assert_false(R.FZ);
    unit_assert_int_eq(16, Ticks);
}

UNIT_TEST_SUITE(LD)
{
	UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(LD_A_A);
    UNIT_RUN_TEST(LD_A_B);
    UNIT_RUN_TEST(LD_A_C);
    UNIT_RUN_TEST(LD_A_D);
    UNIT_RUN_TEST(LD_A_E);
    UNIT_RUN_TEST(LD_A_H);
    UNIT_RUN_TEST(LD_A_L);

    UNIT_RUN_TEST(LD_A_pHL);

    UNIT_RUN_TEST(LD_B_A);
    UNIT_RUN_TEST(LD_B_B);
    UNIT_RUN_TEST(LD_B_C);
    UNIT_RUN_TEST(LD_B_D);
    UNIT_RUN_TEST(LD_B_E);
    UNIT_RUN_TEST(LD_B_H);
    UNIT_RUN_TEST(LD_B_L);

    UNIT_RUN_TEST(LD_B_pHL);

    UNIT_RUN_TEST(LD_C_A);
    UNIT_RUN_TEST(LD_C_B);
    UNIT_RUN_TEST(LD_C_C);
    UNIT_RUN_TEST(LD_C_D);
    UNIT_RUN_TEST(LD_C_E);
    UNIT_RUN_TEST(LD_C_H);
    UNIT_RUN_TEST(LD_C_L);

    UNIT_RUN_TEST(LD_C_pHL);

    UNIT_RUN_TEST(LD_D_A);
    UNIT_RUN_TEST(LD_D_B);
    UNIT_RUN_TEST(LD_D_C);
    UNIT_RUN_TEST(LD_D_D);
    UNIT_RUN_TEST(LD_D_E);
    UNIT_RUN_TEST(LD_D_H);
    UNIT_RUN_TEST(LD_D_L);

    UNIT_RUN_TEST(LD_D_pHL);

    UNIT_RUN_TEST(LD_E_A);
    UNIT_RUN_TEST(LD_E_B);
    UNIT_RUN_TEST(LD_E_C);
    UNIT_RUN_TEST(LD_E_D);
    UNIT_RUN_TEST(LD_E_E);
    UNIT_RUN_TEST(LD_E_H);
    UNIT_RUN_TEST(LD_E_L);

    UNIT_RUN_TEST(LD_E_pHL);

    UNIT_RUN_TEST(LD_H_A);
    UNIT_RUN_TEST(LD_H_B);
    UNIT_RUN_TEST(LD_H_C);
    UNIT_RUN_TEST(LD_H_D);
    UNIT_RUN_TEST(LD_H_E);
    UNIT_RUN_TEST(LD_H_H);
    UNIT_RUN_TEST(LD_H_L);

    UNIT_RUN_TEST(LD_H_pHL);

    UNIT_RUN_TEST(LD_L_A);
    UNIT_RUN_TEST(LD_L_B);
    UNIT_RUN_TEST(LD_L_C);
    UNIT_RUN_TEST(LD_L_D);
    UNIT_RUN_TEST(LD_L_E);
    UNIT_RUN_TEST(LD_L_H);
    UNIT_RUN_TEST(LD_L_L);

    UNIT_RUN_TEST(LD_L_pHL);

    UNIT_RUN_TEST(LD_pHL_A);
    UNIT_RUN_TEST(LD_pHL_B);
    UNIT_RUN_TEST(LD_pHL_C);
    UNIT_RUN_TEST(LD_pHL_D);
    UNIT_RUN_TEST(LD_pHL_E);
    UNIT_RUN_TEST(LD_pHL_H);
    UNIT_RUN_TEST(LD_pHL_L);

    UNIT_RUN_TEST(LD_A_u);
    UNIT_RUN_TEST(LD_B_u);
    UNIT_RUN_TEST(LD_C_u);
    UNIT_RUN_TEST(LD_D_u);
    UNIT_RUN_TEST(LD_E_u);
    UNIT_RUN_TEST(LD_H_u);
    UNIT_RUN_TEST(LD_L_u);

    UNIT_RUN_TEST(LD_pHL_u);

    UNIT_RUN_TEST(LD_A_pBC);
    UNIT_RUN_TEST(LD_A_pDE);

    UNIT_RUN_TEST(LD_BC_uu);
    UNIT_RUN_TEST(LD_DE_uu);
    UNIT_RUN_TEST(LD_HL_uu);
    UNIT_RUN_TEST(LD_SP_uu);

    UNIT_RUN_TEST(LD_puu_SP);
    UNIT_RUN_TEST(LD_puu_A);
    UNIT_RUN_TEST(LD_A_puu);

    UNIT_RUN_TEST(LDI_pHL_A);
    UNIT_RUN_TEST(LDD_pHL_A);
    UNIT_RUN_TEST(LDI_A_pHL);
    UNIT_RUN_TEST(LDD_A_pHL);

    UNIT_RUN_TEST(LDH_pu_A);
    UNIT_RUN_TEST(LDH_A_pu);
    UNIT_RUN_TEST(LDH_pC_A);
    UNIT_RUN_TEST(LDH_A_pC);

    UNIT_RUN_TEST(LD_HL_SP_s);
}

int main(int argc, char ** argv)
{
    DebugMode = true;
	UNIT_RUN_SUITE(LD);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}