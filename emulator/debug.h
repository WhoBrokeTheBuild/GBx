#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stdbool.h>

extern bool DebugMode;

typedef enum {
    BKCND_NONE,
    
    BKCND_A_EQ,
    BKCND_B_EQ,
    BKCND_C_EQ,
    BKCND_BC_EQ,
    BKCND_D_EQ,
    BKCND_E_EQ,
    BKCND_DE_EQ,
    BKCND_H_EQ,
    BKCND_L_EQ,
    BKCND_HL_EQ,
    BKCND_SP_EQ,
    BKCND_PC_EQ,
    BKCND_NEXT_OP_EQ,

} breakpoint_cond_t;

void setBreakpoint(breakpoint_cond_t cond, uint32_t value);

void clearBreakpoint();

bool atBreakpoint();

void debugPrompt();

#endif // DEBUG_H