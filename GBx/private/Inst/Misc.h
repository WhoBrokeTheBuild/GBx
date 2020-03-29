#ifndef INST_MISC_H
#define INST_MISC_H

#include <GBx/ALU.h>
#include <GBx/CPU.h>
#include <GBx/LCD.h>
#include <GBx/Log.h>
#include <GBx/Interrupts.h>
#include <GBx/MMU.h>

static void _NOP()
{
    LogInst("NOP");
}

static void _HALT()
{
    LogInst("HALT");
    CPUEnabled = false;
}

static void _STOP()
{
    LogInst("STOP");
    ++R.PC;
    LCDC.Enabled = false;
}

static void _DI()
{
    LogInst("DI");
    IME = false;
}

static void _EI()
{
    LogInst("EI");
    NextInstruction();
    IME = true;
}

static void _DAA()
{
    LogInst("DAA");
    if (R.FN) {
        // Subtraction
        if (R.FC) {
            R.A -= 0x60;
        }
        if (R.FH) {
            R.A -= 0x06;
        }
    } else {
        // Addition
        if (R.FC || R.A > 0x99) {
            R.A += 0x60;
            R.FC = true;
        }
        if (R.FH || (R.A & 0x0F) > 0x09) {
            R.A += 0x06;
        }
    }

    R.FZ = (R.A == 0);
    R.FH = false;
}

static void _SCF()
{
    LogInst("SCF");
    R.FN = false;
    R.FH = false;
    R.FC = true;
}

static void _CPL()
{
    LogInst("CPL");
    R.A = ~R.A;
    R.FN = true;
    R.FH = true;
}

static void _CCF()
{
    LogInst("CCF");
    R.FN = false;
    R.FH = false;
    R.FC = !R.FC;
}


#endif // INST_MISC_H