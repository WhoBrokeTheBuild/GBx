#include "inst/dec.h"
#include "bootstrap.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xCFFF;

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
}

#define MAKE_DEC_TEST8(_REG_)                       \
    UNIT_TEST(DEC_##_REG_##_01)                     \
    {                                               \
        R._REG_ = 0x01;                             \
        _DEC_##_REG_();                             \
        unit_assert_hex_eq(0x00, R._REG_);          \
        unit_assert_true(R.FZ);                     \
        unit_assert_true(R.FN);                     \
        unit_assert_false(R.FH);                    \
        unit_assert_int_eq(4, CPUTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(DEC_##_REG_##_F0)                     \
    {                                               \
        R._REG_ = 0xF0;                             \
        _DEC_##_REG_();                             \
        unit_assert_hex_eq(0xEF, R._REG_);          \
        unit_assert_false(R.FZ);                    \
        unit_assert_true(R.FN);                     \
        unit_assert_true(R.FH);                     \
        unit_assert_int_eq(4, CPUTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(DEC_##_REG_##_00)                     \
    {                                               \
        R._REG_ = 0x00;                             \
        _DEC_##_REG_();                             \
        unit_assert_hex_eq(0xFF, R._REG_);          \
        unit_assert_false(R.FZ);                    \
        unit_assert_true(R.FN);                     \
        unit_assert_true(R.FH);                     \
        unit_assert_int_eq(4, CPUTicks);            \
    }

#define MAKE_DEC_TEST16(_REG_)                      \
    UNIT_TEST(DEC_##_REG_##_0001)                   \
    {                                               \
        R._REG_ = 0x0001;                           \
        _DEC_##_REG_();                             \
        unit_assert_hex_eq(0x0000, R._REG_);        \
        unit_assert_int_eq(8, CPUTicks);            \
    }                                               \
                                                    \
    UNIT_TEST(DEC_##_REG_##_0000)                   \
    {                                               \
        R._REG_ = 0x0000;                           \
        _DEC_##_REG_();                             \
        unit_assert_hex_eq(0xFFFF, R._REG_);        \
        unit_assert_int_eq(8, CPUTicks);            \
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

UNIT_TEST(DEC_pHL_0001)
{
    R.HL = RAM_OFFSET;
    writeByte(RAM_OFFSET, 0x01);
    _DEC_pHL();
    unit_assert_hex_eq(0x00, readByte(RAM_OFFSET));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST(DEC_pHL_0000)
{
    R.HL = RAM_OFFSET;
    writeByte(RAM_OFFSET, 0x00);
    _DEC_pHL();
    unit_assert_hex_eq(0xFF, readByte(RAM_OFFSET));
    unit_assert_int_eq(12, CPUTicks);
}

UNIT_TEST_SUITE(dec_suite)
{
    UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(DEC_A_01);
    UNIT_RUN_TEST(DEC_A_F0);
    UNIT_RUN_TEST(DEC_A_00);
    
    UNIT_RUN_TEST(DEC_B_01);
    UNIT_RUN_TEST(DEC_B_F0);
    UNIT_RUN_TEST(DEC_B_00);
    
    UNIT_RUN_TEST(DEC_C_01);
    UNIT_RUN_TEST(DEC_C_F0);
    UNIT_RUN_TEST(DEC_C_00);
    
    UNIT_RUN_TEST(DEC_D_01);
    UNIT_RUN_TEST(DEC_D_F0);
    UNIT_RUN_TEST(DEC_D_00);
    
    UNIT_RUN_TEST(DEC_E_01);
    UNIT_RUN_TEST(DEC_E_F0);
    UNIT_RUN_TEST(DEC_E_00);
    
    UNIT_RUN_TEST(DEC_H_01);
    UNIT_RUN_TEST(DEC_H_F0);
    UNIT_RUN_TEST(DEC_H_00);

    UNIT_RUN_TEST(DEC_L_01);
    UNIT_RUN_TEST(DEC_L_F0);
    UNIT_RUN_TEST(DEC_L_00);

    UNIT_RUN_TEST(DEC_DE_0001);
    UNIT_RUN_TEST(DEC_DE_0000);
    
    UNIT_RUN_TEST(DEC_BC_0001);
    UNIT_RUN_TEST(DEC_BC_0000);

    UNIT_RUN_TEST(DEC_HL_0001);
    UNIT_RUN_TEST(DEC_HL_0000);

    UNIT_RUN_TEST(DEC_SP_0001);
    UNIT_RUN_TEST(DEC_SP_0000);

    UNIT_RUN_TEST(DEC_pHL_0001);
    UNIT_RUN_TEST(DEC_pHL_0000);
}

int main(int argc, char *argv[])
{
    DebugMode = true;
    UNIT_RUN_SUITE(dec_suite);
    UNIT_REPORT();
    return UNIT_EXIT_CODE;
}