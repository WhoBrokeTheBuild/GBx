#ifndef MISC_H
#define MISC_H

#include "../cpu.h"

static void _NOP()
{
    TickCounter += 4;
}

static void _HALT()
{

}

#endif // MISC_H