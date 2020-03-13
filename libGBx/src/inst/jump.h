#ifndef JUMP_H
#define JUMP_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"
#include "types.h"

static void _JP_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP %04Xh", uu);

    R.PC = uu;
    Tick(4);
}

static void _JP_NZ_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP NZ,%04Xh", uu);

    if (!R.FZ) {
        R.PC = uu;
        Tick(4);
    }
}

static void _JP_Z_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP Z,%04Xh", uu);

    if (R.FZ) {
        R.PC = uu;
        Tick(4);
    }
}

static void _JP_NC_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP NC,%04Xh", uu);

    if (!R.FC) {
        R.PC = uu;
        Tick(4);
    }
}

static void _JP_C_uu()
{
    word uu = NextWord();
    Tick(8);

    LogInst("JP C,%04Xh", uu);

    if (R.FC) {
        R.PC = uu;
        Tick(4);
    }
}

static void _JR_s()
{
    sbyte s = NextByte();
    Tick(4);

    LogInst("JR %+d", s);

    R.PC += s;
    Tick(4);
}

static void _JR_NZ_s()
{
    sbyte s = NextByte();
    Tick(4);

    LogInst("JR NZ,%+d", s);

    if (!R.FZ) {
        R.PC += s;
        Tick(4);
    }
}

static void _JR_Z_s()
{
    sbyte s = NextByte();
    Tick(4);

    LogInst("JR Z,%+d", s);

    if (R.FZ) {
        R.PC += s;
        Tick(4);
    }
}

static void _JR_NC_s()
{
    sbyte s = NextByte();
    Tick(4);

    LogInst("JR NC,%+d", s);

    if (!R.FC) {
        R.PC += s;
        Tick(4);
    }
}

static void _JR_C_s()
{
    sbyte s = NextByte();
    Tick(4);

    LogInst("JR C,%+d", s);

    if (R.FC) {
        R.PC += s;
        Tick(4);
    }
}

static void _JP_pHL()
{
    int16_t pHL = ReadWord(R.HL);
    Tick(8);

    LogInst("JP (HL)");

    R.PC = pHL;
    Tick(4);
}

#endif // JUMP_H