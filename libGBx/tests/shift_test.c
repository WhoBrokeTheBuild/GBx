#include "inst/shift.h"
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

MAKE_SLA_TEST(A);
MAKE_SLA_TEST(B);
MAKE_SLA_TEST(C);
MAKE_SLA_TEST(D);
MAKE_SLA_TEST(E);
MAKE_SLA_TEST(H);
MAKE_SLA_TEST(L);

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

#define MAKE_SRA_TEST(REG)                          \
    UNIT_TEST(SRA_##REG##_00)                       \
    {                                               \
        R.REG = 0x00;                               \
        _SRA_##REG();                               \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
        unit_assert_hex_eq(0x00, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SRA_##REG##_01)                       \
    {                                               \
        R.REG = 0x01;                               \
        _SRA_##REG();                               \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_true(R.FC);                     \
        unit_assert_hex_eq(0x00, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SRA_##REG##_FF)                       \
    {                                               \
        R.REG = 0xFF;                               \
        _SRA_##REG();                               \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_true(R.FC);                     \
        unit_assert_hex_eq(0xFF, R.REG);            \
    }

MAKE_SRA_TEST(A);
MAKE_SRA_TEST(B);
MAKE_SRA_TEST(C);
MAKE_SRA_TEST(D);
MAKE_SRA_TEST(E);
MAKE_SRA_TEST(H);
MAKE_SRA_TEST(L);

UNIT_TEST(SRA_pHL_00)
{
    writeByte(R.HL, 0x00);
    _SRA_pHL();
    unit_assert_true(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
    unit_assert_hex_eq(0x00, readByte(R.HL));
}

UNIT_TEST(SRA_pHL_01)
{
    writeByte(R.HL, 0x01);
    _SRA_pHL();
    unit_assert_true(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_true(R.FC);
    unit_assert_hex_eq(0x00, readByte(R.HL));
}

UNIT_TEST(SRA_pHL_FF)
{
    writeByte(R.HL, 0xFF);
    _SRA_pHL();
    unit_assert_false(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_true(R.FC);
    unit_assert_hex_eq(0xFF, readByte(R.HL));
}

UNIT_TEST_SUITE(SRA)
{
    UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(SRA_A_00);
    UNIT_RUN_TEST(SRA_A_01);
    UNIT_RUN_TEST(SRA_A_FF);

    UNIT_RUN_TEST(SRA_B_00);
    UNIT_RUN_TEST(SRA_B_01);
    UNIT_RUN_TEST(SRA_B_FF);

    UNIT_RUN_TEST(SRA_C_00);
    UNIT_RUN_TEST(SRA_C_01);
    UNIT_RUN_TEST(SRA_C_FF);

    UNIT_RUN_TEST(SRA_D_00);
    UNIT_RUN_TEST(SRA_D_01);
    UNIT_RUN_TEST(SRA_D_FF);

    UNIT_RUN_TEST(SRA_E_00);
    UNIT_RUN_TEST(SRA_E_01);
    UNIT_RUN_TEST(SRA_E_FF);

    UNIT_RUN_TEST(SRA_H_00);
    UNIT_RUN_TEST(SRA_H_01);
    UNIT_RUN_TEST(SRA_H_FF);

    UNIT_RUN_TEST(SRA_L_00);
    UNIT_RUN_TEST(SRA_L_01);
    UNIT_RUN_TEST(SRA_L_FF);

    UNIT_RUN_TEST(SRA_pHL_00);
    UNIT_RUN_TEST(SRA_pHL_01);
    UNIT_RUN_TEST(SRA_pHL_FF);
}

#define MAKE_SRL_TEST(REG)                          \
    UNIT_TEST(SRL_##REG##_00)                       \
    {                                               \
        R.REG = 0x00;                               \
        _SRL_##REG();                               \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
        unit_assert_hex_eq(0x00, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SRL_##REG##_01)                       \
    {                                               \
        R.REG = 0x01;                               \
        _SRL_##REG();                               \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_true(R.FC);                     \
        unit_assert_hex_eq(0x00, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SRL_##REG##_AA)                       \
    {                                               \
        R.REG = 0xAA;                               \
        _SRL_##REG();                               \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                     \
        unit_assert_hex_eq(0x55, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SRL_##REG##_FF)                       \
    {                                               \
        R.REG = 0xFF;                               \
        _SRL_##REG();                               \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_true(R.FC);                     \
        unit_assert_hex_eq(0x7F, R.REG);            \
    }

MAKE_SRL_TEST(A);
MAKE_SRL_TEST(B);
MAKE_SRL_TEST(C);
MAKE_SRL_TEST(D);
MAKE_SRL_TEST(E);
MAKE_SRL_TEST(H);
MAKE_SRL_TEST(L);

UNIT_TEST(SRL_pHL_00)
{
    writeByte(R.HL, 0x00);
    _SRL_pHL();
    unit_assert_true(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
    unit_assert_hex_eq(0x00, readByte(R.HL));
}

UNIT_TEST(SRL_pHL_01)
{
    writeByte(R.HL, 0x01);
    _SRL_pHL();
    unit_assert_true(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_true(R.FC);
    unit_assert_hex_eq(0x00, readByte(R.HL));
}

UNIT_TEST(SRL_pHL_AA)
{
    writeByte(R.HL, 0xAA);
    _SRL_pHL();
    unit_assert_false(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
    unit_assert_hex_eq(0x55, readByte(R.HL));
}

UNIT_TEST(SRL_pHL_FF)
{
    writeByte(R.HL, 0xFF);
    _SRL_pHL();
    unit_assert_false(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_true(R.FC);
    unit_assert_hex_eq(0x7F, readByte(R.HL));
}

UNIT_TEST_SUITE(SRL)
{
    UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(SRL_A_00);
    UNIT_RUN_TEST(SRL_A_01);
    UNIT_RUN_TEST(SRL_A_AA);
    UNIT_RUN_TEST(SRL_A_FF);

    UNIT_RUN_TEST(SRL_B_00);
    UNIT_RUN_TEST(SRL_B_01);
    UNIT_RUN_TEST(SRL_B_AA);
    UNIT_RUN_TEST(SRL_B_FF);

    UNIT_RUN_TEST(SRL_C_00);
    UNIT_RUN_TEST(SRL_C_01);
    UNIT_RUN_TEST(SRL_C_AA);
    UNIT_RUN_TEST(SRL_C_FF);

    UNIT_RUN_TEST(SRL_D_00);
    UNIT_RUN_TEST(SRL_D_01);
    UNIT_RUN_TEST(SRL_D_AA);
    UNIT_RUN_TEST(SRL_D_FF);

    UNIT_RUN_TEST(SRL_E_00);
    UNIT_RUN_TEST(SRL_E_01);
    UNIT_RUN_TEST(SRL_E_AA);
    UNIT_RUN_TEST(SRL_E_FF);

    UNIT_RUN_TEST(SRL_H_00);
    UNIT_RUN_TEST(SRL_H_01);
    UNIT_RUN_TEST(SRL_H_AA);
    UNIT_RUN_TEST(SRL_H_FF);

    UNIT_RUN_TEST(SRL_L_00);
    UNIT_RUN_TEST(SRL_L_01);
    UNIT_RUN_TEST(SRL_L_AA);
    UNIT_RUN_TEST(SRL_L_FF);

    UNIT_RUN_TEST(SRL_pHL_00);
    UNIT_RUN_TEST(SRL_pHL_01);
    UNIT_RUN_TEST(SRL_pHL_AA);
    UNIT_RUN_TEST(SRL_pHL_FF);

}

#define MAKE_SWAP_TEST(REG)                         \
    UNIT_TEST(SWAP_##REG##_00)                      \
    {                                               \
        R.REG = 0x00;                               \
        _SWAP_##REG();                              \
        unit_assert_true(R.FZ);                     \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
        unit_assert_hex_eq(0x00, R.REG);            \
    }                                               \
                                                    \
    UNIT_TEST(SWAP_##REG##_0A)                      \
    {                                               \
        R.REG = 0x0A;                               \
        _SWAP_##REG();                              \
        unit_assert_false(R.FZ);                    \
        unit_assert_false(R.FN);                    \
        unit_assert_false(R.FH);                    \
        unit_assert_false(R.FC);                    \
        unit_assert_hex_eq(0xA0, R.REG);            \
    }

MAKE_SWAP_TEST(A);
MAKE_SWAP_TEST(B);
MAKE_SWAP_TEST(C);
MAKE_SWAP_TEST(D);
MAKE_SWAP_TEST(E);
MAKE_SWAP_TEST(H);
MAKE_SWAP_TEST(L);

UNIT_TEST(SWAP_pHL_00)
{
    writeByte(R.HL, 0x00);
    _SWAP_pHL();
    unit_assert_true(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
    unit_assert_hex_eq(0x00, readByte(R.HL));
}
                                                
UNIT_TEST(SWAP_pHL_0A)
{
    writeByte(R.HL, 0x0A);
    _SWAP_pHL();
    unit_assert_false(R.FZ);
    unit_assert_false(R.FN);
    unit_assert_false(R.FH);
    unit_assert_false(R.FC);
    unit_assert_hex_eq(0xA0, readByte(R.HL));
}

UNIT_TEST_SUITE(SWAP)
{
    UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(SWAP_A_00);
    UNIT_RUN_TEST(SWAP_A_0A);

    UNIT_RUN_TEST(SWAP_B_00);
    UNIT_RUN_TEST(SWAP_B_0A);

    UNIT_RUN_TEST(SWAP_C_00);
    UNIT_RUN_TEST(SWAP_C_0A);

    UNIT_RUN_TEST(SWAP_D_00);
    UNIT_RUN_TEST(SWAP_D_0A);

    UNIT_RUN_TEST(SWAP_E_00);
    UNIT_RUN_TEST(SWAP_E_0A);

    UNIT_RUN_TEST(SWAP_H_00);
    UNIT_RUN_TEST(SWAP_H_0A);

    UNIT_RUN_TEST(SWAP_L_00);
    UNIT_RUN_TEST(SWAP_L_0A);

    UNIT_RUN_TEST(SWAP_pHL_00);
    UNIT_RUN_TEST(SWAP_pHL_0A);
}

int main(int argc, char ** argv)
{
    VerboseLevel = 4;
    UNIT_RUN_SUITE(SLA);
    UNIT_RUN_SUITE(SRA);
    UNIT_RUN_SUITE(SRL);
    UNIT_RUN_SUITE(SWAP);
    UNIT_REPORT();
    return UNIT_EXIT_CODE;
}