#ifndef SHIFT_H
#define SHIFT_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _SLA(x) \
    R.FC = ((x) & 0x80) == 0x80; \
    (x) <<= 1; \
    R.FZ = ((x) == 0); \
    R.FN = false; \
    R.FH = false

#define _SRA(x) \
    R.FC = ((x) & 0x01) == 0x01; \
    (x) >>= 1; \
    R.FZ = ((x) == 0); \
    R.FN = false; \
    R.FH = false

#define _SRL(x) \
    R.FC = ((x) & 0x01) == 0x01; \
    (x) = ((x) >> 1) | ((x) & 0x80); \
    R.FZ = ((x) == 0); \
    R.FN = false; \
    R.FH = false

#define _SWAP(x) \
    (x) = (((x) & 0x0F) << 4) | (((x) & 0xF0) >> 4)

static void _SLA_B()
{
    LogDebug("SLA B");
    _SLA(R.B);
}

static void _SLA_C()
{
    LogDebug("SLA C");
    _SLA(R.C);
}

static void _SLA_D()
{
    LogDebug("SLA D");
    _SLA(R.D);
}

static void _SLA_E()
{
    LogDebug("SLA E");
    _SLA(R.E);
}

static void _SLA_H()
{
    LogDebug("SLA H");
    _SLA(R.H);
}

static void _SLA_L()
{
    LogDebug("SLA L");
    _SLA(R.L);
}

static void _SLA_pHL()
{
    LogDebug("SLA (HL)");
    uint8_t n = readByte(R.HL);
    TickCounter += 4;

    _SLA(n);

    writeByte(R.HL, n);
    TickCounter += 4;
}

static void _SLA_A()
{
    LogDebug("SLA A");
    _SLA(R.A);
}

static void _SRA_B()
{
    LogDebug("SRA B");
    _SRA(R.B);
}

static void _SRA_C()
{
    LogDebug("SRA C");
    _SRA(R.C);
}

static void _SRA_D()
{
    LogDebug("SRA D");
    _SRA(R.D);
}

static void _SRA_E()
{
    LogDebug("SRA E");
    _SRA(R.E);
}

static void _SRA_H()
{
    LogDebug("SRA H");
    _SRA(R.H);
}

static void _SRA_L()
{
    LogDebug("SRA L");
    _SRA(R.L);
}

static void _SRA_pHL()
{
    LogDebug("_SRA (HL)");
    uint8_t n = readByte(R.HL);
    TickCounter += 4;

    _SRA(n);

    writeByte(R.HL, n);
    TickCounter += 4;
}

static void _SRA_A()
{
    LogDebug("SRA A");
    _SRA(R.A);
}

static void _SRL_B()
{
    LogDebug("SRL B");
    _SRL(R.B);
}

static void _SRL_C()
{
    LogDebug("SRL C");
    _SRL(R.C);
}

static void _SRL_D()
{
    LogDebug("SRL D");
    _SRL(R.D);
}

static void _SRL_E()
{
    LogDebug("SRL E");
    _SRL(R.E);
}

static void _SRL_H()
{
    LogDebug("SRL H");
    _SRL(R.H);
}

static void _SRL_L()
{
    LogDebug("SRL L");
    _SRL(R.L);
}

static void _SRL_pHL()
{
    LogDebug("_SRL (HL)");
    uint8_t n = readByte(R.HL);
    TickCounter += 4;

    _SRL(n);

    writeByte(R.HL, n);
    TickCounter += 4;
}

static void _SRL_A()
{
    LogDebug("SRL A");
    _SRL(R.A);
}

static void _SWAP_B()
{
    LogDebug("SWAP B");
    _SWAP(R.B);
    TickCounter += 4;
}

static void _SWAP_C()
{
    LogDebug("SWAP C");
    _SWAP(R.C);
    TickCounter += 4;
}

static void _SWAP_D()
{
    LogDebug("SWAP D");
    _SWAP(R.D);
    TickCounter += 4;
}

static void _SWAP_E()
{
    LogDebug("SWAP E");
    _SWAP(R.E);
    TickCounter += 4;
}

static void _SWAP_H()
{
    LogDebug("SWAP H");
    _SWAP(R.H);
    TickCounter += 4;
}

static void _SWAP_L()
{
    LogDebug("SWAP L");
    _SWAP(R.L);
    TickCounter += 4;
}

static void _SWAP_pHL()
{
    LogDebug("SWAP (HL)");
    uint8_t n = readByte(R.HL);
    TickCounter += 4;

    _SWAP(n);
    TickCounter += 4;

    writeByte(R.HL, n);
    TickCounter += 4;
}

static void _SWAP_A()
{
    LogDebug("SWAP A");
    _SWAP(R.A);
    TickCounter += 4;
}

#undef _SLA
#undef _SRA
#undef _SRL
#undef _SWAP

#endif // SHIFT_H