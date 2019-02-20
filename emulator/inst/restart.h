#ifndef RESTART_H
#define RESTART_H

#include "../cpu.h"
#include "../log.h"
#include "../memory.h"
#include "../register.h"

static void _RST_00()
{
    LogDebug("RST 00h");

    pushWord(R.PC);
    TickCounter += 8;

    R.PC = 0x00;
    TickCounter += 4;
}

static void _RST_08()
{
    LogDebug("RST 08h");

    pushWord(R.PC);
    TickCounter += 8;

    R.PC = 0x08;
    TickCounter += 4;
}

static void _RST_10()
{
    LogDebug("RST 10h");

    pushWord(R.PC);
    TickCounter += 8;

    R.PC = 0x10;
    TickCounter += 4;
}

static void _RST_18()
{
    LogDebug("RST 18h");
    
    pushWord(R.PC);
    TickCounter += 8;

    R.PC = 0x18;
    TickCounter += 4;
}

static void _RST_20()
{
    LogDebug("RST 20h");
    
    pushWord(R.PC);
    TickCounter += 8;

    R.PC = 0x20;
    TickCounter += 4;
}

static void _RST_28()
{
    LogDebug("RST 28h");
    
    pushWord(R.PC);
    TickCounter += 8;

    R.PC = 0x28;
    TickCounter += 4;
}

static void _RST_30()
{
    LogDebug("RST 30h");
    
    pushWord(R.PC);
    TickCounter += 8;

    R.PC = 0x30;
    TickCounter += 4;
}

static void _RST_38()
{
    LogDebug("RST 38h");
    
    pushWord(R.PC);
    TickCounter += 8;

    R.PC = 0x38;
    TickCounter += 4;
}

#endif // RESTART_H