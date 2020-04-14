#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->HL = 0x1234;
}

#define MAKE_INC_TEST8(REG)                         \
    UNIT_TEST(INC_##REG##_00)                       \
    {                                               \
        CPU->REG = 0x00;                               \
        SM83_INST_INC_##REG(CPU);                               \
        unit_assert_hex_eq(0x01, CPU->REG);            \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##REG##_0F)                       \
    {                                               \
        CPU->REG = 0x0F;                               \
        SM83_INST_INC_##REG(CPU);                               \
        unit_assert_hex_eq(0x10, CPU->REG);            \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                    \
        unit_assert_true(CPU->FH);                     \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##REG##_FF)                       \
    {                                               \
        CPU->REG = 0xFF;                               \
        SM83_INST_INC_##REG(CPU);                               \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                    \
        unit_assert_true(CPU->FH);                     \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);            \
    }

MAKE_INC_TEST8(A);
MAKE_INC_TEST8(B);
MAKE_INC_TEST8(C);
MAKE_INC_TEST8(D);
MAKE_INC_TEST8(E);
MAKE_INC_TEST8(H);
MAKE_INC_TEST8(L);

UNIT_TEST(INC_pHL_00)
{
    writeByte(CPU->HL, 0x00);
    SM83_INST_INC_pHL(CPU);
    unit_assert_hex_eq(0x01, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(INC_pHL_FF)
{
    writeByte(CPU->HL, 0xFF);
    SM83_INST_INC_pHL(CPU);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(INC8)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(INC_A_00);
    UNIT_RUN_TEST(INC_A_0F);
    UNIT_RUN_TEST(INC_A_FF);
	
    UNIT_RUN_TEST(INC_B_00);
    UNIT_RUN_TEST(INC_B_0F);
    UNIT_RUN_TEST(INC_B_FF);
	
    UNIT_RUN_TEST(INC_C_00);
    UNIT_RUN_TEST(INC_C_0F);
    UNIT_RUN_TEST(INC_C_FF);
	
    UNIT_RUN_TEST(INC_D_00);
    UNIT_RUN_TEST(INC_D_0F);
    UNIT_RUN_TEST(INC_D_FF);
	
    UNIT_RUN_TEST(INC_E_00);
    UNIT_RUN_TEST(INC_E_0F);
    UNIT_RUN_TEST(INC_E_FF);
	
    UNIT_RUN_TEST(INC_H_00);
    UNIT_RUN_TEST(INC_H_0F);
    UNIT_RUN_TEST(INC_H_FF);

	UNIT_RUN_TEST(INC_L_00);
    UNIT_RUN_TEST(INC_L_0F);
    UNIT_RUN_TEST(INC_L_FF);

	UNIT_RUN_TEST(INC_pHL_00);
	UNIT_RUN_TEST(INC_pHL_FF);
}

#define MAKE_INC_TEST16(REG)                        \
    UNIT_TEST(INC_##REG##_0000)                     \
    {                                               \
        CPU->REG = 0x0000;                             \
        SM83_INST_INC_##REG(CPU);                               \
        unit_assert_hex_eq(0x0001, CPU->REG);          \
        unit_assert_int_eq(1, CPU->internal->TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##REG##_FFFF)                     \
    {                                               \
        CPU->REG = 0xFFFF;                             \
        SM83_INST_INC_##REG(CPU);                               \
        unit_assert_hex_eq(0x0000, CPU->REG);          \
        unit_assert_int_eq(1, CPU->internal->TotalTicks);            \
    }

MAKE_INC_TEST16(BC);
MAKE_INC_TEST16(DE);
MAKE_INC_TEST16(HL);
MAKE_INC_TEST16(SP);

UNIT_TEST_SUITE(INC16)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(INC_DE_0000);
    UNIT_RUN_TEST(INC_DE_FFFF);
    
	UNIT_RUN_TEST(INC_BC_0000);
    UNIT_RUN_TEST(INC_BC_FFFF);

	UNIT_RUN_TEST(INC_HL_0000);
    UNIT_RUN_TEST(INC_HL_FFFF);

	UNIT_RUN_TEST(INC_SP_0000);
    UNIT_RUN_TEST(INC_SP_FFFF);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(INC8);
	UNIT_RUN_SUITE(INC16);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}