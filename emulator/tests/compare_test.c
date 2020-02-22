#include "inst/compare.h"
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

#define MAKE_CP_TEST(REG)                           \
    UNIT_TEST(CP_##REG##_00_00)                     \
    {                                               \
        R.A = 0x00;                                 \
        R.REG = 0x00;                               \
        _CP_##REG();                                \
        unit_assert_true(R.FZ);                     \
        unit_assert_true(R.FN);                     \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
    }                                               \
                                                    \
    UNIT_TEST(CP_##REG##_00_FF)                     \
    {                                               \
        R.A = 0x00;                                 \
        R.REG = 0xFF;                               \
        _CP_##REG();                                \
        unit_assert_false(R.FZ);                    \
        unit_assert_true(R.FN);                     \
        unit_assert_true(R.FH);                     \
        unit_assert_true(R.FC);                     \
    }                                               \
                                                    \
    UNIT_TEST(CP_##REG##_AA_BB)                     \
    {                                               \
        R.A = 0xAA;                                 \
        R.REG = 0xBB;                               \
        _CP_##REG();                                \
        unit_assert_false(R.FZ);                    \
        unit_assert_true(R.FN);                     \
        unit_assert_true(R.FH);                     \
        unit_assert_true(R.FC);                     \
    }                                               \
                                                    \
    UNIT_TEST(CP_##REG##_BB_AA)                     \
    {                                               \
        R.A = 0xBB;                                 \
        R.REG = 0xAA;                               \
        _CP_##REG();                                \
        unit_assert_false(R.FZ);                    \
        unit_assert_true(R.FN);                     \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
    }

UNIT_TEST(CP_A_42)
{
    R.A = 0x34;
    _CP_A();
    unit_assert_true(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
}

MAKE_CP_TEST(B);
MAKE_CP_TEST(C);
MAKE_CP_TEST(D);
MAKE_CP_TEST(E);
MAKE_CP_TEST(H);
MAKE_CP_TEST(L);

UNIT_TEST(CP_pHL_00_00)
{
    R.A = 0x00;
    writeByte(R.HL, 0x00);
    _CP_pHL();
    unit_assert_true(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
}

UNIT_TEST(CP_pHL_00_FF)
{
    R.A = 0x00;
    writeByte(R.HL, 0xFF);
    _CP_pHL();
    unit_assert_false(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_true(R.FH);
    unit_assert_true(R.FC);
}

UNIT_TEST(CP_pHL_AA_BB)
{
    R.A = 0xAA;
    writeByte(R.HL, 0xBB);
    _CP_pHL();
    unit_assert_false(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_true(R.FH);
    unit_assert_true(R.FC);
}

UNIT_TEST(CP_pHL_BB_AA)
{
    R.A = 0xBB;
    writeByte(R.HL, 0xAA);
    _CP_pHL();
    unit_assert_false(R.FZ);
    unit_assert_true(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
}

UNIT_TEST_SUITE(CP)
{
	UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(CP_A_42);

    UNIT_RUN_TEST(CP_B_00_00);
    UNIT_RUN_TEST(CP_B_00_FF);
    UNIT_RUN_TEST(CP_B_AA_BB);
    UNIT_RUN_TEST(CP_B_BB_AA);
	
    UNIT_RUN_TEST(CP_C_00_00);
    UNIT_RUN_TEST(CP_C_00_FF);
    UNIT_RUN_TEST(CP_C_AA_BB);
    UNIT_RUN_TEST(CP_C_BB_AA);
	
    UNIT_RUN_TEST(CP_D_00_00);
    UNIT_RUN_TEST(CP_D_00_FF);
    UNIT_RUN_TEST(CP_D_AA_BB);
    UNIT_RUN_TEST(CP_D_BB_AA);
	
    UNIT_RUN_TEST(CP_E_00_00);
    UNIT_RUN_TEST(CP_E_00_FF);
    UNIT_RUN_TEST(CP_E_AA_BB);
    UNIT_RUN_TEST(CP_E_BB_AA);
	
    UNIT_RUN_TEST(CP_H_00_00);
    UNIT_RUN_TEST(CP_H_00_FF);
    UNIT_RUN_TEST(CP_H_AA_BB);
    UNIT_RUN_TEST(CP_H_BB_AA);

	UNIT_RUN_TEST(CP_L_00_00);
	UNIT_RUN_TEST(CP_L_00_FF);
	UNIT_RUN_TEST(CP_L_AA_BB);
	UNIT_RUN_TEST(CP_L_BB_AA);

	UNIT_RUN_TEST(CP_pHL_00_00);
	UNIT_RUN_TEST(CP_pHL_00_FF);
	UNIT_RUN_TEST(CP_pHL_AA_BB);
	UNIT_RUN_TEST(CP_pHL_BB_AA);
}

int main(int argc, char ** argv)
{
    VerboseLevel = 4;
	UNIT_RUN_SUITE(CP);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}