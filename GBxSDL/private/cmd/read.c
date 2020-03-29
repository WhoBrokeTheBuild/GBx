#include "read.h"
#include "help.h"

#include <GBx/GBx.h>

#include <stdio.h>
#include <string.h>

void cmdRead(const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", CMD_HELP_READ);
        return;
    }

    int size;
    word addr;
    sscanf(input, "%d %hX", &size, &addr);

    if (size == 1) {
        byte value = ReadByte(addr);
        LogInfo("(%04X) = %02X", addr, value);
    }
    else if (size == 2) {
        word value = ReadWord(addr);
        LogInfo("(%04X) = %04X", addr, value);
    }
    else {
        LogError("Unsupported size '%d'", size);
    }
}