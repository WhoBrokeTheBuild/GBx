#include "inst/call.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xC100;

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
    R.SP = RAM_OFFSET + 0x0100;
}

UNIT_TEST(CALL_uu)
{
    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    _CALL_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, CPUTicks);
}

UNIT_TEST(CALL_NZ_uu)
{
    R.PC = RAM_OFFSET;
    R.FZ = true;
    _CALL_NZ_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FZ = false;
    _CALL_NZ_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(28, CPUTicks);
}

UNIT_TEST(CALL_Z_uu)
{
    R.PC = RAM_OFFSET;
    R.FZ = false;
    _CALL_Z_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FZ = true;
    _CALL_Z_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(28, CPUTicks);
}

UNIT_TEST(CALL_NC_uu)
{
    R.PC = RAM_OFFSET;
    R.FC = true;
    _CALL_NC_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FC = false;
    _CALL_NC_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(28, CPUTicks);
}

UNIT_TEST(CALL_C_uu)
{
    R.PC = RAM_OFFSET;
    R.FC = false;
    _CALL_C_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FC = true;
    _CALL_C_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(28, CPUTicks);
}

UNIT_TEST_SUITE(CALL)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(CALL_uu);
	UNIT_RUN_TEST(CALL_NZ_uu);
	UNIT_RUN_TEST(CALL_Z_uu);
	UNIT_RUN_TEST(CALL_NC_uu);
	UNIT_RUN_TEST(CALL_C_uu);
}

int main(int argc, char ** argv)
{
    DebugMode = true;
	UNIT_RUN_SUITE(CALL);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}