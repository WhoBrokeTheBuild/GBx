#ifndef CALLS_H
#define CALLS_H

#include "log.h"
#include "memory.h"
#include "register.h"

static void opCALL() 
{
    LogVerbose("%s", __FUNCTION__);
    writeByte(R.SP--, readByte(R.PC + 2));
    writeByte(R.SP--, readByte(R.PC + 3));
    R.PC = readByte(R.PC++) | (readByte(R.PC++) << 8);
}

static void opRET()
{
    LogVerbose("%s", __FUNCTION__);
    R.PC = (readByte(R.SP++) << 8) | readByte(R.SP++);
}

#endif // CALLS_H