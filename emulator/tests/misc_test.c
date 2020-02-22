#include "inst/misc.h"
#include "clock.h"
#include "memory.h"
#include "unit.h"

void setup() 
{
    Ticks = 0;
    memset(&R, sizeof(R), 0);
}

UNIT_TEST(HALT)
{
    CPUEnabled = true;
    _HALT();
    unit_assert_false(CPUEnabled);
    unit_assert_int_eq(Ticks, 0);
}

UNIT_TEST(STOP)
{
    R.PC = 0x0000;
    LCDC.LCDEnable = true;
    _STOP();
    unit_assert_hex_eq(R.PC, 0x0001);
    unit_assert_false(LCDC.LCDEnable);
    unit_assert_int_eq(Ticks, 0);
}

UNIT_TEST(DI)
{
    IME = true;
    _DI();
    unit_assert_false(IME);
    unit_assert_int_eq(Ticks, 0);
}

UNIT_TEST(EI)
{
    IME = false;
    _EI();
    unit_assert_true(IME);
    unit_assert_int_eq(Ticks, 0);
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
    VerboseLevel = 4;
	UNIT_RUN_SUITE(MISC);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}