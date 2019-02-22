#include "alu.h"
#include "register.h"

uint8_t DIV;

int8_t add8(int8_t a, int8_t b)
{
    int16_t c = a + b;
    R.FC = (c & 0xFF00) > 0;
    R.FN = 0;
    R.FH = (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
    R.FZ = (c == 0 ? 1 : 0);
    return c;
}

int8_t sub8(int8_t a, int8_t b)
{
    int16_t c = a - b;
    R.FC = (c & 0xFF00) > 0;
    R.FN = 1;
    R.FH = 0; //(((a & 0xF) - (b & 0xF)) & 0x10) == 0x10;
    R.FZ = (c == 0 ? 1 : 0);
    return c;
}

uint16_t add16(uint16_t a, uint16_t b)
{
    uint32_t c = a + b;
    R.FC = (c & 0xFFFF0000) > 0;
    R.FN = 0;
    R.FH = (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
    R.FZ = (c == 0 ? 1 : 0);
    return c;
}

uint16_t sub16(uint16_t a, uint16_t b)
{
    uint32_t c = a - b;
    R.FC = (c & 0xFFFF0000) > 0;
    R.FN = 1;
    R.FH = 0; //(((a & 0xF) - (b & 0xF)) & 0x10) == 0x10;
    R.FZ = (c == 0 ? 1 : 0);
    return c;
}

uint8_t and8(uint8_t a, uint8_t b)
{
    a &= b;
    R.FZ = (a == 0);
    R.FN = 0;
    R.FH = 1;
    R.FC = 0;
    return a;
}

uint8_t or8(uint8_t a, uint8_t b)
{
    a |= b;
    R.FZ = (a == 0);
    R.FN = 0;
    R.FH = 0;
    R.FC = 0;
    return a;
}

uint8_t xor8(uint8_t a, uint8_t b)
{
    a ^= b;
    R.FZ = (a == 0);
    R.FN = 0;
    R.FH = 0;
    R.FC = 0;
    return a;
}

uint8_t inc8(uint8_t a)
{
    R.FZ = (a + 1 == 0);
    R.FN = 0;
    R.FH = (((a & 0xF) + 1) & 0x10) == 0x10;
    return ++a;
}

uint8_t dec8(uint8_t a)
{
    R.FZ = (a - 1 == 0);
    R.FN = 1;
    R.FH = ((a & 0xF) - 1) == 0x00;
    return --a;
}