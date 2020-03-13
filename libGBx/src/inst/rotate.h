#ifndef ROTATE_H
#define ROTATE_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

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
    LogInst("RLC B");
    _RLC(R.B);
}

static void _RLC_C()
{
    LogInst("RLC C");
    _RLC(R.C);
}

static void _RLC_D()
{
    LogInst("RLC D");
    _RLC(R.D);
}

static void _RLC_E()
{
    LogInst("RLC E");
    _RLC(R.E);
}

static void _RLC_H()
{
    LogInst("RLC H");
    _RLC(R.H);
}

static void _RLC_L()
{
    LogInst("RLC L");
    _RLC(R.L);
}

static void _RLC_pHL()
{
    LogInst("RLC (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    _RLC(n);

    WriteByte(R.HL, n);
    Tick(4);
}

static void _RLC_A()
{
    LogInst("RLC A");
    _RLC(R.A);
}

static void _RLCA()
{
    LogInst("RLCA");
    _RLC(R.A);
}

static void _RRC_B()
{
    LogInst("RRC B");
    _RRC(R.B);
}

static void _RRC_C()
{
    LogInst("RRC C");
    _RRC(R.C);
}

static void _RRC_D()
{
    LogInst("RRC D");
    _RRC(R.D);
}

static void _RRC_E()
{
    LogInst("RRC E");
    _RRC(R.E);
}

static void _RRC_H()
{
    LogInst("RRC H");
    _RRC(R.H);
}

static void _RRC_L()
{
    LogInst("RRC L");
    _RRC(R.L);
}

static void _RRC_pHL()
{
    LogInst("RRC (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    _RRC(n);

    WriteByte(R.HL, n);
    Tick(4);
}

static void _RRC_A()
{
    LogInst("RRC A");
    _RRC(R.A);
}

static void _RRCA()
{
    LogInst("RRCA");
    _RRC(R.A);
}

static void _RL_B()
{
    LogInst("RL B");
    _RL(R.B);
}

static void _RL_C()
{
    LogInst("RL C");
    _RL(R.C);
}

static void _RL_D()
{
    LogInst("RL D");
    _RL(R.D);
}

static void _RL_E()
{
    LogInst("RL E");
    _RL(R.E);
}

static void _RL_H()
{
    LogInst("RL H");
    _RL(R.H);
}

static void _RL_L()
{
    LogInst("RL L");
    _RL(R.L);
}

static void _RL_pHL()
{
    LogInst("RL (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    _RL(n);

    WriteByte(R.HL, n);
    Tick(4);
}

static void _RL_A()
{
    LogInst("RL A");
    _RL(R.A);
}

static void _RLA()
{
    LogInst("RL A");
    _RL(R.A);
}

static void _RR_B()
{
    LogInst("RR B");
    _RR(R.B);
}

static void _RR_C()
{
    LogInst("RR C");
    _RR(R.C);
}

static void _RR_D()
{
    LogInst("RR D");
    _RR(R.D);
}

static void _RR_E()
{
    LogInst("RR E");
    _RR(R.E);
}

static void _RR_H()
{
    LogInst("RR H");
    _RR(R.H);
}

static void _RR_L()
{
    LogInst("RR L");
    _RR(R.L);
}

static void _RR_pHL()
{
    LogInst("RR (HL)");
    byte n = ReadByte(R.HL);
    Tick(4);

    _RR(n);

    WriteByte(R.HL, n);
    Tick(4);
}

static void _RR_A()
{
    LogInst("RR A");
    _RR(R.A);
}

static void _RRA()
{
    LogInst("RR A");
    _RR(R.A);
}

#undef _RLC
#undef _RRC
#undef _RL
#undef _RR

#endif // ROTATE_H