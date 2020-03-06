#include "inst/jump.h"
#include "clock.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xC100;

void setup() 
{
    TotalTicks = 0;
    memset(&R, sizeof(R), 0);
    R.PC = RAM_OFFSET;
}

UNIT_TEST(JP_uu)
{
    writeWord(R.PC, 0x1234);
    _JP_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(12, TotalTicks);
}

UNIT_TEST(JP_NZ_uu_true)
{
    writeWord(R.PC, 0x1234);
    R.FZ = true;
    _JP_NZ_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JP_NZ_uu_false)
{
    writeWord(R.PC, 0x1234);
    R.FZ = false;
    _JP_NZ_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(12, TotalTicks);
}

UNIT_TEST(JP_Z_uu_false)
{
    writeWord(R.PC, 0x1234);
    R.FZ = false;
    _JP_Z_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JP_Z_uu_true)
{
    writeWord(R.PC, 0x1234);
    R.FZ = true;
    _JP_Z_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(12, TotalTicks);
}

UNIT_TEST(JP_NC_uu_true)
{
    writeWord(R.PC, 0x1234);
    R.FC = true;
    _JP_NC_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JP_NC_uu_false)
{
    writeWord(R.PC, 0x1234);
    R.FC = false;
    _JP_NC_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(12, TotalTicks);
}

UNIT_TEST(JP_C_uu_false)
{
    writeWord(R.PC, 0x1234);
    R.FC = false;
    _JP_C_uu();
    unit_assert_hex_eq(RAM_OFFSET + 2, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JP_C_uu_true)
{
    writeWord(R.PC, 0x1234);
    R.FC = true;
    _JP_C_uu();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(12, TotalTicks);
}

UNIT_TEST(JP_pHL)
{
    R.HL = RAM_OFFSET;
    writeWord(R.HL, 0x1234);
    _JP_pHL();
    unit_assert_hex_eq(0x1234, R.PC);
    unit_assert_int_eq(12, TotalTicks);
}

UNIT_TEST_SUITE(JP)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(JP_uu);
	UNIT_RUN_TEST(JP_NZ_uu_true);
	UNIT_RUN_TEST(JP_NZ_uu_false);
	UNIT_RUN_TEST(JP_Z_uu_false);
	UNIT_RUN_TEST(JP_Z_uu_true);
	UNIT_RUN_TEST(JP_NC_uu_true);
	UNIT_RUN_TEST(JP_NC_uu_false);
	UNIT_RUN_TEST(JP_C_uu_false);
	UNIT_RUN_TEST(JP_C_uu_true);

	UNIT_RUN_TEST(JP_pHL);
}

UNIT_TEST(JR_s_positive)
{
    writeByte(R.PC, 2);
    _JR_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_s_negative)
{
    writeByte(R.PC, -2);
    _JR_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_NZ_s_true_positive)
{
    writeByte(R.PC, 2);
    R.FZ = true;
    _JR_NZ_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, TotalTicks);
}

UNIT_TEST(JR_NZ_s_false_positive)
{
    writeByte(R.PC, 2);
    R.FZ = false;
    _JR_NZ_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_NZ_s_true_negative)
{
    writeByte(R.PC, -2);
    R.FZ = true;
    _JR_NZ_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, TotalTicks);
}

UNIT_TEST(JR_NZ_s_false_negative)
{
    writeByte(R.PC, -2);
    R.FZ = false;
    _JR_NZ_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_Z_s_false_positive)
{
    writeByte(R.PC, 2);
    R.FZ = false;
    _JR_Z_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, TotalTicks);
}

UNIT_TEST(JR_Z_s_true_positive)
{
    writeByte(R.PC, 2);
    R.FZ = true;
    _JR_Z_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_Z_s_false_negative)
{
    writeByte(R.PC, -2);
    R.FZ = false;
    _JR_Z_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, TotalTicks);
}

UNIT_TEST(JR_Z_s_true_negative)
{
    writeByte(R.PC, -2);
    R.FZ = true;
    _JR_Z_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_NC_s_true_positive)
{
    writeByte(R.PC, 2);
    R.FC = true;
    _JR_NC_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, TotalTicks);
}

UNIT_TEST(JR_NC_s_false_positive)
{
    writeByte(R.PC, 2);
    R.FC = false;
    _JR_NC_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_NC_s_true_negative)
{
    writeByte(R.PC, -2);
    R.FC = true;
    _JR_NC_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, TotalTicks);
}

UNIT_TEST(JR_NC_s_false_negative)
{
    writeByte(R.PC, -2);
    R.FC = false;
    _JR_NC_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_C_s_false_positive)
{
    writeByte(R.PC, 2);
    R.FC = false;
    _JR_C_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, TotalTicks);
}

UNIT_TEST(JR_C_s_true_positive)
{
    writeByte(R.PC, 2);
    R.FC = true;
    _JR_C_s();
    unit_assert_hex_eq(RAM_OFFSET + 3, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST(JR_C_s_false_negative)
{
    writeByte(R.PC, -2);
    R.FC = false;
    _JR_C_s();
    unit_assert_hex_eq(RAM_OFFSET + 1, R.PC);
    unit_assert_int_eq(4, TotalTicks);
}

UNIT_TEST(JR_C_s_true_negative)
{
    writeByte(R.PC, -2);
    R.FC = true;
    _JR_C_s();
    unit_assert_hex_eq(RAM_OFFSET - 1, R.PC);
    unit_assert_int_eq(8, TotalTicks);
}

UNIT_TEST_SUITE(JR)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(JR_s_positive);
	UNIT_RUN_TEST(JR_s_negative);
	UNIT_RUN_TEST(JR_NZ_s_true_positive);
	UNIT_RUN_TEST(JR_NZ_s_false_positive);
	UNIT_RUN_TEST(JR_NZ_s_true_negative);
	UNIT_RUN_TEST(JR_NZ_s_false_negative);
	UNIT_RUN_TEST(JR_Z_s_false_positive);
	UNIT_RUN_TEST(JR_Z_s_true_positive);
	UNIT_RUN_TEST(JR_Z_s_false_negative);
	UNIT_RUN_TEST(JR_Z_s_true_negative);
	UNIT_RUN_TEST(JR_NC_s_true_positive);
	UNIT_RUN_TEST(JR_NC_s_false_positive);
	UNIT_RUN_TEST(JR_NC_s_true_negative);
	UNIT_RUN_TEST(JR_NC_s_false_negative);
	UNIT_RUN_TEST(JR_C_s_false_positive);
	UNIT_RUN_TEST(JR_C_s_true_positive);
	UNIT_RUN_TEST(JR_C_s_false_negative);
	UNIT_RUN_TEST(JR_C_s_true_negative);
}

int main(int argc, char ** argv)
{
    VerboseLevel = 4;
	UNIT_RUN_SUITE(JP);
	UNIT_RUN_SUITE(JR);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}