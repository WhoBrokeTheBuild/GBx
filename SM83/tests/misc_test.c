#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup() 
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
}

UNIT_TEST(HALT)
{
    CPU->Enabled = true;
    SM83_INST_HALT(CPU);
    unit_assert_false(CPU->Enabled);
    unit_assert_int_eq(CPU->internal->TotalTicks, 0);
}

UNIT_TEST(STOP)
{
    CPU->PC = 0x0000;
    // TODO
    // LCDC.Enabled = true;
    SM83_INST_STOP(CPU);
    unit_assert_hex_eq(CPU->PC, 0x0001);
    // unit_assert_false(LCDC.Enabled);
    unit_assert_int_eq(CPU->internal->TotalTicks, 0);
}

UNIT_TEST(DI)
{
    CPU->IME = true;
    SM83_INST_DI(CPU);
    unit_assert_false(CPU->IME);
    unit_assert_int_eq(CPU->internal->TotalTicks, 0);
}

UNIT_TEST(EI)
{
    CPU->IME = false;
    SM83_INST_EI(CPU);
    unit_assert_false(CPU->IME);
    unit_assert_true(CPU->RequestEnableIME);
    unit_assert_int_eq(CPU->internal->TotalTicks, 0);
}

UNIT_TEST(DAA)
{
    // TODO
}

UNIT_TEST_SUITE(MISC)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(HALT);
	UNIT_RUN_TEST(STOP);
	UNIT_RUN_TEST(DI);
	UNIT_RUN_TEST(EI);
	UNIT_RUN_TEST(DAA);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(MISC);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}