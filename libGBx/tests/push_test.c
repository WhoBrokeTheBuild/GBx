#include "inst/push.h"
#include "clock.h"
#include "memory.h"
#include "unit.h"

void setup() 
{
    TotalTicks = 0;
    memset(&R, sizeof(R), 0);
    R.SP = 0xFFFE;
}

UNIT_TEST(PUSH_AF)
{
    R.AF = 0x1234;
    _PUSH_AF();
    unit_assert_hex_eq(0xFFFC, R.SP);
    unit_assert_hex_eq(0x1234, ReadWord(R.SP));
    unit_assert_int_eq(16, TotalTicks);
}

UNIT_TEST(PUSH_BC)
{
    R.BC = 0x1234;
    _PUSH_BC();
    unit_assert_hex_eq(0xFFFC, R.SP);
    unit_assert_hex_eq(0x1234, ReadWord(R.SP));
    unit_assert_int_eq(16, TotalTicks);
}

UNIT_TEST(PUSH_DE)
{
    R.DE = 0x1234;
    _PUSH_DE();
    unit_assert_hex_eq(0xFFFC, R.SP);
    unit_assert_hex_eq(0x1234, ReadWord(R.SP));
    unit_assert_int_eq(16, TotalTicks);
}

UNIT_TEST(PUSH_HL)
{
    R.HL = 0x1234;
    _PUSH_HL();
    unit_assert_hex_eq(0xFFFC, R.SP);
    unit_assert_hex_eq(0x1234, ReadWord(R.SP));
    unit_assert_int_eq(16, TotalTicks);
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
    VerboseLevel = 4;
	UNIT_RUN_SUITE(PUSH);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}