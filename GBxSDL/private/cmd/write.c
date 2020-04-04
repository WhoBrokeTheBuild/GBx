#include "write.h"
#include "help.h"

#include <GBx/GBx.h>

#include <stdio.h>
#include <string.h>

void cmdWrite(const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", CMD_HELP_READ);
        return;
    }
    
    int size;
    uint16_t addr;
    sscanf(input, "%d %hX", &size, &addr);

    if (size == 8) {
        uint8_t value;
        sscanf(input, "%*s %*s %hhX", &value);
        LogInfo("Writing %02X to %04X", value, addr);
        WriteByte(addr, value);
    }
    else if (size == 16) {
        uint16_t value;
        sscanf(input, "%*s %*s %hX", &value);
        LogInfo("Writing %04X to %04X", value, addr);
        WriteWord(addr, value);
    }
    else {
        LogError("Unsupported size '%d'", size);
    }
}