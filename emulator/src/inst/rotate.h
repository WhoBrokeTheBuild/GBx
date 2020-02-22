#ifndef ROTATE_H
#define ROTATE_H

#include "../clock.h"
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
    LogVerbose(3, "RLC B");
    _RLC(R.B);
}

static void _RLC_C()
{
    LogVerbose(3, "RLC C");
    _RLC(R.C);
}

static void _RLC_D()
{
    LogVerbose(3, "RLC D");
    _RLC(R.D);
}

static void _RLC_E()
{
    LogVerbose(3, "RLC E");
    _RLC(R.E);
}

static void _RLC_H()
{
    LogVerbose(3, "RLC H");
    _RLC(R.H);
}

static void _RLC_L()
{
    LogVerbose(3, "RLC L");
    _RLC(R.L);
}

static void _RLC_pHL()
{
    LogVerbose(3, "RLC (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _RLC(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _RLC_A()
{
    LogVerbose(3, "RLC A");
    _RLC(R.A);
}

static void _RLCA()
{
    LogVerbose(3, "RLCA");
    _RLC(R.A);
}

static void _RRC_B()
{
    LogVerbose(3, "RRC B");
    _RRC(R.B);
}

static void _RRC_C()
{
    LogVerbose(3, "RRC C");
    _RRC(R.C);
}

static void _RRC_D()
{
    LogVerbose(3, "RRC D");
    _RRC(R.D);
}

static void _RRC_E()
{
    LogVerbose(3, "RRC E");
    _RRC(R.E);
}

static void _RRC_H()
{
    LogVerbose(3, "RRC H");
    _RRC(R.H);
}

static void _RRC_L()
{
    LogVerbose(3, "RRC L");
    _RRC(R.L);
}

static void _RRC_pHL()
{
    LogVerbose(3, "RRC (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _RRC(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _RRC_A()
{
    LogVerbose(3, "RRC A");
    _RRC(R.A);
}

static void _RRCA()
{
    LogVerbose(3, "RRCA");
    _RRC(R.A);
}

static void _RL_B()
{
    LogVerbose(3, "RL B");
    _RL(R.B);
}

static void _RL_C()
{
    LogVerbose(3, "RL C");
    _RL(R.C);
}

static void _RL_D()
{
    LogVerbose(3, "RL D");
    _RL(R.D);
}

static void _RL_E()
{
    LogVerbose(3, "RL E");
    _RL(R.E);
}

static void _RL_H()
{
    LogVerbose(3, "RL H");
    _RL(R.H);
}

static void _RL_L()
{
    LogVerbose(3, "RL L");
    _RL(R.L);
}

static void _RL_pHL()
{
    LogVerbose(3, "RL (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _RL(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _RL_A()
{
    LogVerbose(3, "RL A");
    _RL(R.A);
}

static void _RLA()
{
    LogVerbose(3, "RL A");
    _RL(R.A);
}

static void _RR_B()
{
    LogVerbose(3, "RR B");
    _RR(R.B);
}

static void _RR_C()
{
    LogVerbose(3, "RR C");
    _RR(R.C);
}

static void _RR_D()
{
    LogVerbose(3, "RR D");
    _RR(R.D);
}

static void _RR_E()
{
    LogVerbose(3, "RR E");
    _RR(R.E);
}

static void _RR_H()
{
    LogVerbose(3, "RR H");
    _RR(R.H);
}

static void _RR_L()
{
    LogVerbose(3, "RR L");
    _RR(R.L);
}

static void _RR_pHL()
{
    LogVerbose(3, "RR (HL)");
    uint8_t n = readByte(R.HL);
    tick(4);

    _RR(n);

    writeByte(R.HL, n);
    tick(4);
}

static void _RR_A()
{
    LogVerbose(3, "RR A");
    _RR(R.A);
}

static void _RRA()
{
    LogVerbose(3, "RR A");
    _RR(R.A);
}

#undef _RLC
#undef _RRC
#undef _RL
#undef _RR

#endif // ROTATE_H