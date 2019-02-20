#ifndef INTCTL_H
#define INTCTL_H

#include "../cpu.h"
#include "../interrupt.h"

static void _DI()
{
    IME = false;
    TickCounter += 4;
}

static void _EI()
{
    IME = true;
    TickCounter += 4;
}

#endif // INTCTL_H