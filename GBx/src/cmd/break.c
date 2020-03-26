#include "delete.h"
#include "../breakpoint.h"

#include <GBx/log.h>

#include <stdio.h>
#include <string.h>

void cmdBreak(const char * input)
{
    if (!input) {
        PrintBreakpoints();
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        PrintBreakpoints();
        return;
    }
    
    char * equal = strchr(input, '=');
    if (equal) {
        *equal = '\0';

        unsigned int value;
        sscanf(equal + 1, "%04X", &value);

        SetBreakpoint(input, value);
        LogInfo("Breakpoint set when %s=$%04X", input, value);
    }
    else {
        unsigned int pc;
        sscanf(input, "%04X", &pc);
        SetBreakpoint("PC", pc);
        LogInfo("Breakpoint set when PC=$%04X", pc);
    }
}
