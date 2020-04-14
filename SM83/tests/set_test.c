#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup()
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->HL = 0x1234;
}

#define MAKE_SET_REG_TEST(REG, BIT)                 \
    UNIT_TEST(SET_##BIT##_##REG)                    \
    {                                               \
        CPU->REG = 0;                                  \
        SM83_INST_SET_b_##REG(CPU, BIT);                          \
        unit_assert_hex_eq(1 << BIT, CPU->REG);        \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);  \
    }

#define MAKE_SET_REG_TEST_LIST(REG)                 \
    MAKE_SET_REG_TEST(REG, 0)                       \
    MAKE_SET_REG_TEST(REG, 1)                       \
    MAKE_SET_REG_TEST(REG, 2)                       \
    MAKE_SET_REG_TEST(REG, 3)                       \
    MAKE_SET_REG_TEST(REG, 4)                       \
    MAKE_SET_REG_TEST(REG, 5)                       \
    MAKE_SET_REG_TEST(REG, 6)                       \
    MAKE_SET_REG_TEST(REG, 7)

MAKE_SET_REG_TEST_LIST(A);
MAKE_SET_REG_TEST_LIST(B);
MAKE_SET_REG_TEST_LIST(C);
MAKE_SET_REG_TEST_LIST(D);
MAKE_SET_REG_TEST_LIST(E);
MAKE_SET_REG_TEST_LIST(H);
MAKE_SET_REG_TEST_LIST(L);

#define MAKE_SET_pHL_TEST(BIT)                              \
    UNIT_TEST(SET_##BIT##_pHL)                              \
    {                                                       \
        writeByte(0x1234, 0);                           \
        SM83_INST_SET_b_pHL(CPU, BIT);                                    \
        unit_assert_hex_eq(1 << BIT, readByte(0x1234)); \
        unit_assert_int_eq(2, CPU->internal->TotalTicks);                    \
    }

MAKE_SET_pHL_TEST(0)
MAKE_SET_pHL_TEST(1)
MAKE_SET_pHL_TEST(2)
MAKE_SET_pHL_TEST(3)
MAKE_SET_pHL_TEST(4)
MAKE_SET_pHL_TEST(5)
MAKE_SET_pHL_TEST(6)
MAKE_SET_pHL_TEST(7)

UNIT_TEST_SUITE(SET)
{
	UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(SET_0_A);
    UNIT_RUN_TEST(SET_0_B);
    UNIT_RUN_TEST(SET_0_C);
    UNIT_RUN_TEST(SET_0_D);
    UNIT_RUN_TEST(SET_0_E);
    UNIT_RUN_TEST(SET_0_H);
    UNIT_RUN_TEST(SET_0_L);
    UNIT_RUN_TEST(SET_0_pHL);

    UNIT_RUN_TEST(SET_1_A);
    UNIT_RUN_TEST(SET_1_B);
    UNIT_RUN_TEST(SET_1_C);
    UNIT_RUN_TEST(SET_1_D);
    UNIT_RUN_TEST(SET_1_E);
    UNIT_RUN_TEST(SET_1_H);
    UNIT_RUN_TEST(SET_1_L);
    UNIT_RUN_TEST(SET_1_pHL);

    UNIT_RUN_TEST(SET_2_A);
    UNIT_RUN_TEST(SET_2_B);
    UNIT_RUN_TEST(SET_2_C);
    UNIT_RUN_TEST(SET_2_D);
    UNIT_RUN_TEST(SET_2_E);
    UNIT_RUN_TEST(SET_2_H);
    UNIT_RUN_TEST(SET_2_L);
    UNIT_RUN_TEST(SET_2_pHL);

    UNIT_RUN_TEST(SET_3_A);
    UNIT_RUN_TEST(SET_3_B);
    UNIT_RUN_TEST(SET_3_C);
    UNIT_RUN_TEST(SET_3_D);
    UNIT_RUN_TEST(SET_3_E);
    UNIT_RUN_TEST(SET_3_H);
    UNIT_RUN_TEST(SET_3_L);
    UNIT_RUN_TEST(SET_3_pHL);

    UNIT_RUN_TEST(SET_4_A);
    UNIT_RUN_TEST(SET_4_B);
    UNIT_RUN_TEST(SET_4_C);
    UNIT_RUN_TEST(SET_4_D);
    UNIT_RUN_TEST(SET_4_E);
    UNIT_RUN_TEST(SET_4_H);
    UNIT_RUN_TEST(SET_4_L);
    UNIT_RUN_TEST(SET_4_pHL);

    UNIT_RUN_TEST(SET_5_A);
    UNIT_RUN_TEST(SET_5_B);
    UNIT_RUN_TEST(SET_5_C);
    UNIT_RUN_TEST(SET_5_D);
    UNIT_RUN_TEST(SET_5_E);
    UNIT_RUN_TEST(SET_5_H);
    UNIT_RUN_TEST(SET_5_L);
    UNIT_RUN_TEST(SET_5_pHL);

    UNIT_RUN_TEST(SET_6_A);
    UNIT_RUN_TEST(SET_6_B);
    UNIT_RUN_TEST(SET_6_C);
    UNIT_RUN_TEST(SET_6_D);
    UNIT_RUN_TEST(SET_6_E);
    UNIT_RUN_TEST(SET_6_H);
    UNIT_RUN_TEST(SET_6_L);
    UNIT_RUN_TEST(SET_6_pHL);

    UNIT_RUN_TEST(SET_7_A);
    UNIT_RUN_TEST(SET_7_B);
    UNIT_RUN_TEST(SET_7_C);
    UNIT_RUN_TEST(SET_7_D);
    UNIT_RUN_TEST(SET_7_E);
    UNIT_RUN_TEST(SET_7_H);
    UNIT_RUN_TEST(SET_7_L);
    UNIT_RUN_TEST(SET_7_pHL);
}

int main(int argc, char ** argv)
{
    stub_init();

	UNIT_RUN_SUITE(SET);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}