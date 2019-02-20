#ifndef RESTART_H
#define RESTART_H

static void _RST_00()
{
    LogVerbose("RST 00");

    pushWord(R.PC);
    TickCounter += 4;

    R.PC = 0x00;
    TickCounter += 28;
}

static void _RST_08()
{
    LogVerbose("RST 08");

    pushWord(R.PC);
    TickCounter += 4;

    R.PC = 0x08;
    TickCounter += 28;
}

static void _RST_10()
{
    LogVerbose("RST 10");

    pushWord(R.PC);
    TickCounter += 4;

    R.PC = 0x10;
    TickCounter += 28;
}

static void _RST_18()
{
    LogVerbose("RST 18");
    
    pushWord(R.PC);
    TickCounter += 4;

    R.PC = 0x18;
    TickCounter += 28;
}

static void _RST_20()
{
    LogVerbose("RST 20");
    
    pushWord(R.PC);
    TickCounter += 4;

    R.PC = 0x20;
    TickCounter += 28;
}

static void _RST_28()
{
    LogVerbose("RST 28");
    
    pushWord(R.PC);
    TickCounter += 4;

    R.PC = 0x28;
    TickCounter += 28;
}

static void _RST_30()
{
    LogVerbose("RST 30");
    
    pushWord(R.PC);
    TickCounter += 4;

    R.PC = 0x30;
    TickCounter += 28;
}

static void _RST_38()
{
    LogVerbose("RST 38");
    
    pushWord(R.PC);
    TickCounter += 4;

    R.PC = 0x38;
    TickCounter += 28;
}

#endif // RESTART_H