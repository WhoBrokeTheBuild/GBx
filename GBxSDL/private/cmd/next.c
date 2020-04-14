#include "next.h"

#include <stdio.h>
#include <string.h>

void cmdNext(gbx_t * ctx, const char * input)
{
    if (!input || strlen(input) == 0) {
        SM83_Step(ctx->CPU);
        return;
    }

    unsigned count;
    sscanf(input, "%u", &count);

    for (int i = 0; i < count; ++i) {
        SM83_Step(ctx->CPU);
    }
}