#include "next.h"

#include <GBx/GBx.h>

#include <stdio.h>
#include <string.h>

void cmdNext(const char * input)
{
    if (!input || strlen(input) == 0) {
        SM83_Step(&CPU);
        return;
    }

    unsigned count;
    sscanf(input, "%u", &count);

    for (int i = 0; i < count; ++i) {
        SM83_Step(&CPU);
    }
}