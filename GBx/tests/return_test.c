#include "Inst/RET.h"

#include <GBx/GBx.h>

#include "unit.h"

const word RAM_OFFSET = 0xC100;

void setup() 
{
    TotalTicks = 0;
    memset(&R, sizeof(R), 0);
    R.PC = 0x0000;
    R.SP = RAM_OFFSET + 0x0100;
}

UNIT_TEST(RET)
{
    PushWord(0x1234);
    _RET();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(16, TotalTicks);
}

UNIT_TEST(RET_NZ_true)
{
    PushWord(0x1234);
    R.FZ = true;
    _RET_NZ();
    unit_assert_hex_eq(0x0000, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(RET_NZ_false)
{
    PushWord(0x1234);
    R.FZ = false;
    _RET_NZ();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, TotalTicks);
}

UNIT_TEST(RET_Z_false)
{
    PushWord(0x1234);
    R.FZ = false;
    _RET_Z();
    unit_assert_hex_eq(0x0000, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(RET_Z_true)
{
    PushWord(0x1234);
    R.FZ = true;
    _RET_Z();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, TotalTicks);
}

UNIT_TEST(RET_NC_true)
{
    PushWord(0x1234);
    R.FC = true;
    _RET_NC();
    unit_assert_hex_eq(0x0000, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(RET_NC_false)
{
    PushWord(0x1234);
    R.FC = false;
    _RET_NC();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, TotalTicks);
}

UNIT_TEST(RET_C_false)
{
    PushWord(0x1234);
    R.FC = false;
    _RET_C();
    unit_assert_hex_eq(0x0000, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(RET_C_true)
{
    PushWord(0x1234);
    R.FC = true;
    _RET_C();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, TotalTicks);
}

UNIT_TEST(RETI)
{
    PushWord(0x1234);
    _RETI();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(16, TotalTicks);
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
    VerboseLevel = 4;
	UNIT_RUN_SUITE(RET);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}