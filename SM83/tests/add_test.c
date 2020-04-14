#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup()
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->HL = 0x1234;
}

#define MAKE_ADD_TEST(REG)                          \
    UNIT_TEST(ADD_##REG##_00_00)                    \
    {                                               \
        CPU->A = 0x00;                                 \
        CPU->REG = 0x00;                               \
        SM83_INST_ADD_##REG(CPU);                               \
        unit_assert_hex_eq(0x00, CPU->A);              \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                     \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }                                               \
                                                    \
    UNIT_TEST(ADD_##REG##_00_FF)                    \
    {                                               \
        CPU->A = 0x00;                                 \
        CPU->REG = 0xFF;                               \
        SM83_INST_ADD_##REG(CPU);                               \
        unit_assert_hex_eq(0xFF, CPU->A);              \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                     \
        unit_assert_false(CPU->FH);                     \
        unit_assert_false(CPU->FC);                     \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }                                               \
                                                    \
    UNIT_TEST(ADD_##REG##_AA_BB)                    \
    {                                               \
        CPU->A = 0xAA;                                 \
        CPU->REG = 0xBB;                               \
        SM83_INST_ADD_##REG(CPU);                               \
        unit_assert_hex_eq(0x65, CPU->A);              \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                     \
        unit_assert_true(CPU->FH);                     \
        unit_assert_true(CPU->FC);                     \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }                                               \
                                                    \
    UNIT_TEST(ADD_##REG##_BB_AA)                    \
    {                                               \
        CPU->A = 0xBB;                                 \
        CPU->REG = 0xAA;                               \
        SM83_INST_ADD_##REG(CPU);                               \
        unit_assert_hex_eq(0x65, CPU->A);              \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                     \
        unit_assert_true(CPU->FH);                    \
        unit_assert_true(CPU->FC);                    \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }

UNIT_TEST(ADD_A_42)
{
    CPU->A = 0x42;
    SM83_INST_ADD_A(CPU);
    unit_assert_hex_eq(0x84, CPU->A);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_int_eq(0, CPU->internal->TotalTicks);
}

MAKE_ADD_TEST(B);
MAKE_ADD_TEST(C);
MAKE_ADD_TEST(D);
MAKE_ADD_TEST(E);
MAKE_ADD_TEST(H);
MAKE_ADD_TEST(L);

UNIT_TEST(ADD_pHL_00_00)
{
    CPU->A = 0x00;
    writeByte(CPU->HL, 0x00);
    SM83_INST_ADD_pHL(CPU);
    unit_assert_hex_eq(0x00, CPU->A);
    unit_assert_true(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(ADD_pHL_00_FF)
{
    CPU->A = 0x00;
    writeByte(CPU->HL, 0xFF);
    SM83_INST_ADD_pHL(CPU);
    unit_assert_hex_eq(0xFF, CPU->A);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(ADD_pHL_AA_BB)
{
    CPU->A = 0xAA;
    writeByte(CPU->HL, 0xBB);
    SM83_INST_ADD_pHL(CPU);
    unit_assert_hex_eq(0x65, CPU->A);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_true(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(ADD_pHL_BB_AA)
{
    CPU->A = 0xBB;
    writeByte(CPU->HL, 0xAA);
    SM83_INST_ADD_pHL(CPU);
    unit_assert_hex_eq(0x65, CPU->A);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_true(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(ADD)
{
	UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(ADD_A_42);

    UNIT_RUN_TEST(ADD_B_00_00);
    UNIT_RUN_TEST(ADD_B_00_FF);
    UNIT_RUN_TEST(ADD_B_AA_BB);
    UNIT_RUN_TEST(ADD_B_BB_AA);
	
    UNIT_RUN_TEST(ADD_C_00_00);
    UNIT_RUN_TEST(ADD_C_00_FF);
    UNIT_RUN_TEST(ADD_C_AA_BB);
    UNIT_RUN_TEST(ADD_C_BB_AA);
	
    UNIT_RUN_TEST(ADD_D_00_00);
    UNIT_RUN_TEST(ADD_D_00_FF);
    UNIT_RUN_TEST(ADD_D_AA_BB);
    UNIT_RUN_TEST(ADD_D_BB_AA);
	
    UNIT_RUN_TEST(ADD_E_00_00);
    UNIT_RUN_TEST(ADD_E_00_FF);
    UNIT_RUN_TEST(ADD_E_AA_BB);
    UNIT_RUN_TEST(ADD_E_BB_AA);
	
    UNIT_RUN_TEST(ADD_H_00_00);
    UNIT_RUN_TEST(ADD_H_00_FF);
    UNIT_RUN_TEST(ADD_H_AA_BB);
    UNIT_RUN_TEST(ADD_H_BB_AA);

	UNIT_RUN_TEST(ADD_L_00_00);
	UNIT_RUN_TEST(ADD_L_00_FF);
	UNIT_RUN_TEST(ADD_L_AA_BB);
	UNIT_RUN_TEST(ADD_L_BB_AA);

	UNIT_RUN_TEST(ADD_pHL_00_00);
	UNIT_RUN_TEST(ADD_pHL_00_FF);
	UNIT_RUN_TEST(ADD_pHL_AA_BB);
	UNIT_RUN_TEST(ADD_pHL_BB_AA);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(ADD);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}