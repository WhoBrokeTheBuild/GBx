#include "inst/restart.h"
#include "memory.h"
#include "unit.h"

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
    R.SP = 0xFFFE;
    R.PC = 0x1234;
}

UNIT_TEST(RST_00)
{
    _RST_00();
    unit_assert_hex_eq(0x0000, R.PC);
    unit_assert_hex_eq(0x1234, readWord(R.SP));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(RST_08)
{
    _RST_08();
    unit_assert_hex_eq(0x0008, R.PC);
    unit_assert_hex_eq(0x1234, readWord(R.SP));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(RST_10)
{
    _RST_10();
    unit_assert_hex_eq(0x0010, R.PC);
    unit_assert_hex_eq(0x1234, readWord(R.SP));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(RST_18)
{
    _RST_18();
    unit_assert_hex_eq(0x0018, R.PC);
    unit_assert_hex_eq(0x1234, readWord(R.SP));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(RST_20)
{
    _RST_20();
    unit_assert_hex_eq(0x0020, R.PC);
    unit_assert_hex_eq(0x1234, readWord(R.SP));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(RST_28)
{
    _RST_28();
    unit_assert_hex_eq(0x0028, R.PC);
    unit_assert_hex_eq(0x1234, readWord(R.SP));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(RST_30)
{
    _RST_30();
    unit_assert_hex_eq(0x0030, R.PC);
    unit_assert_hex_eq(0x1234, readWord(R.SP));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(RST_38)
{
    _RST_38();
    unit_assert_hex_eq(0x0038, R.PC);
    unit_assert_hex_eq(0x1234, readWord(R.SP));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST_SUITE(RST)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(RST_00);
	UNIT_RUN_TEST(RST_08);
	UNIT_RUN_TEST(RST_10);
	UNIT_RUN_TEST(RST_18);
	UNIT_RUN_TEST(RST_20);
	UNIT_RUN_TEST(RST_28);
	UNIT_RUN_TEST(RST_30);
	UNIT_RUN_TEST(RST_38);
}

int main(int argc, char ** argv)
{
    DebugMode = true;
	UNIT_RUN_SUITE(RST);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}