#include "inst/return.h"
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

MU_TEST(RET)
{
    pushWord(0x1234);
    _RET();
    mu_assert_int_eq(R.PC, 0x1234);
    mu_assert_int_eq(CPUTicks, 16);
}

MU_TEST(RET_NZ)
{
    pushWord(0x1234);
    R.PC = 0x0000;
    R.FZ = true;
    _RET_NZ();
    mu_assert_int_eq(R.PC, 0x0000);
    mu_assert_int_eq(CPUTicks, 8);

    R.FZ = false;
    _RET_NZ();
    mu_assert_int_eq(R.PC, 0x1234);
    mu_assert_int_eq(CPUTicks, 8 + 20);
}

MU_TEST(RET_Z)
{
    pushWord(0x1234);
    R.PC = 0x0000;
    R.FZ = false;
    _RET_Z();
    mu_assert_int_eq(R.PC, 0x0000);
    mu_assert_int_eq(CPUTicks, 8);

    R.FZ = true;
    _RET_Z();
    mu_assert_int_eq(R.PC, 0x1234);
    mu_assert_int_eq(CPUTicks, 8 + 20);
}

MU_TEST(RET_NC)
{
    pushWord(0x1234);
    R.PC = 0x0000;
    R.FC = true;
    _RET_NC();
    mu_assert_int_eq(R.PC, 0x0000);
    mu_assert_int_eq(CPUTicks, 8);

    R.FC = false;
    _RET_NC();
    mu_assert_int_eq(R.PC, 0x1234);
    mu_assert_int_eq(CPUTicks, 8 + 20);
}

MU_TEST(RET_C)
{
    pushWord(0x1234);
    R.PC = 0x0000;
    R.FC = false;
    _RET_C();
    mu_assert_int_eq(R.PC, 0x0000);
    mu_assert_int_eq(CPUTicks, 8);

    R.FC = true;
    _RET_C();
    mu_assert_int_eq(R.PC, 0x1234);
    mu_assert_int_eq(CPUTicks, 8 + 20);
}

MU_TEST(RETI)
{
    pushWord(0x1234);
    R.PC = 0x0000;
    _RETI();
    mu_assert_int_eq(R.PC, 0x1234);
    mu_assert_int_eq(CPUTicks, 16);
}

MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&setup, NULL);

	MU_RUN_TEST(RET);
    MU_RUN_TEST(RET_NZ);
    MU_RUN_TEST(RET_Z);
    MU_RUN_TEST(RET_NC);
    MU_RUN_TEST(RET_C);
    MU_RUN_TEST(RETI);
}

int main(int argc, char *argv[])
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}