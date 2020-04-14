#include "delete.h"
#include "../breakpoint.h"

#include <stdio.h>
#include <string.h>

void cmdBreak(gbx_t * ctx, const char * input)
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

        unsigned value;
        sscanf(equal + 1, "%04X", &value);

        SetBreakpoint(input, value);
        printf("Breakpoint set when %s=$%04X\n", input, value);
    }
    if (strcmp(input, "SPCHNG") == 0) {
        SetBreakpointStackChanged(ctx);
        printf("Breakpoint set when Stack Changed");
    }
    else {
        unsigned pc;
        sscanf(input, "%04X", &pc);
        SetBreakpoint("PC", pc);
        printf("Breakpoint set when PC=$%04X\n", pc);
    }
}
