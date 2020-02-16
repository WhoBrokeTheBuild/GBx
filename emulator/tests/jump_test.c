#include "inst/jump.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xC100;

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
}

UNIT_TEST(JP_uu)
{
    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    _JP_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(JP_NZ_uu)
{
    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FZ = true;
    _JP_NZ_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FZ = false;
    _JP_NZ_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, CPUTicks);
}

UNIT_TEST(JP_Z_uu)
{
    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FZ = false;
    _JP_Z_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FZ = true;
    _JP_Z_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, CPUTicks);
}

UNIT_TEST(JP_NC_uu)
{
    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FC = true;
    _JP_NC_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FC = false;
    _JP_NC_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, CPUTicks);
}

UNIT_TEST(JP_C_uu)
{
    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FC = false;
    _JP_C_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeWord(R.PC, 0x1234);
    R.FC = true;
    _JP_C_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(20, CPUTicks);
}

UNIT_TEST(JP_pHL)
{
    R.PC = RAM_OFFSET;
    R.HL = RAM_OFFSET;
    writeWord(R.HL, 0x1234);
    _JP_pHL();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST_SUITE(JP)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(JP_uu);
	UNIT_RUN_TEST(JP_NZ_uu);
	UNIT_RUN_TEST(JP_Z_uu);
	UNIT_RUN_TEST(JP_NC_uu);
	UNIT_RUN_TEST(JP_C_uu);

	UNIT_RUN_TEST(JP_pHL);
}

UNIT_TEST(JR_s)
{
    R.PC = RAM_OFFSET;
    writeByte(R.PC, 2);
    _JR_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(8, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, -2);
    _JR_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(16, CPUTicks);
}

UNIT_TEST(JR_NZ_s)
{
    R.PC = RAM_OFFSET;
    R.FZ = true;
    _JR_NZ_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, 2);
    R.FZ = false;
    _JR_NZ_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(12, CPUTicks);

    R.PC = RAM_OFFSET;
    R.FZ = true;
    _JR_NZ_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(16, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, -2);
    R.FZ = false;
    _JR_NZ_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(24, CPUTicks);
}

UNIT_TEST(JR_Z_s)
{
    R.PC = RAM_OFFSET;
    R.FZ = false;
    _JR_Z_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, 2);
    R.FZ = true;
    _JR_Z_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(12, CPUTicks);

    R.PC = RAM_OFFSET;
    R.FZ = false;
    _JR_Z_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(16, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, -2);
    R.FZ = true;
    _JR_Z_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(24, CPUTicks);
}

UNIT_TEST(JR_NC_s)
{
    R.PC = RAM_OFFSET;
    R.FC = true;
    _JR_NC_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, 2);
    R.FC = false;
    _JR_NC_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(12, CPUTicks);

    R.PC = RAM_OFFSET;
    R.FC = true;
    _JR_NC_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(16, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, -2);
    R.FC = false;
    _JR_NC_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(24, CPUTicks);
}

UNIT_TEST(JR_C_s)
{
    R.PC = RAM_OFFSET;
    R.FC = false;
    _JR_C_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, 2);
    R.FC = true;
    _JR_C_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(12, CPUTicks);

    R.PC = RAM_OFFSET;
    R.FC = false;
    _JR_C_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(16, CPUTicks);

    R.PC = RAM_OFFSET;
    writeByte(R.PC, -2);
    R.FC = true;
    _JR_C_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(24, CPUTicks);
}

UNIT_TEST_SUITE(JR)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(JR_s);
	UNIT_RUN_TEST(JR_NZ_s);
	UNIT_RUN_TEST(JR_Z_s);
	UNIT_RUN_TEST(JR_NC_s);
	UNIT_RUN_TEST(JR_C_s);
}

int main(int argc, char ** argv)
{
    DebugMode = true;
	UNIT_RUN_SUITE(JP);
	UNIT_RUN_SUITE(JR);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}