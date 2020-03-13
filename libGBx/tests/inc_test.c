#include "inst/inc.h"
#include "clock.h"
#include "memory.h"
#include "unit.h"

const word RAM_OFFSET = 0xC100;

void setup() 
{
    TotalTicks = 0;
    memset(&R, sizeof(R), 0);
    R.HL = RAM_OFFSET;
}

#define MAKE_INC_TEST8(REG)                         \
    UNIT_TEST(INC_##REG##_00)                       \
    {                                               \
        R.REG = 0x00;                               \
        _INC_##REG();                               \
        unit_assert_hex_eq(0x01, R.REG);            \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_int_eq(4, TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##REG##_0F)                       \
    {                                               \
        R.REG = 0x0F;                               \
        _INC_##REG();                               \
        unit_assert_hex_eq(0x10, R.REG);            \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_true(R.FH);                     \
        unit_assert_int_eq(4, TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##REG##_FF)                       \
    {                                               \
        R.REG = 0xFF;                               \
        _INC_##REG();                               \
        unit_assert_hex_eq(0x00, R.REG);            \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_true(R.FH);                     \
        unit_assert_int_eq(4, TotalTicks);            \
    }

MAKE_INC_TEST8(A);
MAKE_INC_TEST8(B);
MAKE_INC_TEST8(C);
MAKE_INC_TEST8(D);
MAKE_INC_TEST8(E);
MAKE_INC_TEST8(H);
MAKE_INC_TEST8(L);

UNIT_TEST(INC_pHL_00)
{
    WriteByte(R.HL, 0x00);
    _INC_pHL();
    unit_assert_hex_eq(0x01, ReadByte(R.HL));
    unit_assert_int_eq(12, TotalTicks);
}

UNIT_TEST(INC_pHL_FF)
{
    WriteByte(R.HL, 0xFF);
    _INC_pHL();
    unit_assert_hex_eq(0x00, ReadByte(R.HL));
    unit_assert_int_eq(12, TotalTicks);
}

UNIT_TEST_SUITE(INC8)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(INC_A_00);
    UNIT_RUN_TEST(INC_A_0F);
    UNIT_RUN_TEST(INC_A_FF);
	
    UNIT_RUN_TEST(INC_B_00);
    UNIT_RUN_TEST(INC_B_0F);
    UNIT_RUN_TEST(INC_B_FF);
	
    UNIT_RUN_TEST(INC_C_00);
    UNIT_RUN_TEST(INC_C_0F);
    UNIT_RUN_TEST(INC_C_FF);
	
    UNIT_RUN_TEST(INC_D_00);
    UNIT_RUN_TEST(INC_D_0F);
    UNIT_RUN_TEST(INC_D_FF);
	
    UNIT_RUN_TEST(INC_E_00);
    UNIT_RUN_TEST(INC_E_0F);
    UNIT_RUN_TEST(INC_E_FF);
	
    UNIT_RUN_TEST(INC_H_00);
    UNIT_RUN_TEST(INC_H_0F);
    UNIT_RUN_TEST(INC_H_FF);

	UNIT_RUN_TEST(INC_L_00);
    UNIT_RUN_TEST(INC_L_0F);
    UNIT_RUN_TEST(INC_L_FF);

	UNIT_RUN_TEST(INC_pHL_00);
	UNIT_RUN_TEST(INC_pHL_FF);
}

#define MAKE_INC_TEST16(REG)                        \
    UNIT_TEST(INC_##REG##_0000)                     \
    {                                               \
        R.REG = 0x0000;                             \
        _INC_##REG();                               \
        unit_assert_hex_eq(0x0001, R.REG);          \
        unit_assert_int_eq(8, TotalTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##REG##_FFFF)                     \
    {                                               \
        R.REG = 0xFFFF;                             \
        _INC_##REG();                               \
        unit_assert_hex_eq(0x0000, R.REG);          \
        unit_assert_int_eq(8, TotalTicks);            \
    }

MAKE_INC_TEST16(BC);
MAKE_INC_TEST16(DE);
MAKE_INC_TEST16(HL);
MAKE_INC_TEST16(SP);

UNIT_TEST_SUITE(INC16)
{
	UNIT_SUITE_SETUP(&setup);

	UNIT_RUN_TEST(INC_DE_0000);
    UNIT_RUN_TEST(INC_DE_FFFF);
    
	UNIT_RUN_TEST(INC_BC_0000);
    UNIT_RUN_TEST(INC_BC_FFFF);

	UNIT_RUN_TEST(INC_HL_0000);
    UNIT_RUN_TEST(INC_HL_FFFF);

	UNIT_RUN_TEST(INC_SP_0000);
    UNIT_RUN_TEST(INC_SP_FFFF);
}

int main(int argc, char ** argv)
{
    VerboseLevel = 4;
	UNIT_RUN_SUITE(INC8);
	UNIT_RUN_SUITE(INC16);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}