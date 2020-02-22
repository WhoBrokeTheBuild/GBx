#ifndef JUMP_H
#define JUMP_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _JP_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "JP %04Xh", uu);

    R.PC = uu;
    tick(4);
}

static void _JP_NZ_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "JP NZ,%04Xh", uu);

    if (!R.FZ) {
        R.PC = uu;
        tick(4);
    }
}

static void _JP_Z_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "JP Z,%04Xh", uu);

    if (R.FZ) {
        R.PC = uu;
        tick(4);
    }
}

static void _JP_NC_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "JP NC,%04Xh", uu);

    if (!R.FC) {
        R.PC = uu;
        tick(4);
    }
}

static void _JP_C_uu()
{
    uint16_t uu = nextWord();
    tick(8);

    LogVerbose(3, "JP C,%04Xh", uu);

    if (R.FC) {
        R.PC = uu;
        tick(4);
    }
}

static void _JR_s()
{
    int8_t s = nextByte();
    tick(4);

    LogVerbose(3, "JR %+d", s);

    R.PC += s;
    tick(4);
}

static void _JR_NZ_s()
{
    int8_t s = nextByte();
    tick(4);

    LogVerbose(3, "JR NZ,%+d", s);

    if (!R.FZ) {
        R.PC += s;
        tick(4);
    }
}

static void _JR_Z_s()
{
    int8_t s = nextByte();
    tick(4);

    LogVerbose(3, "JR Z,%+d", s);

    if (R.FZ) {
        R.PC += s;
        tick(4);
    }
}

static void _JR_NC_s()
{
    int8_t s = nextByte();
    tick(4);

    LogVerbose(3, "JR NC,%+d", s);

    if (!R.FC) {
        R.PC += s;
        tick(4);
    }
}

static void _JR_C_s()
{
    int8_t s = nextByte();
    tick(4);

    LogVerbose(3, "JR C,%+d", s);

    if (R.FC) {
        R.PC += s;
        tick(4);
    }
}

static void _JP_pHL()
{
    int16_t pHL = readWord(R.HL);
    tick(8);

    LogVerbose(3, "JP (HL)");

    R.PC = pHL;
    tick(4);
}

#endif // JUMP_H