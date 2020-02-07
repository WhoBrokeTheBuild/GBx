#include "inst/dec.h"
#include "bootstrap.h"
#include "memory.h"
#include "minunit.h"

const uint16_t RAM_OFFSET = 0xCFFF;

void setup() 
{
    memset(&R, sizeof(R), 0);
}

#define MAKE_DEC_TEST8(_REG_)               \
    MU_TEST(DEC_##_REG_##_01)               \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0x01;                     \
        _DEC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0x00);    \
        mu_check(R.FZ);                     \
        mu_check(R.FN);                     \
        mu_check(R.FH);                     \
        mu_assert_int_eq(CPUTicks, 4);      \
    }                                       \
                                            \
    MU_TEST(DEC_##_REG_##_F1)               \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0xF1;                     \
        _DEC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0xF0);    \
        mu_check(!R.FZ);                     \
        mu_check(R.FN);                     \
        mu_check(R.FH);                     \
        mu_assert_int_eq(CPUTicks, 4);      \
    }                                       \
                                            \
    MU_TEST(DEC_##_REG_##_00)               \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0x00;                     \
        _DEC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0xFF);    \
        mu_check(!R.FZ);                    \
        mu_check(R.FN);                     \
        mu_check(!R.FH);                    \
        mu_assert_int_eq(CPUTicks, 4);      \
    }

#define MAKE_DEC_TEST16(_REG_)              \
    MU_TEST(DEC_##_REG_##_0001)             \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0x0001;                   \
        _DEC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0x0000);  \
        mu_assert_int_eq(CPUTicks, 8);      \
    }                                       \
                                            \
    MU_TEST(DEC_##_REG_##_0000)             \
    {                                       \
        CPUTicks = 0;                       \
        R._REG_ = 0x0000;                   \
        _DEC_##_REG_();                     \
        mu_assert_int_eq(R._REG_, 0xFFFF);  \
        mu_assert_int_eq(CPUTicks, 8);      \
    }

MAKE_DEC_TEST8(A);
MAKE_DEC_TEST8(B);
MAKE_DEC_TEST8(C);
MAKE_DEC_TEST8(D);
MAKE_DEC_TEST8(E);
MAKE_DEC_TEST8(H);
MAKE_DEC_TEST8(L);

MAKE_DEC_TEST16(BC);
MAKE_DEC_TEST16(DE);
MAKE_DEC_TEST16(HL);
MAKE_DEC_TEST16(SP);

MU_TEST(DEC_pHL_0001)
{
    CPUTicks = 0;
    R.HL = RAM_OFFSET;
    writeWord(RAM_OFFSET, 0x01);
    _DEC_pHL();
    mu_assert_int_eq(readWord(RAM_OFFSET), 0x00);
    mu_assert_int_eq(CPUTicks, 12);
}

MU_TEST(DEC_pHL_0000)
{
    CPUTicks = 0;
    R.HL = RAM_OFFSET;
    writeByte(RAM_OFFSET, 0x00);
    _DEC_pHL();
    mu_assert_int_eq(readByte(RAM_OFFSET), 0xFF);
    mu_assert_int_eq(CPUTicks, 12);
}

MU_TEST_SUITE(test_suite)
{
    MU_SUITE_CONFIGURE(&setup, NULL);

    MU_RUN_TEST(DEC_A_01);
    MU_RUN_TEST(DEC_A_F1);
    MU_RUN_TEST(DEC_A_00);
    
    MU_RUN_TEST(DEC_B_01);
    MU_RUN_TEST(DEC_B_F1);
    MU_RUN_TEST(DEC_B_00);
    
    MU_RUN_TEST(DEC_C_01);
    MU_RUN_TEST(DEC_C_F1);
    MU_RUN_TEST(DEC_C_00);
    
    MU_RUN_TEST(DEC_D_01);
    MU_RUN_TEST(DEC_D_F1);
    MU_RUN_TEST(DEC_D_00);
    
    MU_RUN_TEST(DEC_E_01);
    MU_RUN_TEST(DEC_E_F1);
    MU_RUN_TEST(DEC_E_00);
    
    MU_RUN_TEST(DEC_H_01);
    MU_RUN_TEST(DEC_H_F1);
    MU_RUN_TEST(DEC_H_00);

    MU_RUN_TEST(DEC_L_01);
    MU_RUN_TEST(DEC_L_F1);
    MU_RUN_TEST(DEC_L_00);

    MU_RUN_TEST(DEC_DE_0001);
    MU_RUN_TEST(DEC_DE_0000);
    
    MU_RUN_TEST(DEC_BC_0001);
    MU_RUN_TEST(DEC_BC_0000);

    MU_RUN_TEST(DEC_HL_0001);
    MU_RUN_TEST(DEC_HL_0000);

    MU_RUN_TEST(DEC_SP_0001);
    MU_RUN_TEST(DEC_SP_0000);

    MU_RUN_TEST(DEC_pHL_0001);
    MU_RUN_TEST(DEC_pHL_0000);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}