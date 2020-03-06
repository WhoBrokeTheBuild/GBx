#include "inst/bit.h"
#include "clock.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xC100;

void setup() 
{
    TotalTicks = 0;
    memset(&R, sizeof(R), 0);
    R.HL = RAM_OFFSET;
}

#define MAKE_BIT_REG_TEST(REG, BIT)                 \
    UNIT_TEST(BIT_##BIT##_##REG)                    \
    {                                               \
        R.REG = 1 << BIT;                           \
        _BIT_b_##REG(BIT);                          \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_true(R.FH);                     \
                                                    \
        R.REG = 0;                                  \
        _BIT_b_##REG(BIT);                          \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_true(R.FH);                     \
    }

#define MAKE_BIT_REG_TEST_LIST(REG)                 \
    MAKE_BIT_REG_TEST(REG, 0)                           \
    MAKE_BIT_REG_TEST(REG, 1)                           \
    MAKE_BIT_REG_TEST(REG, 2)                           \
    MAKE_BIT_REG_TEST(REG, 3)                           \
    MAKE_BIT_REG_TEST(REG, 4)                           \
    MAKE_BIT_REG_TEST(REG, 5)                           \
    MAKE_BIT_REG_TEST(REG, 6)                           \
    MAKE_BIT_REG_TEST(REG, 7)

MAKE_BIT_REG_TEST_LIST(A);
MAKE_BIT_REG_TEST_LIST(B);
MAKE_BIT_REG_TEST_LIST(C);
MAKE_BIT_REG_TEST_LIST(D);
MAKE_BIT_REG_TEST_LIST(E);
MAKE_BIT_REG_TEST_LIST(H);
MAKE_BIT_REG_TEST_LIST(L);

#define MAKE_BIT_pHL_TEST(BIT)                      \
    UNIT_TEST(BIT_##BIT##_pHL)                      \
    {                                               \
        writeByte(RAM_OFFSET, 1 << BIT);            \
        _BIT_b_pHL(BIT);                            \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_true(R.FH);                     \
        unit_assert_int_eq(TotalTicks, 4);            \
                                                    \
        writeByte(RAM_OFFSET, 0);                   \
        _BIT_b_pHL(BIT);                            \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_true(R.FH);                     \
        unit_assert_int_eq(TotalTicks, 8);            \
    }

MAKE_BIT_pHL_TEST(0)
MAKE_BIT_pHL_TEST(1)
MAKE_BIT_pHL_TEST(2)
MAKE_BIT_pHL_TEST(3)
MAKE_BIT_pHL_TEST(4)
MAKE_BIT_pHL_TEST(5)
MAKE_BIT_pHL_TEST(6)
MAKE_BIT_pHL_TEST(7)

UNIT_TEST_SUITE(BIT)
{
	UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(BIT_0_A);
    UNIT_RUN_TEST(BIT_0_B);
    UNIT_RUN_TEST(BIT_0_C);
    UNIT_RUN_TEST(BIT_0_D);
    UNIT_RUN_TEST(BIT_0_E);
    UNIT_RUN_TEST(BIT_0_H);
    UNIT_RUN_TEST(BIT_0_L);
    UNIT_RUN_TEST(BIT_0_pHL);

    UNIT_RUN_TEST(BIT_1_A);
    UNIT_RUN_TEST(BIT_1_B);
    UNIT_RUN_TEST(BIT_1_C);
    UNIT_RUN_TEST(BIT_1_D);
    UNIT_RUN_TEST(BIT_1_E);
    UNIT_RUN_TEST(BIT_1_H);
    UNIT_RUN_TEST(BIT_1_L);
    UNIT_RUN_TEST(BIT_1_pHL);

    UNIT_RUN_TEST(BIT_2_A);
    UNIT_RUN_TEST(BIT_2_B);
    UNIT_RUN_TEST(BIT_2_C);
    UNIT_RUN_TEST(BIT_2_D);
    UNIT_RUN_TEST(BIT_2_E);
    UNIT_RUN_TEST(BIT_2_H);
    UNIT_RUN_TEST(BIT_2_L);
    UNIT_RUN_TEST(BIT_2_pHL);

    UNIT_RUN_TEST(BIT_3_A);
    UNIT_RUN_TEST(BIT_3_B);
    UNIT_RUN_TEST(BIT_3_C);
    UNIT_RUN_TEST(BIT_3_D);
    UNIT_RUN_TEST(BIT_3_E);
    UNIT_RUN_TEST(BIT_3_H);
    UNIT_RUN_TEST(BIT_3_L);
    UNIT_RUN_TEST(BIT_3_pHL);

    UNIT_RUN_TEST(BIT_4_A);
    UNIT_RUN_TEST(BIT_4_B);
    UNIT_RUN_TEST(BIT_4_C);
    UNIT_RUN_TEST(BIT_4_D);
    UNIT_RUN_TEST(BIT_4_E);
    UNIT_RUN_TEST(BIT_4_H);
    UNIT_RUN_TEST(BIT_4_L);
    UNIT_RUN_TEST(BIT_4_pHL);

    UNIT_RUN_TEST(BIT_5_A);
    UNIT_RUN_TEST(BIT_5_B);
    UNIT_RUN_TEST(BIT_5_C);
    UNIT_RUN_TEST(BIT_5_D);
    UNIT_RUN_TEST(BIT_5_E);
    UNIT_RUN_TEST(BIT_5_H);
    UNIT_RUN_TEST(BIT_5_L);
    UNIT_RUN_TEST(BIT_5_pHL);

    UNIT_RUN_TEST(BIT_6_A);
    UNIT_RUN_TEST(BIT_6_B);
    UNIT_RUN_TEST(BIT_6_C);
    UNIT_RUN_TEST(BIT_6_D);
    UNIT_RUN_TEST(BIT_6_E);
    UNIT_RUN_TEST(BIT_6_H);
    UNIT_RUN_TEST(BIT_6_L);
    UNIT_RUN_TEST(BIT_6_pHL);

    UNIT_RUN_TEST(BIT_7_A);
    UNIT_RUN_TEST(BIT_7_B);
    UNIT_RUN_TEST(BIT_7_C);
    UNIT_RUN_TEST(BIT_7_D);
    UNIT_RUN_TEST(BIT_7_E);
    UNIT_RUN_TEST(BIT_7_H);
    UNIT_RUN_TEST(BIT_7_L);
    UNIT_RUN_TEST(BIT_7_pHL);
}

int main(int argc, char ** argv)
{
    VerboseLevel = 4;
	UNIT_RUN_SUITE(BIT);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}