#include "inst/shift.h"
#include "memory.h"
#include "unit.h"

const uint16_t RAM_OFFSET = 0xCFFF;

void setup() 
{
    CPUTicks = 0;
    memset(&R, sizeof(R), 0);
    R.HL = RAM_OFFSET;
}

#define MAKE_SLA_TEST(REG)                          \
    UNIT_TEST(SLA_##REG##_00)                       \
    {                                               \
        R.REG = 0x00;                               \
        _SLA_##REG();                               \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
        unit_assert_hex_eq(0x00, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SLA_##REG##_01)                       \
    {                                               \
        R.REG = 0x01;                               \
        _SLA_##REG();                               \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
        unit_assert_hex_eq(0x02, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SLA_##REG##_80)                       \
    {                                               \
        R.REG = 0x80;                               \
        _SLA_##REG();                               \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_true(R.FC);                     \
        unit_assert_hex_eq(0x00, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SLA_##REG##_FF)                       \
    {                                               \
        R.REG = 0xFF;                               \
        _SLA_##REG();                               \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_true(R.FC);                     \
        unit_assert_hex_eq(0xFE, R.REG);            \
    }

UNIT_TEST(SLA_pHL_00)
{
    writeByte(R.HL, 0x00);
    _SLA_pHL();
    unit_assert_true(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
    unit_assert_hex_eq(0x00, readByte(R.HL));
}

UNIT_TEST(SLA_pHL_01)
{
    writeByte(R.HL, 0x01);
    _SLA_pHL();
    unit_assert_false(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
    unit_assert_hex_eq(0x02, readByte(R.HL));
}

UNIT_TEST(SLA_pHL_80)
{
    writeByte(R.HL, 0x80);
    _SLA_pHL();
    unit_assert_true(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_true(R.FC);
    unit_assert_hex_eq(0x00, readByte(R.HL));
}

UNIT_TEST(SLA_pHL_FF)
{
    writeByte(R.HL, 0xFF);
    _SLA_pHL();
    unit_assert_false(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_true(R.FC);
    unit_assert_hex_eq(0xFE, readByte(R.HL));
}

MAKE_SLA_TEST(A);
MAKE_SLA_TEST(B);
MAKE_SLA_TEST(C);
MAKE_SLA_TEST(D);
MAKE_SLA_TEST(E);
MAKE_SLA_TEST(H);
MAKE_SLA_TEST(L);

UNIT_TEST_SUITE(SLA)
{
    UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(SLA_A_00);
    UNIT_RUN_TEST(SLA_A_01);
    UNIT_RUN_TEST(SLA_A_80);
    UNIT_RUN_TEST(SLA_A_FF);

    UNIT_RUN_TEST(SLA_B_00);
    UNIT_RUN_TEST(SLA_B_01);
    UNIT_RUN_TEST(SLA_B_80);
    UNIT_RUN_TEST(SLA_B_FF);

    UNIT_RUN_TEST(SLA_C_00);
    UNIT_RUN_TEST(SLA_C_01);
    UNIT_RUN_TEST(SLA_C_80);
    UNIT_RUN_TEST(SLA_C_FF);

    UNIT_RUN_TEST(SLA_D_00);
    UNIT_RUN_TEST(SLA_D_01);
    UNIT_RUN_TEST(SLA_D_80);
    UNIT_RUN_TEST(SLA_D_FF);

    UNIT_RUN_TEST(SLA_E_00);
    UNIT_RUN_TEST(SLA_E_01);
    UNIT_RUN_TEST(SLA_E_80);
    UNIT_RUN_TEST(SLA_E_FF);

    UNIT_RUN_TEST(SLA_H_00);
    UNIT_RUN_TEST(SLA_H_01);
    UNIT_RUN_TEST(SLA_H_80);
    UNIT_RUN_TEST(SLA_H_FF);

    UNIT_RUN_TEST(SLA_L_00);
    UNIT_RUN_TEST(SLA_L_01);
    UNIT_RUN_TEST(SLA_L_80);
    UNIT_RUN_TEST(SLA_L_FF);

    UNIT_RUN_TEST(SLA_pHL_00);
    UNIT_RUN_TEST(SLA_pHL_01);
    UNIT_RUN_TEST(SLA_pHL_80);
    UNIT_RUN_TEST(SLA_pHL_FF);
}

UNIT_TEST_SUITE(SRA)
{
    UNIT_SUITE_SETUP(&setup);

}

UNIT_TEST_SUITE(SRL)
{
    UNIT_SUITE_SETUP(&setup);

}

UNIT_TEST_SUITE(SWAP)
{
    UNIT_SUITE_SETUP(&setup);

}

int main(int argc, char *argv[])
{
    DebugMode = true;
    UNIT_RUN_SUITE(SLA);
    UNIT_RUN_SUITE(SRA);
    UNIT_RUN_SUITE(SRL);
    UNIT_RUN_SUITE(SWAP);
    UNIT_REPORT();
    return UNIT_EXIT_CODE;
}