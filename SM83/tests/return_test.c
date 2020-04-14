#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->PC = 0x0000;
    CPU->SP = 0xFFFE;
}

UNIT_TEST(RET)
{
    pushWord(0x1234);
    SM83_INST_RET(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(RET_NZ_true)
{
    pushWord(0x1234);
    CPU->FZ = true;
    SM83_INST_RET_NZ(CPU);
    unit_assert_hex_eq(0x0000, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(RET_NZ_false)
{
    pushWord(0x1234);
    CPU->FZ = false;
    SM83_INST_RET_NZ(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST(RET_Z_false)
{
    pushWord(0x1234);
    CPU->FZ = false;
    SM83_INST_RET_Z(CPU);
    unit_assert_hex_eq(0x0000, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(RET_Z_true)
{
    pushWord(0x1234);
    CPU->FZ = true;
    SM83_INST_RET_Z(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST(RET_NC_true)
{
    pushWord(0x1234);
    CPU->FC = true;
    SM83_INST_RET_NC(CPU);
    unit_assert_hex_eq(0x0000, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(RET_NC_false)
{
    pushWord(0x1234);
    CPU->FC = false;
    SM83_INST_RET_NC(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST(RET_C_false)
{
    pushWord(0x1234);
    CPU->FC = false;
    SM83_INST_RET_C(CPU);
    unit_assert_hex_eq(0x0000, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(RET_C_true)
{
    pushWord(0x1234);
    CPU->FC = true;
    SM83_INST_RET_C(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST(RETI)
{
    pushWord(0x1234);
    SM83_INST_RETI(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(RET)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(RET);
    UNIT_RUN_TEST(RET_NZ_true);
    UNIT_RUN_TEST(RET_NZ_false);
    UNIT_RUN_TEST(RET_Z_false);
    UNIT_RUN_TEST(RET_Z_true);
    UNIT_RUN_TEST(RET_NC_true);
    UNIT_RUN_TEST(RET_NC_false);
    UNIT_RUN_TEST(RET_C_false);
    UNIT_RUN_TEST(RET_C_true);
    UNIT_RUN_TEST(RETI);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(RET);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}