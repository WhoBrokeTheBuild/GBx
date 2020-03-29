#include "info.h"
#include "help.h"

#include <GBx/GBx.h>

#include <stdio.h>
#include <string.h>

void cmdInfo(const char * input)
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
        PrintRegisters();
        PrintTone1();
        PrintTone2();
        PrintWave();
        PrintWaveRAM();
        PrintVolumeControl();
        PrintIE();
        PrintIF();
        PrintLCDC();
        PrintSTAT();
        PrintLCDCoordinates();
        PrintPalette("BGP", &BGP);
        PrintPalette("OBP0", &OBP0);
        PrintPalette("OBP1", &OBP1);
        PrintTimer();
        PrintMBC();
        PrintCartridge();
        PrintJoypad();
    }
    else if (strncmp(input, "apu", length) == 0) {
        PrintTone1();
        PrintTone2();
        PrintWave();
        PrintWaveRAM();
        PrintVolumeControl();
    }
    else if (strncmp(input, "registers", length) == 0) {
        PrintRegisters();
    }
    else if (strncmp(input, "interrupts", length) == 0) {
        PrintIE();
        PrintIF();
        LogInfo("IME=%s", (IME ? "true" : "false"));
    }
    else if (strncmp(input, "lcd", length) == 0) {
        PrintLCDC();
        PrintSTAT();
        PrintLCDCoordinates();
        PrintPalette("BGP", &BGP);
        PrintPalette("OBP0", &OBP0);
        PrintPalette("OBP1", &OBP1);
    }
    else if (strncmp(input, "timer", length) == 0) {
        PrintTimer();
    }
    else if (strncmp(input, "mbc", length) == 0) {
        PrintMBC();
    }
    else if (strncmp(input, "cartridge", length) == 0) {
        PrintCartridge();
    }
    else if (strncmp(input, "joypad", length) == 0) {
        PrintJoypad();
    }
    else {
        LogError("Unrecognized command 'info %s'", input);
    }
}