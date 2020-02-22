#ifndef RESTART_H
#define RESTART_H

#include "../clock.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _RST_00()
{
    LogVerbose(3, "RST 00h");

    pushWord(R.PC);
    tick(8);

    R.PC = 0x00;
    tick(4);
}

static void _RST_08()
{
    LogVerbose(3, "RST 08h");

    pushWord(R.PC);
    tick(8);

    R.PC = 0x08;
    tick(4);
}

static void _RST_10()
{
    LogVerbose(3, "RST 10h");

    pushWord(R.PC);
    tick(8);

    R.PC = 0x10;
    tick(4);
}

static void _RST_18()
{
    LogVerbose(3, "RST 18h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x18;
    tick(4);
}

static void _RST_20()
{
    LogVerbose(3, "RST 20h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x20;
    tick(4);
}

static void _RST_28()
{
    LogVerbose(3, "RST 28h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x28;
    tick(4);
}

static void _RST_30()
{
    LogVerbose(3, "RST 30h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x30;
    tick(4);
}

static void _RST_38()
{
    LogVerbose(3, "RST 38h");
    
    DebugEnable = true;

    pushWord(R.PC);
    tick(8);

    R.PC = 0x38;
    tick(4);
}

#endif // RESTART_H