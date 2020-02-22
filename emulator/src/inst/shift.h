#ifndef SHIFT_H
#define SHIFT_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _SLA(x)                         \
    R.FC = ((x) & 0x80) == 0x80;        \
    (x) <<= 1;                          \
    R.FZ = ((x) == 0);                  \
    R.FN = false;                       \
    R.FH = false

#define _SRA(x)                         \
    R.FC = ((x) & 0x01) == 0x01;        \
    (x) = ((x) >> 1) | ((x) & 0x80);    \
    R.FZ = ((x) == 0);                  \
    R.FN = false;                       \
    R.FH = false

#define _SRL(x)                         \
    R.FC = ((x) & 0x01) == 0x01;        \
    (x) >>= 1;                          \
    R.FZ = ((x) == 0);                  \
    R.FN = false;                       \
    R.FH = false

#define _SWAP(x)                        \
    (x) = (((x) & 0x0F) << 4)           \
        | (((x) & 0xF0) >> 4);          \
    R.FZ = ((x) == 0);                  \
    R.FN = false;                       \
    R.FH = false;                       \
    R.FC = false;

static void _SLA_A()
{
    LogVerbose(3, "SLA A");
    _SLA(R.A);
}

static void _SLA_B()
{
    LogVerbose(3, "SLA B");
    _SLA(R.B);
}

static void _SLA_C()
{
    LogVerbose(3, "SLA C");
    _SLA(R.C);
}

static void _SLA_D()
{
    LogVerbose(3, "SLA D");
    _SLA(R.D);
}

static void _SLA_E()
{
    LogVerbose(3, "SLA E");
    _SLA(R.E);
}

static void _SLA_H()
{
    LogVerbose(3, "SLA H");
    _SLA(R.H);
}

static void _SLA_L()
{
    LogVerbose(3, "SLA L");
    _SLA(R.L);
}

static void _SLA_pHL()
{
    LogVerbose(3, "SLA (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SLA(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _SRA_A()
{
    LogVerbose(3, "SRA A");
    _SRA(R.A);
}

static void _SRA_B()
{
    LogVerbose(3, "SRA B");
    _SRA(R.B);
}

static void _SRA_C()
{
    LogVerbose(3, "SRA C");
    _SRA(R.C);
}

static void _SRA_D()
{
    LogVerbose(3, "SRA D");
    _SRA(R.D);
}

static void _SRA_E()
{
    LogVerbose(3, "SRA E");
    _SRA(R.E);
}

static void _SRA_H()
{
    LogVerbose(3, "SRA H");
    _SRA(R.H);
}

static void _SRA_L()
{
    LogVerbose(3, "SRA L");
    _SRA(R.L);
}

static void _SRA_pHL()
{
    LogVerbose(3, "_SRA (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SRA(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _SRL_A()
{
    LogVerbose(3, "SRL A");
    _SRL(R.A);
}

static void _SRL_B()
{
    LogVerbose(3, "SRL B");
    _SRL(R.B);
}

static void _SRL_C()
{
    LogVerbose(3, "SRL C");
    _SRL(R.C);
}

static void _SRL_D()
{
    LogVerbose(3, "SRL D");
    _SRL(R.D);
}

static void _SRL_E()
{
    LogVerbose(3, "SRL E");
    _SRL(R.E);
}

static void _SRL_H()
{
    LogVerbose(3, "SRL H");
    _SRL(R.H);
}

static void _SRL_L()
{
    LogVerbose(3, "SRL L");
    _SRL(R.L);
}

static void _SRL_pHL()
{
    LogVerbose(3, "_SRL (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SRL(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _SWAP_A()
{
    LogVerbose(3, "SWAP A");
    _SWAP(R.A);
    tick(4);
}

static void _SWAP_B()
{
    LogVerbose(3, "SWAP B");
    _SWAP(R.B);
    tick(4);
}

static void _SWAP_C()
{
    LogVerbose(3, "SWAP C");
    _SWAP(R.C);
    tick(4);
}

static void _SWAP_D()
{
    LogVerbose(3, "SWAP D");
    _SWAP(R.D);
    tick(4);
}

static void _SWAP_E()
{
    LogVerbose(3, "SWAP E");
    _SWAP(R.E);
    tick(4);
}

static void _SWAP_H()
{
    LogVerbose(3, "SWAP H");
    _SWAP(R.H);
    tick(4);
}

static void _SWAP_L()
{
    LogVerbose(3, "SWAP L");
    _SWAP(R.L);
    tick(4);
}

static void _SWAP_pHL()
{
    LogVerbose(3, "SWAP (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SWAP(n);
    tick(4);

    writeByte(R.HL, n);
    tick(4);
}

#undef _SWAP
#undef _SRL
#undef _SRA
#undef _SLA

#endif // SHIFT_H