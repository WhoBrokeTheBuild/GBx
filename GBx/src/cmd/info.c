#include "info.h"
#include "help.h"

#include <GBx/apu.h>
#include <GBx/cartridge.h>
#include <GBx/input.h>
#include <GBx/interrupt.h>
#include <GBx/lcd.h>
#include <GBx/log.h>
#include <GBx/timer.h>

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
        PrintR();
        PrintTone1();
        PrintTone2();
        PrintWave();
        PrintWaveRAM();
        PrintVolumeControl();
        PrintIE();
        PrintIF();
        PrintLCDCoordinates();
        PrintLCDC();
        PrintSTAT();
        PrintTimer();
        PrintCartridgeMBC();
        PrintCartridge();
    }
    else if (strncmp(input, "apu", length) == 0) {
        PrintTone1();
        PrintTone2();
        PrintWave();
        PrintWaveRAM();
        PrintVolumeControl();
    }
    else if (strncmp(input, "registers", length) == 0) {
        PrintR();
    }
    else if (strncmp(input, "interrupts", length) == 0) {
        PrintIE();
        PrintIF();
        LogInfo("IME=%s", (IME ? "true" : "false"));
    }
    else if (strncmp(input, "lcd", length) == 0) {
        PrintLCDCoordinates();
        PrintLCDC();
        PrintSTAT();
    }
    else if (strncmp(input, "timer", length) == 0) {
        PrintTimer();
    }
    else if (strncmp(input, "mbc", length) == 0) {
        PrintCartridgeMBC();
    }
    else if (strncmp(input, "cartridge", length) == 0) {
        PrintCartridge();
    }
    else {
        LogWarn("Unrecognized command 'info %s'", input);
    }
}