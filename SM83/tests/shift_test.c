#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup()
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->HL = 0x1234;
}

#define MAKE_SLA_TEST(REG)                          \
    UNIT_TEST(SLA_##REG##_00)                       \
    {                                               \
        CPU->REG = 0x00;                               \
        SM83_INST_SLA_##REG(CPU);                               \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SLA_##REG##_01)                       \
    {                                               \
        CPU->REG = 0x01;                               \
        SM83_INST_SLA_##REG(CPU);                               \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_hex_eq(0x02, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SLA_##REG##_80)                       \
    {                                               \
        CPU->REG = 0x80;                               \
        SM83_INST_SLA_##REG(CPU);                               \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_true(CPU->FC);                     \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SLA_##REG##_FF)                       \
    {                                               \
        CPU->REG = 0xFF;                               \
        SM83_INST_SLA_##REG(CPU);                               \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_true(CPU->FC);                     \
        unit_assert_hex_eq(0xFE, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
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
    writeByte(CPU->HL, 0x00);
    SM83_INST_SLA_pHL(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(SLA_pHL_01)
{
    writeByte(CPU->HL, 0x01);
    SM83_INST_SLA_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_hex_eq(0x02, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(SLA_pHL_80)
{
    writeByte(CPU->HL, 0x80);
    SM83_INST_SLA_pHL(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(SLA_pHL_FF)
{
    writeByte(CPU->HL, 0xFF);
    SM83_INST_SLA_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_hex_eq(0xFE, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
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
        CPU->REG = 0x00;                               \
        SM83_INST_SRA_##REG(CPU);                               \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SRA_##REG##_01)                       \
    {                                               \
        CPU->REG = 0x01;                               \
        SM83_INST_SRA_##REG(CPU);                               \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_true(CPU->FC);                     \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SRA_##REG##_FF)                       \
    {                                               \
        CPU->REG = 0xFF;                               \
        SM83_INST_SRA_##REG(CPU);                               \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_true(CPU->FC);                     \
        unit_assert_hex_eq(0xFF, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
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
    writeByte(CPU->HL, 0x00);
    SM83_INST_SRA_pHL(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(SRA_pHL_01)
{
    writeByte(CPU->HL, 0x01);
    SM83_INST_SRA_pHL(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(SRA_pHL_FF)
{
    writeByte(CPU->HL, 0xFF);
    SM83_INST_SRA_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_hex_eq(0xFF, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
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
        CPU->REG = 0x00;                               \
        SM83_INST_SRL_##REG(CPU);                               \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SRL_##REG##_01)                       \
    {                                               \
        CPU->REG = 0x01;                               \
        SM83_INST_SRL_##REG(CPU);                               \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_true(CPU->FC);                     \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SRL_##REG##_AA)                       \
    {                                               \
        CPU->REG = 0xAA;                               \
        SM83_INST_SRL_##REG(CPU);                               \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                     \
        unit_assert_hex_eq(0x55, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SRL_##REG##_FF)                       \
    {                                               \
        CPU->REG = 0xFF;                               \
        SM83_INST_SRL_##REG(CPU);                               \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_true(CPU->FC);                     \
        unit_assert_hex_eq(0x7F, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
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
    writeByte(CPU->HL, 0x00);
    SM83_INST_SRL_pHL(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(SRL_pHL_01)
{
    writeByte(CPU->HL, 0x01);
    SM83_INST_SRL_pHL(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(SRL_pHL_AA)
{
    writeByte(CPU->HL, 0xAA);
    SM83_INST_SRL_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_hex_eq(0x55, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}

UNIT_TEST(SRL_pHL_FF)
{
    writeByte(CPU->HL, 0xFF);
    SM83_INST_SRL_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_true(CPU->FC);
    unit_assert_hex_eq(0x7F, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
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

int main(int argc, char ** argv)
{
    stub_init();

    UNIT_RUN_SUITE(SLA);
    UNIT_RUN_SUITE(SRA);
    UNIT_RUN_SUITE(SRL);
    UNIT_REPORT();
    return UNIT_EXIT_CODE;
}