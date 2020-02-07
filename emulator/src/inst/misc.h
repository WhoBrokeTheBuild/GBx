#ifndef MISC_H
#define MISC_H

#include "../cpu.h"
#include "../log.h"
#include "../interrupt.h"
#include "../register.h"
#include "../video.h"

static void _NOP()
{
    LogDebug("NOP");
}

static void _HALT()
{
    LogDebug("HALT");
    CPUEnabled = false;
    cpuTick(4);
}

static void _STOP()
{
    LogDebug("STOP");
    ++R.PC;
    LCDC.LCDEnable = false;
    cpuTick(4);
}

static void _DI()
{
    LogDebug("DI");
    IME = false;
    cpuTick(4);
}

static void _EI()
{
    LogDebug("EI");
    IME = true;
    cpuTick(4);
}

static void _DAA()
{
    LogDebug("DAA");
    if (!R.FN) {
        if (R.FC || R.A > 0x99) {
            R.A += 0x60;
            R.FC = true;
        }
        if (R.FH || (R.A & 0x0F) > 0x09) {
            R.A += 0x06;
        }
    } else {
        if (R.FC) {
            R.A -= 0x60;
        }
        if (R.FH) {
            R.A -= 0x06;
        }
    }

    R.FZ = (R.A == 0);
    R.FH = false;
}

static void _SCF()
{
    LogDebug("CPL");
    R.FN = false;
    R.FH = false;
    R.FC = true;
}

static void _CPL()
{
    LogDebug("CPL");
    R.A = ~R.A;
    R.FN = true;
    R.FH = true;
}

static void _CCF()
{
    LogDebug("CCF");
    R.FN = false;
    R.FH = false;
    R.FC = ~R.C;
}


#endif // MISC_H