#include "inst/reset.h"
#include "clock.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xC100;

void setup() 
{
    Ticks = 0;
    memset(&R, sizeof(R), 0);
    R.HL = RAM_OFFSET;
}

#define MAKE_RES_REG_TEST(REG, BIT)                 \
    UNIT_TEST(RES_##BIT##_##REG)                    \
    {                                               \
        R.REG = 1 << BIT;                           \
        _RES_b_##REG(BIT);                          \
        unit_assert_hex_eq(0x00, R.REG);            \
    }

#define MAKE_RES_REG_TEST_LIST(REG)                 \
    MAKE_RES_REG_TEST(REG, 0)                           \
    MAKE_RES_REG_TEST(REG, 1)                           \
    MAKE_RES_REG_TEST(REG, 2)                           \
    MAKE_RES_REG_TEST(REG, 3)                           \
    MAKE_RES_REG_TEST(REG, 4)                           \
    MAKE_RES_REG_TEST(REG, 5)                           \
    MAKE_RES_REG_TEST(REG, 6)                           \
    MAKE_RES_REG_TEST(REG, 7)

MAKE_RES_REG_TEST_LIST(A);
MAKE_RES_REG_TEST_LIST(B);
MAKE_RES_REG_TEST_LIST(C);
MAKE_RES_REG_TEST_LIST(D);
MAKE_RES_REG_TEST_LIST(E);
MAKE_RES_REG_TEST_LIST(H);
MAKE_RES_REG_TEST_LIST(L);

#define MAKE_RES_pHL_TEST(RES)                          \
    UNIT_TEST(RES_##RES##_pHL)                          \
    {                                                   \
        writeByte(RAM_OFFSET, 1 << RES);                \
        _RES_b_pHL(RES);                                \
        unit_assert_hex_eq(0x00, readByte(RAM_OFFSET)); \
        unit_assert_int_eq(Ticks, 8);                \
    }

MAKE_RES_pHL_TEST(0)
MAKE_RES_pHL_TEST(1)
MAKE_RES_pHL_TEST(2)
MAKE_RES_pHL_TEST(3)
MAKE_RES_pHL_TEST(4)
MAKE_RES_pHL_TEST(5)
MAKE_RES_pHL_TEST(6)
MAKE_RES_pHL_TEST(7)

UNIT_TEST_SUITE(RES)
{
	UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(RES_0_A);
    UNIT_RUN_TEST(RES_0_B);
    UNIT_RUN_TEST(RES_0_C);
    UNIT_RUN_TEST(RES_0_D);
    UNIT_RUN_TEST(RES_0_E);
    UNIT_RUN_TEST(RES_0_H);
    UNIT_RUN_TEST(RES_0_L);
    UNIT_RUN_TEST(RES_0_pHL);

    UNIT_RUN_TEST(RES_1_A);
    UNIT_RUN_TEST(RES_1_B);
    UNIT_RUN_TEST(RES_1_C);
    UNIT_RUN_TEST(RES_1_D);
    UNIT_RUN_TEST(RES_1_E);
    UNIT_RUN_TEST(RES_1_H);
    UNIT_RUN_TEST(RES_1_L);
    UNIT_RUN_TEST(RES_1_pHL);

    UNIT_RUN_TEST(RES_2_A);
    UNIT_RUN_TEST(RES_2_B);
    UNIT_RUN_TEST(RES_2_C);
    UNIT_RUN_TEST(RES_2_D);
    UNIT_RUN_TEST(RES_2_E);
    UNIT_RUN_TEST(RES_2_H);
    UNIT_RUN_TEST(RES_2_L);
    UNIT_RUN_TEST(RES_2_pHL);

    UNIT_RUN_TEST(RES_3_A);
    UNIT_RUN_TEST(RES_3_B);
    UNIT_RUN_TEST(RES_3_C);
    UNIT_RUN_TEST(RES_3_D);
    UNIT_RUN_TEST(RES_3_E);
    UNIT_RUN_TEST(RES_3_H);
    UNIT_RUN_TEST(RES_3_L);
    UNIT_RUN_TEST(RES_3_pHL);

    UNIT_RUN_TEST(RES_4_A);
    UNIT_RUN_TEST(RES_4_B);
    UNIT_RUN_TEST(RES_4_C);
    UNIT_RUN_TEST(RES_4_D);
    UNIT_RUN_TEST(RES_4_E);
    UNIT_RUN_TEST(RES_4_H);
    UNIT_RUN_TEST(RES_4_L);
    UNIT_RUN_TEST(RES_4_pHL);

    UNIT_RUN_TEST(RES_5_A);
    UNIT_RUN_TEST(RES_5_B);
    UNIT_RUN_TEST(RES_5_C);
    UNIT_RUN_TEST(RES_5_D);
    UNIT_RUN_TEST(RES_5_E);
    UNIT_RUN_TEST(RES_5_H);
    UNIT_RUN_TEST(RES_5_L);
    UNIT_RUN_TEST(RES_5_pHL);

    UNIT_RUN_TEST(RES_6_A);
    UNIT_RUN_TEST(RES_6_B);
    UNIT_RUN_TEST(RES_6_C);
    UNIT_RUN_TEST(RES_6_D);
    UNIT_RUN_TEST(RES_6_E);
    UNIT_RUN_TEST(RES_6_H);
    UNIT_RUN_TEST(RES_6_L);
    UNIT_RUN_TEST(RES_6_pHL);

    UNIT_RUN_TEST(RES_7_A);
    UNIT_RUN_TEST(RES_7_B);
    UNIT_RUN_TEST(RES_7_C);
    UNIT_RUN_TEST(RES_7_D);
    UNIT_RUN_TEST(RES_7_E);
    UNIT_RUN_TEST(RES_7_H);
    UNIT_RUN_TEST(RES_7_L);
    UNIT_RUN_TEST(RES_7_pHL);
}

int main(int argc, char ** argv)
{
    DebugMode = true;
	UNIT_RUN_SUITE(RES);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}