#include "inst/subtract.h"
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

#define MAKE_SUB_TEST(REG)                          \
    UNIT_TEST(SUB_##REG##_00_00)                    \
    {                                               \
        R.A = 0x00;                                 \
        R.REG = 0x00;                               \
        _SUB_##REG();                               \
        unit_assert_hex_eq(0x00, R.A);              \
        unit_assert_true(R.FZ);                     \
        unit_assert_true(R.FN);                     \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
    }                                               \
                                                    \
    UNIT_TEST(SUB_##REG##_00_FF)                    \
    {                                               \
        R.A = 0x00;                                 \
        R.REG = 0xFF;                               \
        _SUB_##REG();                               \
        unit_assert_hex_eq(0x01, R.A);              \
        unit_assert_false(R.FZ);                    \
        unit_assert_true(R.FN);                     \
        unit_assert_true(R.FH);                     \
        unit_assert_true(R.FC);                     \
    }                                               \
                                                    \
    UNIT_TEST(SUB_##REG##_AA_BB)                    \
    {                                               \
        R.A = 0xAA;                                 \
        R.REG = 0xBB;                               \
        _SUB_##REG();                               \
        unit_assert_hex_eq(0xEF, R.A);              \
        unit_assert_false(R.FZ);                    \
        unit_assert_true(R.FN);                     \
        unit_assert_true(R.FH);                     \
        unit_assert_true(R.FC);                     \
    }                                               \
                                                    \
    UNIT_TEST(SUB_##REG##_BB_AA)                    \
    {                                               \
        R.A = 0xBB;                                 \
        R.REG = 0xAA;                               \
        _SUB_##REG();                               \
        unit_assert_hex_eq(0x11, R.A);              \
        unit_assert_false(R.FZ);                    \
        unit_assert_true(R.FN);                     \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
    }

UNIT_TEST(SUB_A_42)
{
    R.A = 0x34;
    _SUB_A();
    unit_assert_hex_eq(0x00, R.A);
    unit_assert_true(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
}

MAKE_SUB_TEST(B);
MAKE_SUB_TEST(C);
MAKE_SUB_TEST(D);
MAKE_SUB_TEST(E);
MAKE_SUB_TEST(H);
MAKE_SUB_TEST(L);

UNIT_TEST(SUB_pHL_00_00)
{
    R.A = 0x00;
    writeByte(R.HL, 0x00);
    _SUB_pHL();
    unit_assert_hex_eq(0x00, R.A);
    unit_assert_true(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
}

UNIT_TEST(SUB_pHL_00_FF)
{
    R.A = 0x00;
    writeByte(R.HL, 0xFF);
    _SUB_pHL();
    unit_assert_hex_eq(0x01, R.A);
    unit_assert_false(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_true(R.FH);
    unit_assert_true(R.FC);
}

UNIT_TEST(SUB_pHL_AA_BB)
{
    R.A = 0xAA;
    writeByte(R.HL, 0xBB);
    _SUB_pHL();
    unit_assert_hex_eq(0xEF, R.A);
    unit_assert_false(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_true(R.FH);
    unit_assert_true(R.FC);
}

UNIT_TEST(SUB_pHL_BB_AA)
{
    R.A = 0xBB;
    writeByte(R.HL, 0xAA);
    _SUB_pHL();
    unit_assert_hex_eq(0x11, R.A);
    unit_assert_false(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
}

UNIT_TEST_SUITE(CP)
{
	UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(SUB_A_42);

    UNIT_RUN_TEST(SUB_B_00_00);
    UNIT_RUN_TEST(SUB_B_00_FF);
    UNIT_RUN_TEST(SUB_B_AA_BB);
    UNIT_RUN_TEST(SUB_B_BB_AA);
	
    UNIT_RUN_TEST(SUB_C_00_00);
    UNIT_RUN_TEST(SUB_C_00_FF);
    UNIT_RUN_TEST(SUB_C_AA_BB);
    UNIT_RUN_TEST(SUB_C_BB_AA);
	
    UNIT_RUN_TEST(SUB_D_00_00);
    UNIT_RUN_TEST(SUB_D_00_FF);
    UNIT_RUN_TEST(SUB_D_AA_BB);
    UNIT_RUN_TEST(SUB_D_BB_AA);
	
    UNIT_RUN_TEST(SUB_E_00_00);
    UNIT_RUN_TEST(SUB_E_00_FF);
    UNIT_RUN_TEST(SUB_E_AA_BB);
    UNIT_RUN_TEST(SUB_E_BB_AA);
	
    UNIT_RUN_TEST(SUB_H_00_00);
    UNIT_RUN_TEST(SUB_H_00_FF);
    UNIT_RUN_TEST(SUB_H_AA_BB);
    UNIT_RUN_TEST(SUB_H_BB_AA);

	UNIT_RUN_TEST(SUB_L_00_00);
	UNIT_RUN_TEST(SUB_L_00_FF);
	UNIT_RUN_TEST(SUB_L_AA_BB);
	UNIT_RUN_TEST(SUB_L_BB_AA);

	UNIT_RUN_TEST(SUB_pHL_00_00);
	UNIT_RUN_TEST(SUB_pHL_00_FF);
	UNIT_RUN_TEST(SUB_pHL_AA_BB);
	UNIT_RUN_TEST(SUB_pHL_BB_AA);
}

int main(int argc, char ** argv)
{
    VerboseLevel = 4;
	UNIT_RUN_SUITE(CP);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}