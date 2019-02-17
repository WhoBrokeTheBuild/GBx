#ifndef LOADS_H
#define LOADS_H

#include "log.h"
#include "memory.h"
#include "register.h"

static void opLD_A_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = R.A;
}

static void opLD_B_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = R.B;
}

static void opLD_C_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = R.C;
}

static void opLD_D_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = R.D;
}

static void opLD_E_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = R.E;
}

static void opLD_H_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = R.H;
}

static void opLD_L_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = R.L;
}

static void opLD_BC_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = readByte(R.BC);
}

static void opLD_DE_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = readByte(R.DE);
}

static void opLD_HL_A()
{
    LogVerbose("%s", __FUNCTION__);
    R.A = readByte(R.HL);
}

static void opLD_nn_A()
{
    LogVerbose("%s", __FUNCTION__);
    uint16_t nn = (readByte(R.PC++) << 8) | readByte(R.PC++);
    R.A = readByte(nn);
}

static void opLD_BC() 
{
    LogVerbose("%s", __FUNCTION__);
    R.BC = (readByte(R.PC++) << 8) | readByte(R.PC++);
}

static void opLD_DE() 
{
    LogVerbose("%s", __FUNCTION__);
    R.DE = (readByte(R.PC++) << 8) | readByte(R.PC++);
}

static void opLD_HL() 
{
    LogVerbose("%s", __FUNCTION__);
    R.HL = (readByte(R.PC++) << 8) | readByte(R.PC++);
}

static void opLD_SP() 
{
    LogVerbose("%s", __FUNCTION__);
    R.SP = (readByte(R.PC++) << 8) | readByte(R.PC++);
}

#endif // LOADS_H