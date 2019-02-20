#ifndef CALL_H
#define CALL_H

#include "../memory.h"
#include "../register.h"

static void _CALL_nn()
{
    uint16_t nn = nextWord();
    TickCounter += 4;

    LogVerbose("CALL %02X", nn);

    pushWord(R.PC);
    TickCounter += 4;

    R.PC = nn;
    TickCounter += 4;
}

static void _CALL_NZ_nn()
{
    uint16_t nn = nextWord();
    TickCounter += 4;

    LogVerbose("CALL NZ,%02X", nn);

    if (!R.FZ) {
        pushWord(R.PC);
        TickCounter += 4;

        R.PC = nn;
        TickCounter += 4;
    }
}

static void _CALL_Z_nn()
{
    uint16_t nn = nextWord();
    TickCounter += 4;

    LogVerbose("CALL Z,%02X", nn);

    if (R.FZ) {
        pushWord(R.PC);
        TickCounter += 4;

        R.PC = nn;
        TickCounter += 4;
    }
}

static void _CALL_NC_nn()
{
    uint16_t nn = nextWord();
    TickCounter += 4;

    LogVerbose("CALL NC,%02X", nn);

    if (!R.C) {
        pushWord(R.PC);
        TickCounter += 4;

        R.PC = nn;
        TickCounter += 4;
    }
}

static void _CALL_C_nn()
{
    uint16_t nn = nextWord();
    TickCounter += 4;

    LogVerbose("CALL C,%02X", nn);

    if (R.C) {
        pushWord(R.PC);
        TickCounter += 4;

        R.PC = nn;
        TickCounter += 4;
    }
}


#endif // CALL_H