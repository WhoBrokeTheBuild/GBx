#include "read.h"
#include "help.h"

#include <stdio.h>
#include <string.h>

void cmdRead(gbx_t * ctx, const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", CMD_HELP_READ);
        return;
    }

    int size;
    uint16_t addr;
    sscanf(input, "%d %hX", &size, &addr);

    if (size == 1) {
        uint8_t value = GBx_ReadByte(ctx, addr);
        printf("(%04X) = %02X\n", addr, value);
    }
    else if (size == 2) {
        uint16_t value = GBx_ReadWord(ctx, addr);
        printf("(%04X) = %04X\n", addr, value);
    }
    else {
        fprintf(stderr, "Unsupported size '%d'", size);
    }
}