#include "delete.h"
#include "../breakpoint.h"

#include <stdio.h>
#include <string.h>

void cmdDelete(gbx_t * ctx, const char * input)
{
    if (!input) {
        ClearAllBreakpoints();
        printf("All Breakpoints deleted\n");
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        ClearAllBreakpoints();
        printf("All Breakpoints deleted\n");
        return;
    }
    
    char * equal = strchr(input, '=');
    if (equal) {
        *equal = '\0';

        unsigned value;
        sscanf(equal + 1, "%04X", &value);

        ClearBreakpoint(input, value);
        printf("Breakpoint %s=$%04X deleted\n", input, value);
    }
    else {
        unsigned pc;
        sscanf(input, "%04X", &pc);
        ClearBreakpoint("PC", pc);
        printf("Breakpoint PC=$%04X deleted\n", pc);
    }
}