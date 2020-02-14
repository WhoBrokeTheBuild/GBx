#include "inst/inc.h"
#include "bootstrap.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xCFFF;

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
}

#define MAKE_INC_TEST8(_REG_)                       \
    UNIT_TEST(INC_##_REG_##_00)                     \
    {                                               \
        R._REG_ = 0x00;                             \
        _INC_##_REG_();                             \
        unit_assert_hex_eq(0x01, R._REG_);          \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_int_eq(4, CPUTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##_REG_##_0F)                     \
    {                                               \
        R._REG_ = 0x0F;                             \
        _INC_##_REG_();                             \
        unit_assert_hex_eq(0x10, R._REG_);          \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_true(R.FH);                     \
        unit_assert_int_eq(4, CPUTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##_REG_##_FF)                     \
    {                                               \
        R._REG_ = 0xFF;                             \
        _INC_##_REG_();                             \
        unit_assert_hex_eq(0x00, R._REG_);          \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_true(R.FH);                     \
        unit_assert_int_eq(4, CPUTicks);            \
    }

#define MAKE_INC_TEST16(_REG_)                      \
    UNIT_TEST(INC_##_REG_##_0000)                   \
    {                                               \
        R._REG_ = 0x0000;                           \
        _INC_##_REG_();                             \
        unit_assert_hex_eq(0x0001, R._REG_);        \
        unit_assert_int_eq(8, CPUTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(INC_##_REG_##_FFFF)                   \
    {                                               \
        R._REG_ = 0xFFFF;                           \
        _INC_##_REG_();                             \
        unit_assert_hex_eq(0x0000, R._REG_);        \
        unit_assert_int_eq(8, CPUTicks);            \
    }

MAKE_INC_TEST8(A);
MAKE_INC_TEST8(B);
MAKE_INC_TEST8(C);
MAKE_INC_TEST8(D);
MAKE_INC_TEST8(E);
MAKE_INC_TEST8(H);
MAKE_INC_TEST8(L);

MAKE_INC_TEST16(BC);
MAKE_INC_TEST16(DE);
MAKE_INC_TEST16(HL);
MAKE_INC_TEST16(SP);

UNIT_TEST(INC_pHL_0000)
{
    R.HL = RAM_OFFSET;
    writeByte(RAM_OFFSET, 0x00);
    _INC_pHL();
    unit_assert_hex_eq(0x01, readByte(RAM_OFFSET));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(INC_pHL_FFFF)
{
    R.HL = RAM_OFFSET;
    writeByte(RAM_OFFSET, 0xFF);
    _INC_pHL();
    unit_assert_hex_eq(0x00, readByte(RAM_OFFSET));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST_SUITE(inc_suite)
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

	UNIT_RUN_TEST(INC_DE_0000);
    UNIT_RUN_TEST(INC_DE_FFFF);
    
	UNIT_RUN_TEST(INC_BC_0000);
    UNIT_RUN_TEST(INC_BC_FFFF);

	UNIT_RUN_TEST(INC_HL_0000);
    UNIT_RUN_TEST(INC_HL_FFFF);

	UNIT_RUN_TEST(INC_SP_0000);
    UNIT_RUN_TEST(INC_SP_FFFF);

	UNIT_RUN_TEST(INC_pHL_0000);
	UNIT_RUN_TEST(INC_pHL_FFFF);
}

int main(int argc, char *argv[])
{
    DebugMode = true;
	UNIT_RUN_SUITE(inc_suite);
	UNIT_REPORT();
	return UNIT_EXIT_CODE;
}