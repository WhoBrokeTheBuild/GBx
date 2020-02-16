#ifndef CALL_H
#define CALL_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _CALL_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("CALL %04Xh", uu);

    pushWord(R.PC);
    cpuTick(8);

    R.PC = uu;
    cpuTick(4);
}

static void _CALL_NZ_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("CALL NZ,%04Xh", uu);

    if (!R.FZ) {
        pushWord(R.PC);
        cpuTick(8);

        R.PC = uu;
        cpuTick(4);
    }
}

static void _CALL_Z_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("CALL Z,%04Xh", uu);

    if (R.FZ) {
        pushWord(R.PC);
        cpuTick(8);

        R.PC = uu;
        cpuTick(4);
    }
}

static void _CALL_NC_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("CALL NC,%04Xh", uu);

    if (!R.FC) {
        pushWord(R.PC);
        cpuTick(8);

        R.PC = uu;
        cpuTick(4);
    }
}

static void _CALL_C_uu()
{
    uint16_t uu = nextWord();
    cpuTick(8);

    LogDebug("CALL C,%04Xh", uu);

    if (R.FC) {
        pushWord(R.PC);
        cpuTick(8);

        R.PC = uu;
        cpuTick(4);
    }
}


#endif // CALL_H