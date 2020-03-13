#include "alu.h"

#include "cpu.h"

word add16(word a, word b)
{
    uint32_t c = a + b;
    R.FN = false;
    R.FH = (((a & 0x7FF) + (b & 0x7FF)) & 0x800) == 0x800;
    R.FC = (c & 0xFFFF0000) > 0;
    return (word)c;
}

word add16s(word a, sbyte b)
{
    uint32_t c = (uint32_t)(a + b);
    R.FN = false;
    R.FH = (((a & 0x7FF) + b) & 0x800) == 0x800;
    R.FC = (c & 0xFFFF0000) > 0;
    return (word)c;
}

byte add8(byte a, byte b)
{
    word c = a + b;
    R.FZ = ((c & 0xFF) == 0);
    R.FN = false;
    R.FH = (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
    R.FC = (c & 0xFF00) > 0;
    return (byte)c;
}

byte sub8(byte a, byte b)
{
    word c = a - b;
    R.FZ = ((c & 0xFF) == 0);
    R.FN = true;
    R.FH = ((b & 0xF) > (a & 0xF));
    R.FC = (c & 0xFF00) > 0;
    return (byte)c;
}

byte and8(byte a, byte b)
{
    a &= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = true;
    R.FC = false;
    return a;
}

byte or8(byte a, byte b)
{
    a |= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    R.FC = false;
    return a;
}

byte xor8(byte a, byte b)
{
    a ^= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    R.FC = false;
    return a;
}