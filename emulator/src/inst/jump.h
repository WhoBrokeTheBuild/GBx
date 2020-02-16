#ifndef JUMP_H
#define JUMP_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _JP_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("JP %04Xh", uu);

    R.PC = uu;
    cpuTick(4);
}

static void _JP_NZ_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("JP NZ,%04Xh", uu);

    if (!R.FZ) {
        R.PC = uu;
        cpuTick(4);
    }
}

static void _JP_Z_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("JP Z,%04Xh", uu);

    if (R.FZ) {
        R.PC = uu;
        cpuTick(4);
    }
}

static void _JP_NC_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("JP NC,%04Xh", uu);

    if (!R.FC) {
        R.PC = uu;
        cpuTick(4);
    }
}

static void _JP_C_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("JP C,%04Xh", uu);

    if (R.FC) {
        R.PC = uu;
        cpuTick(4);
    }
}

static void _JR_s()
{
    int8_t s = nextByte();
    cpuTick(4);

    LogDebug("JR %+d", s);

    R.PC += s;
    cpuTick(4);
}

static void _JR_NZ_s()
{
    int8_t s = nextByte();
    cpuTick(4);

    LogDebug("JR NZ,%+d", s);

    if (!R.FZ) {
        R.PC += s;
        cpuTick(4);
    }
}

static void _JR_Z_s()
{
    int8_t s = nextByte();
    cpuTick(4);

    LogDebug("JR Z,%+d", s);

    if (R.FZ) {
        R.PC += s;
        cpuTick(4);
    }
}

static void _JR_NC_s()
{
    int8_t s = nextByte();
    cpuTick(4);

    LogDebug("JR NC,%+d", s);

    if (!R.FC) {
        R.PC += s;
        cpuTick(4);
    }
}

static void _JR_C_s()
{
    int8_t s = nextByte();
    cpuTick(4);

    LogDebug("JR C,%+d", s);

    if (R.FC) {
        R.PC += s;
        cpuTick(4);
    }
}

static void _JP_pHL()
{
    int16_t pHL = readWord(R.HL);
    cpuTick(8);

    LogDebug("JP (HL)");

    R.PC = pHL;
    cpuTick(4);
}

#endif // JUMP_H