#ifndef ROTATE_H
#define ROTATE_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

#define _RLC(x) \
    R.FC = (((x) & 0x80) == 0x80); \
    (x) = ((x) << 1) | R.FC; \
    R.FZ = ((x) == 0); \
    R.FN = false; \
    R.FH = false

#define _RRC(x) \
    R.FC = (((x) & 0x01) == 0x01); \
    (x) = ((x) >> 1) | (R.FC << 7); \
    R.FZ = ((x) == 0); \
    R.FN = false; \
    R.FH = false

#define _RL(x) \
    bool tmp = (((x) & 0x80) == 0x80); \
    (x) = ((x) << 1) | R.FC; \
    R.FC = tmp; \
    R.FZ = ((x) == 0); \
    R.FN = false; \
    R.FH = false

#define _RR(x) \
    bool tmp = (((x) & 0x01) == 0x01); \
    (x) = ((x) >> 1) | (R.FC << 7); \
    R.FC = tmp; \
    R.FZ = ((x) == 0); \
    R.FN = false; \
    R.FH = false

static void _RLC_B()
{
    LogDebug("RLC B");
    _RLC(R.B);
}

static void _RLC_C()
{
    LogDebug("RLC C");
    _RLC(R.C);
}

static void _RLC_D()
{
    LogDebug("RLC D");
    _RLC(R.D);
}

static void _RLC_E()
{
    LogDebug("RLC E");
    _RLC(R.E);
}

static void _RLC_H()
{
    LogDebug("RLC H");
    _RLC(R.H);
}

static void _RLC_L()
{
    LogDebug("RLC L");
    _RLC(R.L);
}

static void _RLC_pHL()
{
    LogDebug("RLC (HL)");
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _RLC(n);

    writeByte(R.HL, n);
    cpuTick(4);
}

static void _RLC_A()
{
    LogDebug("RLC A");
    _RLC(R.A);
}

static void _RLCA()
{
    LogDebug("RLCA");
    _RLC(R.A);
}

static void _RRC_B()
{
    LogDebug("RRC B");
    _RRC(R.B);
}

static void _RRC_C()
{
    LogDebug("RRC C");
    _RRC(R.C);
}

static void _RRC_D()
{
    LogDebug("RRC D");
    _RRC(R.D);
}

static void _RRC_E()
{
    LogDebug("RRC E");
    _RRC(R.E);
}

static void _RRC_H()
{
    LogDebug("RRC H");
    _RRC(R.H);
}

static void _RRC_L()
{
    LogDebug("RRC L");
    _RRC(R.L);
}

static void _RRC_pHL()
{
    LogDebug("RRC (HL)");
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _RRC(n);

    writeByte(R.HL, n);
    cpuTick(4);
}

static void _RRC_A()
{
    LogDebug("RRC A");
    _RRC(R.A);
}

static void _RRCA()
{
    LogDebug("RRCA");
    _RRC(R.A);
}

static void _RL_B()
{
    LogDebug("RL B");
    _RL(R.B);
}

static void _RL_C()
{
    LogDebug("RL C");
    _RL(R.C);
}

static void _RL_D()
{
    LogDebug("RL D");
    _RL(R.D);
}

static void _RL_E()
{
    LogDebug("RL E");
    _RL(R.E);
}

static void _RL_H()
{
    LogDebug("RL H");
    _RL(R.H);
}

static void _RL_L()
{
    LogDebug("RL L");
    _RL(R.L);
}

static void _RL_pHL()
{
    LogDebug("RL (HL)");
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _RL(n);

    writeByte(R.HL, n);
    cpuTick(4);
}

static void _RL_A()
{
    LogDebug("RL A");
    _RL(R.A);
}

static void _RLA()
{
    LogDebug("RL A");
    _RL(R.A);
}

static void _RR_B()
{
    LogDebug("RR B");
    _RR(R.B);
}

static void _RR_C()
{
    LogDebug("RR C");
    _RR(R.C);
}

static void _RR_D()
{
    LogDebug("RR D");
    _RR(R.D);
}

static void _RR_E()
{
    LogDebug("RR E");
    _RR(R.E);
}

static void _RR_H()
{
    LogDebug("RR H");
    _RR(R.H);
}

static void _RR_L()
{
    LogDebug("RR L");
    _RR(R.L);
}

static void _RR_pHL()
{
    LogDebug("RR (HL)");
    uint8_t n = readByte(R.HL);
    cpuTick(4);

    _RR(n);

    writeByte(R.HL, n);
    cpuTick(4);
}

static void _RR_A()
{
    LogDebug("RR A");
    _RR(R.A);
}

static void _RRA()
{
    LogDebug("RR A");
    _RR(R.A);
}

#undef _RLC
#undef _RRC
#undef _RL
#undef _RR

#endif // ROTATE_H