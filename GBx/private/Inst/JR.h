#ifndef INST_JR_H
#define INST_JR_H

#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

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

#endif // INST_JR_H