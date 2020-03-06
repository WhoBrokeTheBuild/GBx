#ifndef RESTART_H
#define RESTART_H

#include "clock.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"

static void _RST_00()
{
    LogInst("RST 00h");

    pushWord(R.PC);
    tick(8);

    R.PC = 0x00;
    tick(4);
}

static void _RST_08()
{
    LogInst("RST 08h");

    pushWord(R.PC);
    tick(8);

    R.PC = 0x08;
    tick(4);
}

static void _RST_10()
{
    LogInst("RST 10h");

    pushWord(R.PC);
    tick(8);

    R.PC = 0x10;
    tick(4);
}

static void _RST_18()
{
    LogInst("RST 18h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x18;
    tick(4);
}

static void _RST_20()
{
    LogInst("RST 20h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x20;
    tick(4);
}

static void _RST_28()
{
    LogInst("RST 28h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x28;
    tick(4);
}

static void _RST_30()
{
    LogInst("RST 30h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x30;
    tick(4);
}

static void _RST_38()
{
    LogInst("RST 38h");
    
    pushWord(R.PC);
    tick(8);

    R.PC = 0x38;
    tick(4);
}

#endif // RESTART_H