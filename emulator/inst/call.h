#ifndef CALL_H
#define CALL_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _CALL_nn()
{
    uint16_t nn = nextWord();
    tick(8);

    LogDebug("CALL %04Xh", nn);

    pushWord(R.PC);
    tick(8);

    R.PC = nn;
    tick(4);
}

static void _CALL_NZ_nn()
{
    uint16_t nn = nextWord();
    tick(8);

    LogDebug("CALL NZ,%04Xh", nn);

    if (!R.FZ) {
        pushWord(R.PC);
        tick(8);

        R.PC = nn;
        tick(4);
    }
}

static void _CALL_Z_nn()
{
    uint16_t nn = nextWord();
    tick(8);

    LogDebug("CALL Z,%04Xh", nn);

    if (R.FZ) {
        pushWord(R.PC);
        tick(8);

        R.PC = nn;
        tick(4);
    }
}

static void _CALL_NC_nn()
{
    uint16_t nn = nextWord();
    tick(8);

    LogDebug("CALL NC,%04Xh", nn);

    if (!R.C) {
        pushWord(R.PC);
        tick(8);

        R.PC = nn;
        tick(4);
    }
}

static void _CALL_C_nn()
{
    uint16_t nn = nextWord();
    tick(8);

    LogDebug("CALL C,%04Xh", nn);

    if (R.C) {
        pushWord(R.PC);
        tick(8);

        R.PC = nn;
        tick(4);
    }
}


#endif // CALL_H