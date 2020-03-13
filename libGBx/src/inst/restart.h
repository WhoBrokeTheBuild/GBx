#ifndef RESTART_H
#define RESTART_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

static void _RST_00()
{
    LogInst("RST 00h");

    PushWord(R.PC);
    Tick(8);

    R.PC = 0x00;
    Tick(4);
}

static void _RST_08()
{
    LogInst("RST 08h");

    PushWord(R.PC);
    Tick(8);

    R.PC = 0x08;
    Tick(4);
}

static void _RST_10()
{
    LogInst("RST 10h");

    PushWord(R.PC);
    Tick(8);

    R.PC = 0x10;
    Tick(4);
}

static void _RST_18()
{
    LogInst("RST 18h");
    
    PushWord(R.PC);
    Tick(8);

    R.PC = 0x18;
    Tick(4);
}

static void _RST_20()
{
    LogInst("RST 20h");
    
    PushWord(R.PC);
    Tick(8);

    R.PC = 0x20;
    Tick(4);
}

static void _RST_28()
{
    LogInst("RST 28h");
    
    PushWord(R.PC);
    Tick(8);

    R.PC = 0x28;
    Tick(4);
}

static void _RST_30()
{
    LogInst("RST 30h");
    
    PushWord(R.PC);
    Tick(8);

    R.PC = 0x30;
    Tick(4);
}

static void _RST_38()
{
    LogInst("RST 38h");
    
    PushWord(R.PC);
    Tick(8);

    R.PC = 0x38;
    Tick(4);
}

#endif // RESTART_H