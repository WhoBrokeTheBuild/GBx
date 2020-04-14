#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->HL = 0x1234;
}

#define MAKE_DEC_TEST8(REG)                         \
    UNIT_TEST(DEC_##REG##_01)                       \
    {                                               \
        CPU->REG = 0x01;                               \
        SM83_INST_DEC_##REG(CPU);                               \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_true(CPU->FN);                     \
        unit_assert_false(CPU->FH);                    \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(DEC_##REG##_F0)                       \
    {                                               \
        CPU->REG = 0xF0;                               \
        SM83_INST_DEC_##REG(CPU);                               \
        unit_assert_hex_eq(0xEF, CPU->REG);            \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_true(CPU->FN);                     \
        unit_assert_true(CPU->FH);                     \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(DEC_##REG##_00)                       \
    {                                               \
        CPU->REG = 0x00;                               \
        SM83_INST_DEC_##REG(CPU);                               \
        unit_assert_hex_eq(0xFF, CPU->REG);            \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_true(CPU->FN);                     \
        unit_assert_true(CPU->FH);                     \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);            \
    }

MAKE_DEC_TEST8(A);
MAKE_DEC_TEST8(B);
MAKE_DEC_TEST8(C);
MAKE_DEC_TEST8(D);
MAKE_DEC_TEST8(E);
MAKE_DEC_TEST8(H);
MAKE_DEC_TEST8(L);

UNIT_TEST(DEC_pHL_01)
{
    writeByte(CPU->HL, 0x01);
    SM83_INST_DEC_pHL(CPU);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(DEC_pHL_00)
{
    writeByte(CPU->HL, 0x00);
    SM83_INST_DEC_pHL(CPU);
    unit_assert_hex_eq(0xFF, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(DEC8)
{
    UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(DEC_A_01);
    UNIT_RUN_TEST(DEC_A_F0);
    UNIT_RUN_TEST(DEC_A_00);
    
    UNIT_RUN_TEST(DEC_B_01);
    UNIT_RUN_TEST(DEC_B_F0);
    UNIT_RUN_TEST(DEC_B_00);
    
    UNIT_RUN_TEST(DEC_C_01);
    UNIT_RUN_TEST(DEC_C_F0);
    UNIT_RUN_TEST(DEC_C_00);
    
    UNIT_RUN_TEST(DEC_D_01);
    UNIT_RUN_TEST(DEC_D_F0);
    UNIT_RUN_TEST(DEC_D_00);
    
    UNIT_RUN_TEST(DEC_E_01);
    UNIT_RUN_TEST(DEC_E_F0);
    UNIT_RUN_TEST(DEC_E_00);
    
    UNIT_RUN_TEST(DEC_H_01);
    UNIT_RUN_TEST(DEC_H_F0);
    UNIT_RUN_TEST(DEC_H_00);

    UNIT_RUN_TEST(DEC_L_01);
    UNIT_RUN_TEST(DEC_L_F0);
    UNIT_RUN_TEST(DEC_L_00);

    UNIT_RUN_TEST(DEC_pHL_01);
    UNIT_RUN_TEST(DEC_pHL_00);
}

#define MAKE_DEC_TEST16(REG)                        \
    UNIT_TEST(DEC_##REG##_0001)                     \
    {                                               \
        CPU->REG = 0x0001;                             \
        SM83_INST_DEC_##REG(CPU);                               \
        unit_assert_hex_eq(0x0000, CPU->REG);          \
        unit_assert_int_eq(1, CPU->internal->TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(DEC_##REG##_0000)                     \
    {                                               \
        CPU->REG = 0x0000;                             \
        SM83_INST_DEC_##REG(CPU);                               \
        unit_assert_hex_eq(0xFFFF, CPU->REG);          \
        unit_assert_int_eq(1, CPU->internal->TotalTicks);            \
    }

MAKE_DEC_TEST16(BC);
MAKE_DEC_TEST16(DE);
MAKE_DEC_TEST16(HL);
MAKE_DEC_TEST16(SP);

UNIT_TEST_SUITE(DEC16)
{
    UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(DEC_DE_0001);
    UNIT_RUN_TEST(DEC_DE_0000);
    
    UNIT_RUN_TEST(DEC_BC_0001);
    UNIT_RUN_TEST(DEC_BC_0000);

    UNIT_RUN_TEST(DEC_HL_0001);
    UNIT_RUN_TEST(DEC_HL_0000);

    UNIT_RUN_TEST(DEC_SP_0001);
    UNIT_RUN_TEST(DEC_SP_0000);
}

int main(int argc, char ** argv)
{
    stub_init();

    UNIT_RUN_SUITE(DEC8);
    UNIT_RUN_SUITE(DEC16);
    UNIT_REPORT();
    return UNIT_EXIT_CODE;
}