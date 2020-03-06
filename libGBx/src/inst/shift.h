#ifndef SHIFT_H
#define SHIFT_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

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
    LogInst("SLA A");
    _SLA(R.A);
}

static void _SLA_B()
{
    LogInst("SLA B");
    _SLA(R.B);
}

static void _SLA_C()
{
    LogInst("SLA C");
    _SLA(R.C);
}

static void _SLA_D()
{
    LogInst("SLA D");
    _SLA(R.D);
}

static void _SLA_E()
{
    LogInst("SLA E");
    _SLA(R.E);
}

static void _SLA_H()
{
    LogInst("SLA H");
    _SLA(R.H);
}

static void _SLA_L()
{
    LogInst("SLA L");
    _SLA(R.L);
}

static void _SLA_pHL()
{
    LogInst("SLA (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SLA(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _SRA_A()
{
    LogInst("SRA A");
    _SRA(R.A);
}

static void _SRA_B()
{
    LogInst("SRA B");
    _SRA(R.B);
}

static void _SRA_C()
{
    LogInst("SRA C");
    _SRA(R.C);
}

static void _SRA_D()
{
    LogInst("SRA D");
    _SRA(R.D);
}

static void _SRA_E()
{
    LogInst("SRA E");
    _SRA(R.E);
}

static void _SRA_H()
{
    LogInst("SRA H");
    _SRA(R.H);
}

static void _SRA_L()
{
    LogInst("SRA L");
    _SRA(R.L);
}

static void _SRA_pHL()
{
    LogInst("_SRA (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SRA(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _SRL_A()
{
    LogInst("SRL A");
    _SRL(R.A);
}

static void _SRL_B()
{
    LogInst("SRL B");
    _SRL(R.B);
}

static void _SRL_C()
{
    LogInst("SRL C");
    _SRL(R.C);
}

static void _SRL_D()
{
    LogInst("SRL D");
    _SRL(R.D);
}

static void _SRL_E()
{
    LogInst("SRL E");
    _SRL(R.E);
}

static void _SRL_H()
{
    LogInst("SRL H");
    _SRL(R.H);
}

static void _SRL_L()
{
    LogInst("SRL L");
    _SRL(R.L);
}

static void _SRL_pHL()
{
    LogInst("_SRL (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _SRL(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _SWAP_A()
{
    LogInst("SWAP A");
    _SWAP(R.A);
    tick(4);
}

static void _SWAP_B()
{
    LogInst("SWAP B");
    _SWAP(R.B);
    tick(4);
}

static void _SWAP_C()
{
    LogInst("SWAP C");
    _SWAP(R.C);
    tick(4);
}

static void _SWAP_D()
{
    LogInst("SWAP D");
    _SWAP(R.D);
    tick(4);
}

static void _SWAP_E()
{
    LogInst("SWAP E");
    _SWAP(R.E);
    tick(4);
}

static void _SWAP_H()
{
    LogInst("SWAP H");
    _SWAP(R.H);
    tick(4);
}

static void _SWAP_L()
{
    LogInst("SWAP L");
    _SWAP(R.L);
    tick(4);
}

static void _SWAP_pHL()
{
    LogInst("SWAP (HL)");
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