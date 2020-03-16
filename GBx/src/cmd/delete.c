#include "delete.h"
#include "../breakpoint.h"

#include <GBx/log.h>

#include <stdio.h>
#include <string.h>

void cmdDelete(const char * input)
{
    if (!input) {
        ClearAllBreakpoints();
        LogInfo("All Breakpoints deleted");
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        ClearAllBreakpoints();
        LogInfo("All Breakpoints deleted");
        return;
    }
    
    char * equal = strchr(input, '=');
    if (equal) {
        *equal = '\0';

        unsigned int value;
        sscanf(equal + 1, "%04X", &value);

        ClearBreakpoint(input, value);
        LogInfo("Breakpoint %s=%04Xh deleted", input, value);
    }
    else {
        unsigned int pc;
        sscanf(input, "%04X", &pc);
        ClearBreakpoint("PC", pc);
        LogInfo("Breakpoint PC=%04Xh deleted", pc);
    }
}