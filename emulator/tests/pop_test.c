#include "inst/pop.h"
#include "bootstrap.h"
#include "memory.h"
#include "minunit.h"

const uint16_t RAM_OFFSET = 0xCFFF;

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
    R.SP = 0xFFFE;
}

MU_TEST(POP_AF)
{
    pushWord(0x1234);
    _POP_AF();
    mu_assert_int_eq(R.SP, 0xFFFE);
    mu_assert_int_eq(R.AF, 0x1234);
    mu_assert_int_eq(CPUTicks, 12);
}

MU_TEST(POP_BC)
{
    pushWord(0x1234);
    _POP_BC();
    mu_assert_int_eq(R.SP, 0xFFFE);
    mu_assert_int_eq(R.BC, 0x1234);
    mu_assert_int_eq(CPUTicks, 12);
}

MU_TEST(POP_DE)
{
    pushWord(0x1234);
    _POP_DE();
    mu_assert_int_eq(R.SP, 0xFFFE);
    mu_assert_int_eq(R.DE, 0x1234);
    mu_assert_int_eq(CPUTicks, 12);
}

MU_TEST(POP_HL)
{
    pushWord(0x1234);
    _POP_HL();
    mu_assert_int_eq(R.SP, 0xFFFE);
    mu_assert_int_eq(R.HL, 0x1234);
    mu_assert_int_eq(CPUTicks, 12);
}

MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&setup, NULL);

	MU_RUN_TEST(POP_AF);
	MU_RUN_TEST(POP_BC);
	MU_RUN_TEST(POP_DE);
	MU_RUN_TEST(POP_HL);
}

int main(int argc, char *argv[])
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}