#include "write.h"
#include "help.h"

#include <stdio.h>
#include <string.h>

void cmdWrite(gbx_t * ctx, const char * input)
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
        printf("Writing %02X to %04X\n", value, addr);
        GBx_WriteByte(ctx, addr, value);
    }
    else if (size == 16) {
        uint16_t value;
        sscanf(input, "%*s %*s %hX", &value);
        printf("Writing %04X to %04X\n", value, addr);
        GBx_WriteWord(ctx, addr, value);
    }
    else {
        fprintf(stderr, "Unsupported size '%d'\n", size);
    }
}