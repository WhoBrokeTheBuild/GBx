#include "breakpoint.h"

#include <GBx/GBx.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef bool (*break_comp_func_t)(gbx_t *, unsigned);

typedef struct
{
    char *            name;
    break_comp_func_t comp;
    unsigned          value;
    
} break_cond_t;

#define MAX_BREAKPOINTS (10)

break_cond_t breakpoints[MAX_BREAKPOINTS];

bool breakCompareA(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->A == value);
}

bool breakCompareB(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->B == value);
}

bool breakCompareC(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->C == value);
}

bool breakCompareD(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->D == value);
}

bool breakCompareE(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->E == value);
}

bool breakCompareH(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->H == value);
}

bool breakCompareL(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->L == value);
}

bool breakCompareAF(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->AF == value);
}

bool breakCompareBC(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->BC == value);
}

bool breakCompareDE(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->DE == value);
}

bool breakCompareHL(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->HL == value);
}

bool breakComparePC(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->PC == value);
}

bool breakCompareSP(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->SP == value);
}

bool breakCompareFZ(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->FZ == (bool)value);
}

bool breakCompareFN(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->FN == (bool)value);
}

bool breakCompareFH(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->FH == (bool)value);
}

bool breakCompareFC(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->FC == (bool)value);
}

bool breakCompareLY(gbx_t * ctx, unsigned value)
{
    return (ctx->LY == value);
}

bool breakStackChanged(gbx_t * ctx, unsigned value)
{
    return (ctx->CPU->SP != value);
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
        else if (reg[0] == 'L' && reg[1] == 'Y') {
            return breakCompareLY;
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
        fprintf(stderr, "Unknown breakpoint register '%s'\n", reg);
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
    
    fprintf(stderr, "Maximum number of breakpoints reached: %d", MAX_BREAKPOINTS);
}

void SetBreakpointStackChanged(gbx_t * ctx)
{
    for (int i = 0; i < MAX_BREAKPOINTS; ++i) {
        if (!breakpoints[i].comp) {
            breakpoints[i].name = strdup("SP*");
            breakpoints[i].comp = breakStackChanged;
            breakpoints[i].value = ctx->CPU->SP;
            return;
        }
    }
    
    fprintf(stderr, "Maximum number of breakpoints reached: %d", MAX_BREAKPOINTS);
}

void ClearBreakpoint(const char * reg, unsigned value)
{
    break_comp_func_t comp = getBreakCompFunc(reg);
    
    if (!comp) {
        fprintf(stderr, "Unknown breakpoint register '%s'\n", reg);
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

bool AtBreakpoint(gbx_t * ctx)
{
    for (int i = 0; i < MAX_BREAKPOINTS; ++i) {
        if (breakpoints[i].comp) {
            if (breakpoints[i].comp(ctx, breakpoints[i].value)) {
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
            printf("Breakpoint when %s=%04X\n", breakpoints[i].name, breakpoints[i].value);
        }
    }
}