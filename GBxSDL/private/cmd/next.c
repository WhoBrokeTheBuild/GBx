#include "next.h"

#include <GBx/GBx.h>

#include <stdio.h>
#include <string.h>

void cmdNext(const char * input)
{
    if (!input || strlen(input) == 0) {
        NextInstruction();
        return;
    }

    unsigned count;
    sscanf(input, "%u", &count);

    for (int i = 0; i < count; ++i) {
        NextInstruction();
    }
}