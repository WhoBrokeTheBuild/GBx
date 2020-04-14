#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup()
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->HL = 0x1234;
}

#define MAKE_CP_TEST(REG)                           \
    UNIT_TEST(CP_##REG##_00_00)                     \
    {                                               \
        CPU->A = 0x00;                                 \
        CPU->REG = 0x00;                               \
        SM83_INST_CP_##REG(CPU);                                \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_true(CPU->FN);                     \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }                                               \
                                                    \
    UNIT_TEST(CP_##REG##_00_FF)                     \
    {                                               \
        CPU->A = 0x00;                                 \
        CPU->REG = 0xFF;                               \
        SM83_INST_CP_##REG(CPU);                                \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_true(CPU->FN);                     \
        unit_assert_true(CPU->FH);                     \
        unit_assert_true(CPU->FC);                     \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }                                               \
                                                    \
    UNIT_TEST(CP_##REG##_AA_BB)                     \
    {                                               \
        CPU->A = 0xAA;                                 \
        CPU->REG = 0xBB;                               \
        SM83_INST_CP_##REG(CPU);                                \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_true(CPU->FN);                     \
        unit_assert_true(CPU->FH);                     \
        unit_assert_true(CPU->FC);                     \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }                                               \
                                                    \
    UNIT_TEST(CP_##REG##_BB_AA)                     \
    {                                               \
        CPU->A = 0xBB;                                 \
        CPU->REG = 0xAA;                               \
        SM83_INST_CP_##REG(CPU);                                \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_true(CPU->FN);                     \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }

UNIT_TEST(CP_A_42)
{
    CPU->A = 0x34;
    SM83_INST_CP_A(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_true(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_int_eq(0, CPU->internal->TotalTicks);
}

MAKE_CP_TEST(B);
MAKE_CP_TEST(C);
MAKE_CP_TEST(D);
MAKE_CP_TEST(E);
MAKE_CP_TEST(H);
MAKE_CP_TEST(L);

UNIT_TEST(CP_pHL_00_00)
{
    CPU->A = 0x00;
    writeByte(CPU->HL, 0x00);
    SM83_INST_CP_pHL(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_true(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(CP_pHL_00_FF)
{
    CPU->A = 0x00;
    writeByte(CPU->HL, 0xFF);
    SM83_INST_CP_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_true(CPU->FN);
    unit_assert_true(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(CP_pHL_AA_BB)
{
    CPU->A = 0xAA;
    writeByte(CPU->HL, 0xBB);
    SM83_INST_CP_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_true(CPU->FN);
    unit_assert_true(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(CP_pHL_BB_AA)
{
    CPU->A = 0xBB;
    writeByte(CPU->HL, 0xAA);
    SM83_INST_CP_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_true(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(CP)
{
	UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(CP_A_42);

    UNIT_RUN_TEST(CP_B_00_00);
    UNIT_RUN_TEST(CP_B_00_FF);
    UNIT_RUN_TEST(CP_B_AA_BB);
    UNIT_RUN_TEST(CP_B_BB_AA);
	
    UNIT_RUN_TEST(CP_C_00_00);
    UNIT_RUN_TEST(CP_C_00_FF);
    UNIT_RUN_TEST(CP_C_AA_BB);
    UNIT_RUN_TEST(CP_C_BB_AA);
	
    UNIT_RUN_TEST(CP_D_00_00);
    UNIT_RUN_TEST(CP_D_00_FF);
    UNIT_RUN_TEST(CP_D_AA_BB);
    UNIT_RUN_TEST(CP_D_BB_AA);
	
    UNIT_RUN_TEST(CP_E_00_00);
    UNIT_RUN_TEST(CP_E_00_FF);
    UNIT_RUN_TEST(CP_E_AA_BB);
    UNIT_RUN_TEST(CP_E_BB_AA);
	
    UNIT_RUN_TEST(CP_H_00_00);
    UNIT_RUN_TEST(CP_H_00_FF);
    UNIT_RUN_TEST(CP_H_AA_BB);
    UNIT_RUN_TEST(CP_H_BB_AA);

	UNIT_RUN_TEST(CP_L_00_00);
	UNIT_RUN_TEST(CP_L_00_FF);
	UNIT_RUN_TEST(CP_L_AA_BB);
	UNIT_RUN_TEST(CP_L_BB_AA);

	UNIT_RUN_TEST(CP_pHL_00_00);
	UNIT_RUN_TEST(CP_pHL_00_FF);
	UNIT_RUN_TEST(CP_pHL_AA_BB);
	UNIT_RUN_TEST(CP_pHL_BB_AA);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(CP);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}