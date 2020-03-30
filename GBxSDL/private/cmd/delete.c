#include "delete.h"
#include "../breakpoint.h"

#include <GBx/Log.h>

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

        unsigned value;
        sscanf(equal + 1, "%04X", &value);

        ClearBreakpoint(input, value);
        LogInfo("Breakpoint %s=$%04X deleted", input, value);
    }
    else {
        unsigned pc;
        sscanf(input, "%04X", &pc);
        ClearBreakpoint("PC", pc);
        LogInfo("Breakpoint PC=$%04X deleted", pc);
    }
}