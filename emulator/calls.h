#ifndef CALLS_H
#define CALLS_H

#include "log.h"
#include "memory.h"
#include "register.h"

static void opCALL() 
{
    uint16_t nn = (readByte(R.PC++) << 8) | readByte(R.PC++);
    //RAM[R.SP]
}

#endif // CALLS_H