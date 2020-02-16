#include "math.h"

#include "register.h"

uint16_t add16(uint16_t a, uint16_t b)
{
    uint32_t c = a + b;
    R.FN = false;
    R.FH = (((a & 0x7FF) + (b & 0x7FF)) & 0x800) == 0x800;
    R.FC = (c & 0xFFFF0000) > 0;
    return c;
}

uint16_t add16s(uint16_t a, int16_t b)
{
    uint32_t c = (uint32_t)(a + b);
    R.FN = false;
    R.FH = (((a & 0x7FF) + (b & 0x7FF)) & 0x800) == 0x800;
    R.FC = (c & 0xFFFF0000) > 0;
    return c;
}

uint8_t add8(uint8_t a, uint8_t b)
{
    uint16_t c = a + b;
    R.FZ = (c == 0 ? 1 : 0);
    R.FN = false;
    R.FH = (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
    R.FC = (c & 0xFF00) > 0;
    return c;
}

uint8_t add8s(uint8_t a, int8_t b)
{
    uint16_t c = (uint16_t)(a + b);
    R.FZ = (c == 0 ? 1 : 0);
    R.FN = false;
    R.FH = (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
    R.FC = (c & 0xFF00) > 0;
    return c;
}

uint8_t sub8(uint8_t a, uint8_t b)
{
    uint16_t c = a - b;
    R.FZ = (c == 0 ? 1 : 0);
    R.FN = true;
    R.FH = ((b & 0xF) > (a & 0xF));
    R.FC = (c & 0xFF00) > 0;
    return c;
}

uint8_t sub8s(uint8_t a, int8_t b)
{
    uint16_t c = (uint16_t)(a - b);
    R.FZ = (c == 0 ? 1 : 0);
    R.FN = true;
    R.FH = ((b & 0xF) > (a & 0xF));
    R.FC = (c & 0xFF00) > 0;
    return c;
}

uint8_t and8(uint8_t a, uint8_t b)
{
    a &= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = true;
    R.FC = false;
    return a;
}

uint8_t or8(uint8_t a, uint8_t b)
{
    a |= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    R.FC = false;
    return a;
}

uint8_t xor8(uint8_t a, uint8_t b)
{
    a ^= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    R.FC = false;
    return a;
}