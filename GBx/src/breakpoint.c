#include "breakpoint.h"

#include <GBx/cpu.h>
#include <GBx/log.h>

#include <stdlib.h>
#include <string.h>

typedef bool (*break_comp_func_t)(unsigned);

typedef struct
{
    char *            name;
    break_comp_func_t comp;
    unsigned          value;
    
} break_cond_t;

#define MAX_BREAKPOINTS (10)

break_cond_t breakpoints[MAX_BREAKPOINTS];

bool breakCompareA(unsigned value)
{
    return (R.A == value);
}

bool breakCompareB(unsigned value)
{
    return (R.B == value);
}

bool breakCompareC(unsigned value)
{
    return (R.C == value);
}

bool breakCompareD(unsigned value)
{
    return (R.D == value);
}

bool breakCompareE(unsigned value)
{
    return (R.E == value);
}

bool breakCompareH(unsigned value)
{
    return (R.H == value);
}

bool breakCompareL(unsigned value)
{
    return (R.L == value);
}

bool breakCompareAF(unsigned value)
{
    return (R.AF == value);
}

bool breakCompareBC(unsigned value)
{
    return (R.BC == value);
}

bool breakCompareDE(unsigned value)
{
    return (R.DE == value);
}

bool breakCompareHL(unsigned value)
{
    return (R.HL == value);
}

bool breakComparePC(unsigned value)
{
    return (R.PC == value);
}

bool breakCompareSP(unsigned value)
{
    return (R.SP == value);
}

bool breakCompareFZ(unsigned value)
{
    return (R.FZ == (bool)value);
}

bool breakCompareFN(unsigned value)
{
    return (R.FN == (bool)value);
}

bool breakCompareFH(unsigned value)
{
    return (R.FH == (bool)value);
}

bool breakCompareFC(unsigned value)
{
    return (R.FC == (bool)value);
}

break_comp_func_t getBreakCompFunc(const char * reg)
{
    if (reg[0] == '\0') {
        // Error
    }
    else if (reg[1] == '\0') {
        if (reg[0] == 'A') {
            return breakCompareA;
        }
        else if (reg[0] == 'B') {
            return breakCompareB;
        }
        else if (reg[0] == 'C') {
            return breakCompareC;
        }
        else if (reg[0] == 'D') {
            return breakCompareD;
        }
        else if (reg[0] == 'E') {
            return breakCompareE;
        }
        else if (reg[0] == 'H') {
            return breakCompareH;
        }
        else if (reg[0] == 'L') {
            return breakCompareL;
        }
    }
    else if (reg[2] == '\0') {
        if (reg[0] == 'A' && reg[1] == 'F') {
            return breakCompareAF;
        }
        else if (reg[0] == 'B' && reg[1] == 'C') {
            return breakCompareBC;
        }
        else if (reg[0] == 'D' && reg[1] == 'E') {
            return breakCompareDE;
        }
        else if (reg[0] == 'H' && reg[1] == 'L') {
            return breakCompareHL;
        }
        else if (reg[0] == 'S' && reg[1] == 'P') {
            return breakCompareSP;
        }
        else if (reg[0] == 'P' && reg[1] == 'C') {
            return breakComparePC;
        }
        else if (reg[0] == 'F') {
            if (reg[1] == 'Z') {
                return breakCompareFZ;
            }
            else if (reg[1] == 'N') {
                return breakCompareFN;
            }
            else if (reg[1] == 'H') {
                return breakCompareFH;
            }
            else if (reg[1] == 'C') {
                return breakCompareFC;
            }
        }
    }
    
    return NULL;
}

void SetBreakpoint(const char * reg, unsigned value)
{
    break_comp_func_t comp = getBreakCompFunc(reg);
    
    if (!comp) {
        LogError("Unknown breakpoint register '%s'", reg);
        return;
    }
    
    for (int i = 0; i < MAX_BREAKPOINTS; ++i) {
        if (!breakpoints[i].comp) {
            breakpoints[i].name = strdup(reg);
            breakpoints[i].comp = comp;
            breakpoints[i].value = value;
            return;
        }
    }
    
    LogWarn("Maximum number of breakpoints reached: %d", MAX_BREAKPOINTS);
}

void ClearBreakpoint(const char * reg, unsigned value)
{
    break_comp_func_t comp = getBreakCompFunc(reg);
    
    if (!comp) {
        LogError("Unknown breakpoint register '%s'", reg);
        return;
    }
    
    for (int i = 0; i < MAX_BREAKPOINTS; ++i) {
        if (breakpoints[i].comp == comp) {
            free(breakpoints[i].name);
            breakpoints[i].name = NULL;
            breakpoints[i].comp = NULL;
            return;
        }
    }
    
}

void ClearAllBreakpoints()
{
    for (int i = 0; i < MAX_BREAKPOINTS; ++i) {
        if (breakpoints[i].name) {
            free(breakpoints[i].name);
            breakpoints[i].name = NULL;
        }
        breakpoints[i].comp = NULL;
    }
}

bool AtBreakpoint()
{
    for (int i = 0; i < MAX_BREAKPOINTS; ++i) {
        if (breakpoints[i].comp) {
            if (breakpoints[i].comp(breakpoints[i].value)) {
                free(breakpoints[i].name);
                breakpoints[i].name = NULL;
                breakpoints[i].comp = NULL;
                return true;
            }
        }
    }
    
    return false;
}

void PrintBreakpoints()
{
    for (int i = 0; i < MAX_BREAKPOINTS; ++i) {
        if (breakpoints[i].name) {
            LogInfo("Breakpoint when %s=%04X", breakpoints[i].name, breakpoints[i].value);
        }
    }
}