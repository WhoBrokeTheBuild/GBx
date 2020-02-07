#include "inst/push.h"
#include "bootstrap.h"
#include "memory.h"
#include "minunit.h"

const uint16_t RAM_OFFSET = 0xCFFF;

void setup() 
{
    memset(&R, sizeof(R), 0);
    R.SP = 0xFFFE;
}

MU_TEST(PUSH_AF)
{
    CPUTicks = 0;
    R.AF = 0x1234;
    _PUSH_AF();
    mu_assert_int_eq(R.SP, 0xFFFC);
    mu_assert_int_eq(readWord(R.SP), 0x1234);
    mu_assert_int_eq(CPUTicks, 16);
}

MU_TEST(PUSH_BC)
{
    CPUTicks = 0;
    R.BC = 0x1234;
    _PUSH_BC();
    mu_assert_int_eq(R.SP, 0xFFFC);
    mu_assert_int_eq(readWord(R.SP), 0x1234);
    mu_assert_int_eq(CPUTicks, 16);
}

MU_TEST(PUSH_DE)
{
    CPUTicks = 0;
    R.DE = 0x1234;
    _PUSH_DE();
    mu_assert_int_eq(R.SP, 0xFFFC);
    mu_assert_int_eq(readWord(R.SP), 0x1234);
    mu_assert_int_eq(CPUTicks, 16);
}

MU_TEST(PUSH_HL)
{
    CPUTicks = 0;
    R.HL = 0x1234;
    _PUSH_HL();
    mu_assert_int_eq(R.SP, 0xFFFC);
    mu_assert_int_eq(readWord(R.SP), 0x1234);
    mu_assert_int_eq(CPUTicks, 16);
}

MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&setup, NULL);

	MU_RUN_TEST(PUSH_AF);
	MU_RUN_TEST(PUSH_BC);
	MU_RUN_TEST(PUSH_DE);
	MU_RUN_TEST(PUSH_HL);
}

int main(int argc, char *argv[])
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}