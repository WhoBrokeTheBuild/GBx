#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup()
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->HL = 0x1234;
}

#define MAKE_SWAP_TEST(REG)                         \
    UNIT_TEST(SWAP_##REG##_00)                      \
    {                                               \
        CPU->REG = 0x00;                               \
        SM83_INST_SWAP_##REG(CPU);                              \
        unit_assert_true(CPU->FZ);                     \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_hex_eq(0x00, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
    }                                               \
                                                    \
    UNIT_TEST(SWAP_##REG##_0A)                      \
    {                                               \
        CPU->REG = 0x0A;                               \
        SM83_INST_SWAP_##REG(CPU);                              \
        unit_assert_false(CPU->FZ);                    \
        unit_assert_false(CPU->FN);                    \
        unit_assert_false(CPU->FH);                    \
        unit_assert_false(CPU->FC);                    \
        unit_assert_hex_eq(0xA0, CPU->REG);            \
        unit_assert_int_eq(0, CPU->internal->TotalTicks);   \
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
    writeByte(CPU->HL, 0x00);
    SM83_INST_SWAP_pHL(CPU);
    unit_assert_true(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_hex_eq(0x00, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
}
                                                
UNIT_TEST(SWAP_pHL_0A)
{
    writeByte(CPU->HL, 0x0A);
    SM83_INST_SWAP_pHL(CPU);
    unit_assert_false(CPU->FZ);
    unit_assert_false(CPU->FN);
    unit_assert_false(CPU->FH);
    unit_assert_false(CPU->FC);
    unit_assert_hex_eq(0xA0, readByte(CPU->HL));
    unit_assert_int_eq(2, CPU->internal->TotalTicks);
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
    stub_init();

    UNIT_RUN_SUITE(SWAP);
    UNIT_REPORT();
    return UNIT_EXIT_CODE;
}