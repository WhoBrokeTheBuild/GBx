#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "log.h"
#include "memory.h"
#include "register.h"

static void opADD_BC() 
{
    LogVerbose("%s", __FUNCTION__);
    R.HL += R.BC;
}

static void opADD_DE() 
{
    LogVerbose("%s", __FUNCTION__);
    R.HL += R.DE;
}

static void opADD_HL() 
{
    LogVerbose("%s", __FUNCTION__);
    R.HL += R.HL;
}

static void opADD_SP() 
{
    LogVerbose("%s", __FUNCTION__);
    R.HL += R.SP;
}

#endif // ARITHMETIC_H