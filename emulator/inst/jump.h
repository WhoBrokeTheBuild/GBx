#ifndef JUMP_H
#define JUMP_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _JP_nn()
{
    uint16_t nn = nextWord();
    cpuTick(8);

    LogDebug("JP %04Xh", nn);

    R.PC = nn;
    cpuTick(4);
}

static void _JP_NZ_nn()
{
    uint16_t nn = nextWord();
    cpuTick(8);

    LogDebug("JP NZ,%04Xh", nn);

    if (!R.FZ) {
        R.PC = nn;
        cpuTick(4);
    }
}

static void _JP_Z_nn()
{
    uint16_t nn = nextWord();
    cpuTick(8);

    LogDebug("JP Z,%04Xh", nn);

    if (R.FZ) {
        R.PC = nn;
        cpuTick(4);
    }
}

static void _JP_NC_nn()
{
    uint16_t nn = nextWord();
    cpuTick(8);

    LogDebug("JP NC,%04Xh", nn);

    if (!R.FC) {
        R.PC = nn;
        cpuTick(4);
    }
}

static void _JP_C_nn()
{
    uint16_t nn = nextWord();
    cpuTick(8);

    LogDebug("JP C,%04Xh", nn);

    if (R.FC) {
        R.PC = nn;
        cpuTick(4);
    }
}

static void _JR_n()
{
    int8_t n = nextByte();
    cpuTick(4);

    LogDebug("JR %d", n);

    R.PC += n;
    cpuTick(4);
}

static void _JR_NZ_n()
{
    int8_t n = nextByte();
    cpuTick(4);

    LogDebug("JR NZ,%d", n);

    if (!R.FZ) {
        R.PC += n;
        cpuTick(4);
    }
}

static void _JR_Z_n()
{
    int8_t n = nextByte();
    cpuTick(4);

    LogDebug("JR Z,%d", n);

    if (R.FZ) {
        R.PC += n;
        cpuTick(4);
    }
}

static void _JR_NC_n()
{
    int8_t n = nextByte();
    cpuTick(4);

    LogDebug("JR NC,%d", n);

    if (!R.FC) {
        R.PC += n;
        cpuTick(4);
    }
}

static void _JR_C_n()
{
    int8_t n = nextByte();
    cpuTick(4);

    LogDebug("JR C,%d", n);

    if (R.FC) {
        R.PC += n;
        cpuTick(4);
    }
}

#endif // JUMP_H