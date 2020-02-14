#include "inst/pop.h"
#include "memory.h"
#include "unit.h"

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
    R.SP = 0xFFFE;
}

UNIT_TEST(POP_AF)
{
    pushWord(0x1234);
    _POP_AF();
    unit_assert_hex_eq(0xFFFE, R.SP);
    unit_assert_hex_eq(0x1234, R.AF);
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(POP_BC)
{
    pushWord(0x1234);
    _POP_BC();
    unit_assert_hex_eq(0xFFFE, R.SP);
    unit_assert_hex_eq(0x1234, R.BC);
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(POP_DE)
{
    pushWord(0x1234);
    _POP_DE();
    unit_assert_hex_eq(0xFFFE, R.SP);
    unit_assert_hex_eq(0x1234, R.DE);
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(POP_HL)
{
    pushWord(0x1234);
    _POP_HL();
    unit_assert_int_eq(0xFFFE, R.SP);
    unit_assert_int_eq(0x1234, R.HL);
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST_SUITE(POP)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(POP_AF);
	UNIT_RUN_TEST(POP_BC);
	UNIT_RUN_TEST(POP_DE);
	UNIT_RUN_TEST(POP_HL);
}

int main(int argc, char *argv[])
{
    DebugMode = true;
	UNIT_RUN_SUITE(POP);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}