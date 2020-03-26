#ifndef CALL_H
#define CALL_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

static void _CALL_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("CALL $%04X", uu);

    PushWord(R.PC);
    Tick(8);

    R.PC = uu;
    Tick(4);
}

static void _CALL_NZ_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("CALL NZ,$%04X", uu);

    if (!R.FZ) {
        PushWord(R.PC);
        Tick(8);

        R.PC = uu;
        Tick(4);
    }
}

static void _CALL_Z_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("CALL Z,$%04X", uu);

    if (R.FZ) {
        PushWord(R.PC);
        Tick(8);

        R.PC = uu;
        Tick(4);
    }
}

static void _CALL_NC_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("CALL NC,$%04X", uu);

    if (!R.FC) {
        PushWord(R.PC);
        Tick(8);

        R.PC = uu;
        Tick(4);
    }
}

static void _CALL_C_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("CALL C,$%04X", uu);

    if (R.FC) {
        PushWord(R.PC);
        Tick(8);

        R.PC = uu;
        Tick(4);
    }
}


#endif // CALL_H