#ifndef INST_JP_H
#define INST_JP_H

#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _JP_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP $%04X", uu);

    R.PC = uu;
    Tick(4);
}

static void _JP_NZ_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP NZ,$%04X", uu);

    if (!R.FZ) {
        R.PC = uu;
        Tick(4);
    }
}

static void _JP_Z_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP Z,$%04X", uu);

    if (R.FZ) {
        R.PC = uu;
        Tick(4);
    }
}

static void _JP_NC_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP NC,$%04X", uu);

    if (!R.FC) {
        R.PC = uu;
        Tick(4);
    }
}

static void _JP_C_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP C,$%04X", uu);

    if (R.FC) {
        R.PC = uu;
        Tick(4);
    }
}

static void _JP_HL()
{
    LogInst("JP HL");

    R.PC = R.HL;
    Tick(4);
}

#endif // INST_JP_H