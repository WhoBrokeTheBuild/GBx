#include "math.h"

#include "register.h"

uint16_t add16(uint16_t a, uint16_t b)
{
    uint32_t c = a + b;
    R.FC = (c & 0xFFFF0000) > 0;
    R.FN = 0;
    R.FH = (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
    R.FZ = (c == 0 ? 1 : 0);
    return c;
}

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
