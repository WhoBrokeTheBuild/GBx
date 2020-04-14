#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->PC = 0x0000;
    CPU->HL = 0x1234;
}

#define MAKE_LOAD_TEST_R_R(REG1, REG2)              \
    UNIT_TEST(LD_##REG1##_##REG2)                   \
    {                                               \
        CPU->REG2 = 0xAF;                              \
        SM83_INST_LD_##REG1##_##REG2(CPU);                      \
        unit_assert_hex_eq(0xAF, CPU->REG1);           \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);            \
    }

#define MAKE_LOAD_TEST_R_pHL(REG)                   \
    UNIT_TEST(LD_##REG##_pHL)                       \
    {                                               \
        writeByte(0x1234, 0xAF);                \
        SM83_INST_LD_##REG##_pHL(CPU);                          \
        unit_assert_hex_eq(0xAF, CPU->REG);            \
        unit_assert_int_eq(1, CPU->internal->TotalTicks);            \
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
        CPU->REG = 0xAF;                               \
        SM83_INST_LD_pHL_##REG(CPU);                            \
        unit_assert_hex_eq(0xAF, readByte(CPU->HL));   \
        unit_assert_int_eq(1, CPU->internal->TotalTicks);            \
    }

MAKE_LOAD_TEST_pHL_R(A);
MAKE_LOAD_TEST_pHL_R(B);
MAKE_LOAD_TEST_pHL_R(C);
MAKE_LOAD_TEST_pHL_R(D);
MAKE_LOAD_TEST_pHL_R(E);

UNIT_TEST(LD_pHL_H)
{
    CPU->H = 0xCF;
    SM83_INST_LD_pHL_H(CPU);
    unit_assert_hex_eq(0xCF, readByte(CPU->HL));
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_pHL_L)
{
    CPU->L = 0xFF;
    SM83_INST_LD_pHL_L(CPU);
    unit_assert_hex_eq(0xFF, readByte(CPU->HL));
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

#define MAKE_LOAD_TEST_R_u8(REG)                     \
    UNIT_TEST(LD_##REG##_u8)                         \
    {                                               \
        writeByte(CPU->PC, 0xAF);                      \
        SM83_INST_LD_##REG##_u8(CPU);                            \
        unit_assert_hex_eq(0xAF, CPU->REG);            \
        unit_assert_int_eq(1, CPU->internal->TotalTicks);            \
    }

MAKE_LOAD_TEST_R_u8(A);
MAKE_LOAD_TEST_R_u8(B);
MAKE_LOAD_TEST_R_u8(C);
MAKE_LOAD_TEST_R_u8(D);
MAKE_LOAD_TEST_R_u8(E);
MAKE_LOAD_TEST_R_u8(H);
MAKE_LOAD_TEST_R_u8(L);

UNIT_TEST(LD_pHL_u8)
{
    writeByte(CPU->PC, 0xAF);
    SM83_INST_LD_pHL_u8(CPU);
    unit_assert_hex_eq(0xAF, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_A_pBC)
{
    CPU->BC = 0x1234;
    writeByte(CPU->BC, 0xAF);
    SM83_INST_LD_A_pBC(CPU);
    unit_assert_hex_eq(0xAF, readByte(CPU->BC));
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_A_pDE)
{
    CPU->DE = 0x1234;
    writeByte(CPU->DE, 0xAF);
    SM83_INST_LD_A_pDE(CPU);
    unit_assert_hex_eq(0xAF, readByte(CPU->DE));
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_BC_u16)
{
    writeWord(CPU->PC, 0xAFBC);
    SM83_INST_LD_BC_u16(CPU);
    unit_assert_hex_eq(0xAFBC, CPU->BC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_DE_u16)
{
    writeWord(CPU->PC, 0xAFBC);
    SM83_INST_LD_DE_u16(CPU);
    unit_assert_hex_eq(0xAFBC, CPU->DE);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_HL_u16)
{
    writeWord(CPU->PC, 0xAFBC);
    SM83_INST_LD_HL_u16(CPU);
    unit_assert_hex_eq(0xAFBC, CPU->HL);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_SP_u16)
{
    writeWord(CPU->PC, 0xAFBC);
    SM83_INST_LD_SP_u16(CPU);
    unit_assert_hex_eq(0xAFBC, CPU->SP);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_SP_HL)
{
    SM83_INST_LD_SP_HL(CPU);
    unit_assert_hex_eq(CPU->HL, CPU->SP);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_pu16_SP)
{
    writeWord(CPU->PC, 0x1234);
    CPU->SP = 0xAFBC;
    SM83_INST_LD_pu16_SP(CPU);
    unit_assert_hex_eq(0xAFBC, readWord(0x1234));
    unit_assert_int_eq(4, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_pu16_A)
{
    writeWord(CPU->PC, 0x1234);
    CPU->A = 0xAF;
    SM83_INST_LD_pu16_A(CPU);
    unit_assert_hex_eq(0xAF, readByte(0x1234));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_A_pu16)
{
    writeWord(CPU->PC, 0x1234);
    writeByte(0x1234, 0xAF);
    SM83_INST_LD_A_pu16(CPU);
    unit_assert_hex_eq(0xAF, CPU->A);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(LDI_pHL_A)
{
    CPU->A = 0xAF;
    SM83_INST_LDI_pHL_A(CPU);
    unit_assert_hex_eq(0xAF, CPU->A);
    unit_assert_hex_eq(0x1234 + 1, CPU->HL);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LDD_pHL_A)
{
    CPU->A = 0xAF;
    SM83_INST_LDD_pHL_A(CPU);
    unit_assert_hex_eq(0xAF, CPU->A);
    unit_assert_hex_eq(0x1234 - 1, CPU->HL);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LDI_A_pHL)
{
    writeByte(CPU->HL, 0xAF);
    SM83_INST_LDI_A_pHL(CPU);
    unit_assert_hex_eq(0xAF, readByte(CPU->HL - 1));
    unit_assert_hex_eq(0x1234 + 1, CPU->HL);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LDD_A_pHL)
{
    writeByte(CPU->HL, 0xAF);
    SM83_INST_LDD_A_pHL(CPU);
    unit_assert_hex_eq(0xAF, readByte(CPU->HL + 1));
    unit_assert_hex_eq(0x1234 - 1, CPU->HL);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LDH_pu8_A)
{
    writeByte(CPU->PC, 0x80);
    CPU->A = 0xAF;
    SM83_INST_LDH_pu8_A(CPU);
    unit_assert_hex_eq(0xAF, readByte(0xFF80));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(LDH_A_pu8)
{
    writeByte(CPU->PC, 0x80);
    writeByte(0xFF80, 0xAF);
    SM83_INST_LDH_A_pu8(CPU);
    unit_assert_hex_eq(0xAF, CPU->A);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(LDH_pC_A)
{
    CPU->C = 0x80;
    CPU->A = 0xAF;
    SM83_INST_LDH_pC_A(CPU);
    unit_assert_hex_eq(0xAF, readByte(0xFF80));
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LDH_A_pC)
{
    CPU->C = 0x80;
    writeByte(0xFF80, 0xAF);
    SM83_INST_LDH_A_pC(CPU);
    unit_assert_hex_eq(0xAF, CPU->A);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(LD_HL_SP_s8)
{
    CPU->SP = 0x1234;
    writeByte(CPU->PC, 2);
    SM83_INST_LD_HL_SP_s8(CPU);
    unit_assert_hex_eq(0x1236, CPU->HL);
    unit_assert_false(CPU->FZ);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);

    CPU->SP = 0x1234;
    writeByte(CPU->PC, -2);
    SM83_INST_LD_HL_SP_s8(CPU);
    unit_assert_hex_eq(0x1232, CPU->HL);
    unit_assert_false(CPU->FZ);
    unit_assert_int_eq(4, CPU->internal->TotalTicks);
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

    UNIT_RUN_TEST(LD_A_u8);
    UNIT_RUN_TEST(LD_B_u8);
    UNIT_RUN_TEST(LD_C_u8);
    UNIT_RUN_TEST(LD_D_u8);
    UNIT_RUN_TEST(LD_E_u8);
    UNIT_RUN_TEST(LD_H_u8);
    UNIT_RUN_TEST(LD_L_u8);

    UNIT_RUN_TEST(LD_pHL_u8);

    UNIT_RUN_TEST(LD_A_pBC);
    UNIT_RUN_TEST(LD_A_pDE);

    UNIT_RUN_TEST(LD_BC_u16);
    UNIT_RUN_TEST(LD_DE_u16);
    UNIT_RUN_TEST(LD_HL_u16);
    UNIT_RUN_TEST(LD_SP_u16);

    UNIT_RUN_TEST(LD_pu16_SP);
    UNIT_RUN_TEST(LD_pu16_A);
    UNIT_RUN_TEST(LD_A_pu16);

    UNIT_RUN_TEST(LDI_pHL_A);
    UNIT_RUN_TEST(LDD_pHL_A);
    UNIT_RUN_TEST(LDI_A_pHL);
    UNIT_RUN_TEST(LDD_A_pHL);

    UNIT_RUN_TEST(LDH_pu8_A);
    UNIT_RUN_TEST(LDH_A_pu8);
    UNIT_RUN_TEST(LDH_pC_A);
    UNIT_RUN_TEST(LDH_A_pC);

    UNIT_RUN_TEST(LD_HL_SP_s8);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(LD);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}