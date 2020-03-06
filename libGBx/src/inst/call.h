#ifndef CALL_H
#define CALL_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

static void _CALL_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogInst("CALL %04Xh", uu);

    pushWord(R.PC);
    tick(8);

    R.PC = uu;
    tick(4);
}

static void _CALL_NZ_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogInst("CALL NZ,%04Xh", uu);

    if (!R.FZ) {
        pushWord(R.PC);
        tick(8);

        R.PC = uu;
        tick(4);
    }
}

static void _CALL_Z_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogInst("CALL Z,%04Xh", uu);

    if (R.FZ) {
        pushWord(R.PC);
        tick(8);

        R.PC = uu;
        tick(4);
    }
}

static void _CALL_NC_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogInst("CALL NC,%04Xh", uu);

    if (!R.FC) {
        pushWord(R.PC);
        tick(8);

        R.PC = uu;
        tick(4);
    }
}

static void _CALL_C_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogInst("CALL C,%04Xh", uu);

    if (R.FC) {
        pushWord(R.PC);
        tick(8);

        R.PC = uu;
        tick(4);
    }
}


#endif // CALL_H