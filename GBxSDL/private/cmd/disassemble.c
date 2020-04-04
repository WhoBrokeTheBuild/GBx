#include "disassemble.h"

#include <GBx/GBx.h>

#include <stdio.h>
#include <string.h>

void cmdDisassemble(const char * input)
{
    unsigned count;
    uint16_t addr;

    if (!input || strlen(input) == 0) {
        count = 1;
        addr = CPU.PC;
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
            addr = CPU.PC;
        }
    }

    LogInfo("Disassembling %u instructions at %04X", count, addr);

    char buffer[80];

    int tmpVerbose = VerboseLevel;
    VerboseLevel = 0;

    for (int i = 0; i < count; ++i) {
        uint16_t tmp = addr;
        addr = SM83_Disassemble(&CPU, buffer, sizeof(buffer), addr);
        printf("%04X    %s\n", tmp, buffer);
    }

    VerboseLevel = tmpVerbose;

    printf("\n");
}