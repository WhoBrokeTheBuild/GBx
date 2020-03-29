#ifndef GBX_ALU_H
#define GBX_ALU_H

#include <GBx/CPU.h>

static inline word AddWords(word a, word b)
{
    dword c = a + b;
    R.FN = false;
    R.FH = (((a & 0x7FF) + (b & 0x7FF)) & 0x800) == 0x800;
    R.FC = (c & 0xFFFF0000) > 0;
    return c;
}

static inline word AddWordSignedByte(word a, sbyte b)
{
    dword c = (uint32_t)(a + b);
    R.FN = false;
    R.FH = (((a & 0x7FF) + b) & 0x800) == 0x800;
    R.FC = (c & 0xFFFF0000) > 0;
    return c;
}

static inline byte AddBytes(byte a, byte b)
{
    word c = a + b;
    R.FZ = ((c & 0xFF) == 0);
    R.FN = false;
    R.FH = (((a & 0xF) + (b & 0xF)) & 0x10) == 0x10;
    R.FC = (c & 0xFF00) > 0;
    return c;
}

static inline byte AndBytes(byte a, byte b)
{
    a &= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = true;
    R.FC = false;
    return a;
}

static inline byte OrBytes(byte a, byte b)
{
    a |= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    R.FC = false;
    return a;
}

static inline byte XorBytes(byte a, byte b)
{
    a ^= b;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    R.FC = false;
    return a;
}

static inline byte SubtractBytes(byte a, byte b)
{
    word c = a - b;
    R.FZ = ((c & 0xFF) == 0);
    R.FN = true;
    R.FH = ((b & 0xF) > (a & 0xF));
    R.FC = (c & 0xFF00) > 0;
    return c;
}

static inline byte SetBit(byte a, int bit)
{
    return a | (1 << bit);
}

static inline byte ResetBit(byte a, int bit)
{
    return a & ~(1 << bit);
}

static inline void TestBit(byte a, int bit)
{
    R.FZ = ((a & (1 << bit)) == 0);
    R.FN = false;
    R.FH = true;
}

static inline byte SwapNibbles(byte a) 
{
    a = ((a & 0x0F) << 4)
        | ((a & 0xF0) >> 4);
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    R.FC = false;
    return a;
}

static inline byte RotateLeft(byte a)
{
    bool fc = ((a & 0x80) == 0x80);
    a = (a << 1) | R.FC;
    R.FC = fc;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    return a;
}

static inline byte RotateLeftWithCarry(byte a)
{
    R.FC = ((a & 0x80) == 0x80);
    a = (a << 1) | R.FC;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    return a;
}

static inline byte RotateRight(byte a)
{
    bool fc = ((a & 0x01) == 0x01);
    a = (a >> 1) | (R.FC << 7);
    R.FC = fc;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    return a;
}

static inline byte RotateRightWithCarry(byte a)
{
    R.FC = ((a & 0x01) == 0x01);
    a = (a >> 1) | (R.FC << 7);
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    return a;
}

static inline byte ShiftRight(byte a)
{
    R.FC = (a & 0x01) == 0x01;
    a >>= 1;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    return a;
}

static inline byte ShiftRightWithCarry(byte a)
{
    R.FC = (a & 0x01) == 0x01;
    a = (a >> 1) | (a & 0x80);
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    return a;
}

static inline byte ShiftLeftWithCarry(byte a)
{
    R.FC = (a & 0x80) == 0x80;
    a <<= 1;
    R.FZ = (a == 0);
    R.FN = false;
    R.FH = false;
    return a;
}

#endif // GBX_ALU_H