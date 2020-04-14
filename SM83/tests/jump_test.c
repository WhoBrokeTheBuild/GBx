#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->PC = 0x1000;
}

UNIT_TEST(JP_u16)
{
    writeWord(CPU->PC, 0x1234);
    SM83_INST_JP_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_NZ_u16_true)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FZ = true;
    SM83_INST_JP_NZ_u16(CPU);
    unit_assert_hex_eq(0x1002, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_NZ_u16_false)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FZ = false;
    SM83_INST_JP_NZ_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_Z_u16_false)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FZ = false;
    SM83_INST_JP_Z_u16(CPU);
    unit_assert_hex_eq(0x1002, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_Z_u16_true)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FZ = true;
    SM83_INST_JP_Z_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_NC_u16_true)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FC = true;
    SM83_INST_JP_NC_u16(CPU);
    unit_assert_hex_eq(0x1002, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_NC_u16_false)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FC = false;
    SM83_INST_JP_NC_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_C_u16_false)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FC = false;
    SM83_INST_JP_C_u16(CPU);
    unit_assert_hex_eq(0x1002, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_C_u16_true)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FC = true;
    SM83_INST_JP_C_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(JP_HL)
{
    CPU->HL = 0x1234;
    SM83_INST_JP_HL(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(0, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(JP)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(JP_u16);
	UNIT_RUN_TEST(JP_NZ_u16_true);
	UNIT_RUN_TEST(JP_NZ_u16_false);
	UNIT_RUN_TEST(JP_Z_u16_false);
	UNIT_RUN_TEST(JP_Z_u16_true);
	UNIT_RUN_TEST(JP_NC_u16_true);
	UNIT_RUN_TEST(JP_NC_u16_false);
	UNIT_RUN_TEST(JP_C_u16_false);
	UNIT_RUN_TEST(JP_C_u16_true);

	UNIT_RUN_TEST(JP_HL);
}

UNIT_TEST(JR_s8_positive)
{
    writeByte(CPU->PC, 2);
    SM83_INST_JR_s8(CPU);
    unit_assert_hex_eq(0x1003, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_s8_negative)
{
    writeByte(CPU->PC, -2);
    SM83_INST_JR_s8(CPU);
    unit_assert_hex_eq(0x0FFF, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_NZ_s8_true_positive)
{
    writeByte(CPU->PC, 2);
    CPU->FZ = true;
    SM83_INST_JR_NZ_s8(CPU);
    unit_assert_hex_eq(0x1001, CPU->PC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_NZ_s8_false_positive)
{
    writeByte(CPU->PC, 2);
    CPU->FZ = false;
    SM83_INST_JR_NZ_s8(CPU);
    unit_assert_hex_eq(0x1003, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_NZ_s8_true_negative)
{
    writeByte(CPU->PC, -2);
    CPU->FZ = true;
    SM83_INST_JR_NZ_s8(CPU);
    unit_assert_hex_eq(0x1001, CPU->PC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_NZ_s8_false_negative)
{
    writeByte(CPU->PC, -2);
    CPU->FZ = false;
    SM83_INST_JR_NZ_s8(CPU);
    unit_assert_hex_eq(0x0FFF, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_Z_s8_false_positive)
{
    writeByte(CPU->PC, 2);
    CPU->FZ = false;
    SM83_INST_JR_Z_s8(CPU);
    unit_assert_hex_eq(0x1001, CPU->PC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_Z_s8_true_positive)
{
    writeByte(CPU->PC, 2);
    CPU->FZ = true;
    SM83_INST_JR_Z_s8(CPU);
    unit_assert_hex_eq(0x1003, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_Z_s8_false_negative)
{
    writeByte(CPU->PC, -2);
    CPU->FZ = false;
    SM83_INST_JR_Z_s8(CPU);
    unit_assert_hex_eq(0x1001, CPU->PC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_Z_s8_true_negative)
{
    writeByte(CPU->PC, -2);
    CPU->FZ = true;
    SM83_INST_JR_Z_s8(CPU);
    unit_assert_hex_eq(0x0FFF, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_NC_s8_true_positive)
{
    writeByte(CPU->PC, 2);
    CPU->FC = true;
    SM83_INST_JR_NC_s8(CPU);
    unit_assert_hex_eq(0x1001, CPU->PC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_NC_s8_false_positive)
{
    writeByte(CPU->PC, 2);
    CPU->FC = false;
    SM83_INST_JR_NC_s8(CPU);
    unit_assert_hex_eq(0x1003, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_NC_s8_true_negative)
{
    writeByte(CPU->PC, -2);
    CPU->FC = true;
    SM83_INST_JR_NC_s8(CPU);
    unit_assert_hex_eq(0x1001, CPU->PC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_NC_s8_false_negative)
{
    writeByte(CPU->PC, -2);
    CPU->FC = false;
    SM83_INST_JR_NC_s8(CPU);
    unit_assert_hex_eq(0x0FFF, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_C_s8_false_positive)
{
    writeByte(CPU->PC, 2);
    CPU->FC = false;
    SM83_INST_JR_C_s8(CPU);
    unit_assert_hex_eq(0x1001, CPU->PC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_C_s8_true_positive)
{
    writeByte(CPU->PC, 2);
    CPU->FC = true;
    SM83_INST_JR_C_s8(CPU);
    unit_assert_hex_eq(0x1003, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_C_s8_false_negative)
{
    writeByte(CPU->PC, -2);
    CPU->FC = false;
    SM83_INST_JR_C_s8(CPU);
    unit_assert_hex_eq(0x1001, CPU->PC);
    unit_assert_int_eq(1, CPU->internal->TotalTicks);
}

UNIT_TEST(JR_C_s8_true_negative)
{
    writeByte(CPU->PC, -2);
    CPU->FC = true;
    SM83_INST_JR_C_s8(CPU);
    unit_assert_hex_eq(0x0FFF, CPU->PC);
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(JR)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(JR_s8_positive);
	UNIT_RUN_TEST(JR_s8_negative);
	UNIT_RUN_TEST(JR_NZ_s8_true_positive);
	UNIT_RUN_TEST(JR_NZ_s8_false_positive);
	UNIT_RUN_TEST(JR_NZ_s8_true_negative);
	UNIT_RUN_TEST(JR_NZ_s8_false_negative);
	UNIT_RUN_TEST(JR_Z_s8_false_positive);
	UNIT_RUN_TEST(JR_Z_s8_true_positive);
	UNIT_RUN_TEST(JR_Z_s8_false_negative);
	UNIT_RUN_TEST(JR_Z_s8_true_negative);
	UNIT_RUN_TEST(JR_NC_s8_true_positive);
	UNIT_RUN_TEST(JR_NC_s8_false_positive);
	UNIT_RUN_TEST(JR_NC_s8_true_negative);
	UNIT_RUN_TEST(JR_NC_s8_false_negative);
	UNIT_RUN_TEST(JR_C_s8_false_positive);
	UNIT_RUN_TEST(JR_C_s8_true_positive);
	UNIT_RUN_TEST(JR_C_s8_false_negative);
	UNIT_RUN_TEST(JR_C_s8_true_negative);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(JP);
	UNIT_RUN_SUITE(JR);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}