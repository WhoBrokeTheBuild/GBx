#include "inst/inc.h"
#include "bootstrap.h"
#include "memory.h"
#include "minunit.h"

const uint16_t RAM_OFFSET = 0xCFFF;

void setup() 
{
    memset(&R, sizeof(R), 0);
}

#define MAKE_INC_TEST8(_REG_)               \
    MU_TEST(INC_##_REG_##_00)               \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0x00;                     \
        _INC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0x01);    \
        mu_check(!R.FZ);                    \
        mu_check(!R.FN);                    \
        mu_check(!R.FH);                    \
        mu_assert_int_eq(CPUTicks, 4);      \
    }                                       \
                                            \
    MU_TEST(INC_##_REG_##_0F)               \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0x0F;                     \
        _INC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0x10);    \
        mu_check(!R.FZ);                    \
        mu_check(!R.FN);                    \
        mu_check(R.FH);                     \
        mu_assert_int_eq(CPUTicks, 4);      \
    }                                       \
                                            \
    MU_TEST(INC_##_REG_##_FF)               \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0xFF;                     \
        _INC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0x00);    \
        mu_check(R.FZ);                     \
        mu_check(!R.FN);                    \
        mu_check(R.FH);                     \
        mu_assert_int_eq(CPUTicks, 4);      \
    }

#define MAKE_INC_TEST16(_REG_)              \
    MU_TEST(INC_##_REG_##_0000)             \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0x0000;                   \
        _INC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0x0001);  \
        mu_assert_int_eq(CPUTicks, 8);      \
    }                                       \
                                            \
    MU_TEST(INC_##_REG_##_FFFF)             \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0xFFFF;                   \
        _INC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0x0000);  \
        mu_assert_int_eq(CPUTicks, 8);      \
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

MU_TEST(INC_pHL_0000)
{
    CPUTicks = 0;
    R.HL = RAM_OFFSET;
    writeWord(RAM_OFFSET, 0x00);
    _INC_pHL();
    mu_assert_int_eq(readWord(RAM_OFFSET), 0x01);
    mu_assert_int_eq(CPUTicks, 12);
}

MU_TEST(INC_pHL_FFFF)
{
    CPUTicks = 0;
    R.HL = RAM_OFFSET;
    writeByte(RAM_OFFSET, 0xFF);
    _INC_pHL();
    mu_assert_int_eq(readByte(RAM_OFFSET), 0x00);
    mu_assert_int_eq(CPUTicks, 12);
}

MU_TEST_SUITE(test_suite)
{
	MU_SUITE_CONFIGURE(&setup, NULL);

	MU_RUN_TEST(INC_A_00);
    MU_RUN_TEST(INC_A_0F);
    MU_RUN_TEST(INC_A_FF);
	
    MU_RUN_TEST(INC_B_00);
    MU_RUN_TEST(INC_B_0F);
    MU_RUN_TEST(INC_B_FF);
	
    MU_RUN_TEST(INC_C_00);
    MU_RUN_TEST(INC_C_0F);
    MU_RUN_TEST(INC_C_FF);
	
    MU_RUN_TEST(INC_D_00);
    MU_RUN_TEST(INC_D_0F);
    MU_RUN_TEST(INC_D_FF);
	
    MU_RUN_TEST(INC_E_00);
    MU_RUN_TEST(INC_E_0F);
    MU_RUN_TEST(INC_E_FF);
	
    MU_RUN_TEST(INC_H_00);
    MU_RUN_TEST(INC_H_0F);
    MU_RUN_TEST(INC_H_FF);

	MU_RUN_TEST(INC_L_00);
    MU_RUN_TEST(INC_L_0F);
    MU_RUN_TEST(INC_L_FF);

	MU_RUN_TEST(INC_DE_0000);
    MU_RUN_TEST(INC_DE_FFFF);
    
	MU_RUN_TEST(INC_BC_0000);
    MU_RUN_TEST(INC_BC_FFFF);

	MU_RUN_TEST(INC_HL_0000);
    MU_RUN_TEST(INC_HL_FFFF);

	MU_RUN_TEST(INC_SP_0000);
    MU_RUN_TEST(INC_SP_FFFF);

	MU_RUN_TEST(INC_pHL_0000);
	MU_RUN_TEST(INC_pHL_FFFF);
}

int main(int argc, char *argv[])
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}