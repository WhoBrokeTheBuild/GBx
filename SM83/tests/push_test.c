#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->SP = 0xFFFE;
}

UNIT_TEST(PUSH_AF)
{
    CPU->AF = 0x1234;
    SM83_INST_PUSH_AF(CPU);
    unit_assert_hex_eq(0xFFFC, CPU->SP);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(PUSH_BC)
{
    CPU->BC = 0x1234;
    SM83_INST_PUSH_BC(CPU);
    unit_assert_hex_eq(0xFFFC, CPU->SP);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(PUSH_DE)
{
    CPU->DE = 0x1234;
    SM83_INST_PUSH_DE(CPU);
    unit_assert_hex_eq(0xFFFC, CPU->SP);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(PUSH_HL)
{
    CPU->HL = 0x1234;
    SM83_INST_PUSH_HL(CPU);
    unit_assert_hex_eq(0xFFFC, CPU->SP);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(PUSH)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(PUSH_AF);
	UNIT_RUN_TEST(PUSH_BC);
	UNIT_RUN_TEST(PUSH_DE);
	UNIT_RUN_TEST(PUSH_HL);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(PUSH);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}