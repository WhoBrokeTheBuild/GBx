#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->PC = 0x0000;
    CPU->SP = 0xFFFE;
}

UNIT_TEST(CALL_u16)
{
    writeWord(CPU->PC, 0x1234);
    SM83_INST_CALL_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST(CALL_NZ_u16_true)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FZ = true;
    SM83_INST_CALL_NZ_u16(CPU);
    unit_assert_hex_eq(0x0002, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(CALL_NZ_u16_false)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FZ = false;
    SM83_INST_CALL_NZ_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST(CALL_Z_u16_false)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FZ = false;
    SM83_INST_CALL_Z_u16(CPU);
    unit_assert_hex_eq(0x0002, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(CALL_Z_u16_true)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FZ = true;
    SM83_INST_CALL_Z_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST(CALL_NC_u16_true)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FC = true;
    SM83_INST_CALL_NC_u16(CPU);
    unit_assert_hex_eq(0x0002, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(CALL_NC_u16_false)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FC = false;
    SM83_INST_CALL_NC_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST(CALL_C_u16_false)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FC = false;
    SM83_INST_CALL_C_u16(CPU);
    unit_assert_hex_eq(0x0002, CPU->PC);
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(CALL_C_u16_true)
{
    writeWord(CPU->PC, 0x1234);
    CPU->FC = true;
    SM83_INST_CALL_C_u16(CPU);
    unit_assert_hex_eq(0x1234, CPU->PC);
    unit_assert_int_eq(5, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(CALL)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(CALL_u16);
	UNIT_RUN_TEST(CALL_NZ_u16_true);
	UNIT_RUN_TEST(CALL_NZ_u16_false);
	UNIT_RUN_TEST(CALL_Z_u16_false);
	UNIT_RUN_TEST(CALL_Z_u16_true);
	UNIT_RUN_TEST(CALL_NC_u16_true);
	UNIT_RUN_TEST(CALL_NC_u16_false);
	UNIT_RUN_TEST(CALL_C_u16_false);
	UNIT_RUN_TEST(CALL_C_u16_true);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(CALL);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}