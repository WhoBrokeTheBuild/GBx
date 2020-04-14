#include "disassemble.h"

#include <stdio.h>
#include <string.h>

void cmdDisassemble(gbx_t * ctx, const char * input)
{
    unsigned count;
    uint16_t addr;

    if (!input || strlen(input) == 0) {
        count = 1;
        addr = ctx->CPU->PC;
    }
    else {
        char * space = strchr(input, ' ');
        if (space) {
            *space = '\0';

            sscanf(input, "%d", &count);
            sscanf(space + 1, "%4hX", &addr);
        }
        else {
            sscanf(input, "%d", &count);
            addr = ctx->CPU->PC;
        }
    }

    printf("Disassembling %u instructions at %04X\n", count, addr);

    char buffer[80];

    int tmpVerbose = ctx->VerboseLevel;
    ctx->CPU->VerboseLevel = 0;
    ctx->VerboseLevel = 0;

    for (int i = 0; i < count; ++i) {
        uint16_t tmp = addr;
        addr = SM83_Disassemble(ctx->CPU, buffer, sizeof(buffer), addr);
        printf("%04X    %s\n", tmp, buffer);
    }

    ctx->CPU->VerboseLevel = tmpVerbose;
    ctx->VerboseLevel = tmpVerbose;

    printf("\n");
}