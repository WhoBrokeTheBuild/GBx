#include "inst/misc.h"
#include "bootstrap.h"
#include "memory.h"
#include "video.h"
#include "minunit.h"

const uint16_t RAM_OFFSET = 0xCFFF;

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
}

MU_TEST(HALT)
{
    CPUEnabled = true;
    _HALT();
    mu_check(!CPUEnabled);
    mu_assert_int_eq(CPUTicks, 4);
}

MU_TEST(STOP)
{
    R.PC = 0x0000;
    LCDC.LCDEnable = true;
    _STOP();
    mu_assert_int_eq(R.PC, 0x0001);
    mu_check(!LCDC.LCDEnable);
    mu_assert_int_eq(CPUTicks, 4);
}

MU_TEST(DI)
{
    IME = true;
    _DI();
    mu_check(!IME);
    mu_assert_int_eq(CPUTicks, 4);
}

MU_TEST(EI)
{
    IME = false;
    _EI();
    mu_check(IME);
    mu_assert_int_eq(CPUTicks, 4);
}

MU_TEST(DAA)
{
    // TODO
}

MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&setup, NULL);

	MU_RUN_TEST(HALT);
	MU_RUN_TEST(STOP);
	MU_RUN_TEST(DI);
	MU_RUN_TEST(EI);
}

int main(int argc, char *argv[])
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}