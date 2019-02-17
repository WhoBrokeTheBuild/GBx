#ifndef JUMPS_H
#define JUMPS_H

#include "log.h"
#include "memory.h"
#include "register.h"

static void opJP() 
{
    LogVerbose("%s", __FUNCTION__);
    R.PC = (readByte(R.PC++) << 8) | readByte(R.PC++);
}

static void opJP_NZ() 
{
    LogVerbose("%s", __FUNCTION__);
}

static void opJP_Z() 
{
    LogVerbose("%s", __FUNCTION__);
}

static void opJP_NC() 
{
    LogVerbose("%s", __FUNCTION__);
}

static void opJP_C() 
{
    LogVerbose("%s", __FUNCTION__);
}

static void opRET()
{
    LogVerbose("%s", __FUNCTION__);
    R.PC = (readByte(R.SP++) << 8) | readByte(R.SP++);
}

#endif // JUMPS_H