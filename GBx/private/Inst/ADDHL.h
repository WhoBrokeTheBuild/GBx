#ifndef INST_ADDHL_H
#define INST_ADDHL_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>

static void _ADD_HL_BC()
{
    LogInst("ADD HL,BC");
    R.HL = AddWords(R.HL, R.BC);
    Tick(4);
}

static void _ADD_HL_DE()
{
    LogInst("ADD HL,DE");
    R.HL = AddWords(R.HL, R.DE);
    Tick(4);
}

static void _ADD_HL_HL()
{
    LogInst("ADD HL,HL");
    R.HL = AddWords(R.HL, R.HL);
    Tick(4);
}

static void _ADD_HL_SP()
{
    LogInst("ADD HL,SP");
    R.HL = AddWords(R.HL, R.SP);
    Tick(4);
}

#endif // INST_ADDHL_H