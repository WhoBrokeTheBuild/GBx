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
    uint16_t addr;
    sscanf(input, "%d %hX", &size, &addr);

    if (size == 1) {
        uint8_t value = ReadByte(addr);
        LogInfo("(%04X) = %02X", addr, value);
    }
    else if (size == 2) {
        uint16_t value = ReadWord(addr);
        LogInfo("(%04X) = %04X", addr, value);
    }
    else {
        LogError("Unsupported size '%d'", size);
    }
}