#ifndef MISC_H
#define MISC_H

#include "../cpu.h"
#include "../log.h"
#include "../interrupt.h"
#include "../register.h"
#include "../lcd.h"

static void _NOP()
{
    LogVerbose(3, "NOP");
}

static void _HALT()
{
    LogVerbose(3, "HALT");
    CPUEnabled = false;
}

static void _STOP()
{
    LogVerbose(3, "STOP");
    ++R.PC;
    LCDC.LCDEnable = false;
}

static void _DI()
{
    LogVerbose(3, "DI");
    IME = false;
}

static void _EI()
{
    LogVerbose(3, "EI");
    IME = true;
}

static void _DAA()
{
    LogVerbose(3, "DAA");
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
    LogVerbose(3, "SCF");
    R.FN = false;
    R.FH = false;
    R.FC = true;
}

static void _CPL()
{
    LogVerbose(3, "CPL");
    R.A = ~R.A;
    R.FN = true;
    R.FH = true;
}

static void _CCF()
{
    LogVerbose(3, "CCF");
    R.FN = false;
    R.FH = false;
    R.FC = ~R.C;
}


#endif // MISC_H