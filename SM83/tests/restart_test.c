#include "Instructions.h"

#include "stub.inc.h"
#include "unit.h"

void setup()
{
    SM83_Reset(CPU);
    memset(Memory, 0, sizeof(Memory));
    CPU->SP = 0xFFFE;
    CPU->PC = 0x1234;
}

UNIT_TEST(RST_00)
{
    SM83_INST_RST_00(CPU);
    unit_assert_hex_eq(0x0000, CPU->PC);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(RST_08)
{
    SM83_INST_RST_08(CPU);
    unit_assert_hex_eq(0x0008, CPU->PC);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(RST_10)
{
    SM83_INST_RST_10(CPU);
    unit_assert_hex_eq(0x0010, CPU->PC);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(RST_18)
{
    SM83_INST_RST_18(CPU);
    unit_assert_hex_eq(0x0018, CPU->PC);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(RST_20)
{
    SM83_INST_RST_20(CPU);
    unit_assert_hex_eq(0x0020, CPU->PC);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(RST_28)
{
    SM83_INST_RST_28(CPU);
    unit_assert_hex_eq(0x0028, CPU->PC);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(RST_30)
{
    SM83_INST_RST_30(CPU);
    unit_assert_hex_eq(0x0030, CPU->PC);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST(RST_38)
{
    SM83_INST_RST_38(CPU);
    unit_assert_hex_eq(0x0038, CPU->PC);
    unit_assert_hex_eq(0x1234, readWord(CPU->SP));
    unit_assert_int_eq(3, CPU->internal->TotalTicks);
}

UNIT_TEST_SUITE(RST)
{
    UNIT_SUITE_SETUP(&setup);

    UNIT_RUN_TEST(RST_00);
    UNIT_RUN_TEST(RST_08);
    UNIT_RUN_TEST(RST_10);
    UNIT_RUN_TEST(RST_18);
    UNIT_RUN_TEST(RST_20);
    UNIT_RUN_TEST(RST_28);
    UNIT_RUN_TEST(RST_30);
    UNIT_RUN_TEST(RST_38);
}

int main(int argc, char ** argv)
{
    stub_init();

    UNIT_RUN_SUITE(RST);
    UNIT_REPORT();
    return UNIT_EXIT_CODE;
}