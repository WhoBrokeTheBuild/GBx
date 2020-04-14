#include "info.h"
#include "help.h"

#include <stdio.h>
#include <string.h>

void cmdInfo(gbx_t * ctx, const char * input)
{
    if (!input) {
        printf("%s", CMD_HELP_INFO);
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        printf("%s", CMD_HELP_INFO);
        return;
    }

    if (strncmp(input, "all", length) == 0) {
        SM83_PrintRegisters(ctx->CPU);
        GBx_PrintTone1(ctx);
        GBx_PrintTone2(ctx);
        GBx_PrintWave(ctx);
        GBx_PrintWaveRAM(ctx);
        GBx_PrintVolumeControl(ctx);
        SM83_PrintInterrupts(ctx->CPU);
        GBx_PrintLCDC(ctx);
        GBx_PrintSTAT(ctx);
        GBx_PrintLCDCoordinates(ctx);
        GBx_PrintPalettes(ctx);
        GBx_PrintTimer(ctx);
        GBx_PrintMBC(ctx);
        GBx_PrintCartridge(ctx);
        GBx_PrintJoypad(ctx);
    }
    else if (strncmp(input, "apu", length) == 0) {
        GBx_PrintTone1(ctx);
        GBx_PrintTone2(ctx);
        GBx_PrintWave(ctx);
        GBx_PrintWaveRAM(ctx);
        GBx_PrintVolumeControl(ctx);
    }
    else if (strncmp(input, "registers", length) == 0) {
        SM83_PrintRegisters(ctx->CPU);
    }
    else if (strncmp(input, "interrupts", length) == 0) {
        SM83_PrintInterrupts(ctx->CPU);
    }
    else if (strncmp(input, "lcd", length) == 0) {
        GBx_PrintLCDC(ctx);
        GBx_PrintSTAT(ctx);
        GBx_PrintLCDCoordinates(ctx);
        GBx_PrintPalettes(ctx);
    }
    else if (strncmp(input, "timer", length) == 0) {
        GBx_PrintTimer(ctx);
    }
    else if (strncmp(input, "mbc", length) == 0) {
        GBx_PrintMBC(ctx);
    }
    else if (strncmp(input, "cartridge", length) == 0) {
        GBx_PrintCartridge(ctx);
    }
    else if (strncmp(input, "joypad", length) == 0) {
        GBx_PrintJoypad(ctx);
    }
    else {
        fprintf(stderr, "Unrecognized command 'info %s'\n", input);
    }
}