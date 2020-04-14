#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->SP = 0xFFFE;
}

UNIT_TEST(POP_AF)
{
    pushWord(0x1234);
    SM83_INST_POP_AF(CPU);
    unit_assert_hex_eq(0xFFFE, CPU->SP);
    unit_assert_hex_eq(0x1234, CPU->AF);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(POP_BC)
{
    pushWord(0x1234);
    SM83_INST_POP_BC(CPU);
    unit_assert_hex_eq(0xFFFE, CPU->SP);
    unit_assert_hex_eq(0x1234, CPU->BC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(POP_DE)
{
    pushWord(0x1234);
    SM83_INST_POP_DE(CPU);
    unit_assert_hex_eq(0xFFFE, CPU->SP);
    unit_assert_hex_eq(0x1234, CPU->DE);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(POP_HL)
{
    pushWord(0x1234);
    SM83_INST_POP_HL(CPU);
    unit_assert_int_eq(0xFFFE, CPU->SP);
    unit_assert_int_eq(0x1234, CPU->HL);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(POP)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(POP_AF);
	UNIT_RUN_TEST(POP_BC);
	UNIT_RUN_TEST(POP_DE);
	UNIT_RUN_TEST(POP_HL);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(POP);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}