#include "bootstrap.h"

#include "apu.h"
#include "cartridge.h"
#include "clock.h"
#include "cpu.h"
#include "input.h"
#include "interrupt.h"
#include "memory.h"
#include "lcd.h"
#include "log.h"
#include "timer.h"

bool HasBootstrapROM = false;
bool BootstrapROMEnabled = false;

byte BootstrapROM[0x100];

bool LoadBootstrapROM(const char * filename)
{
    LogVerbose(1, "Opening Bootstrap ROM: '%s'", filename);

    FILE * file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    LogVerbose(1, "Bootstrap ROM Size %zu", size);

    size_t bytesRead = fread(BootstrapROM, 1, sizeof(BootstrapROM), file);
    fclose(file);

    if (bytesRead < sizeof(BootstrapROM)) {
        LogFatal("Bootstrap ROM too small: '%s'", filename);
    }

    STAT.Mode = STAT_MODE_HBLANK;
    
    BootstrapROMEnabled = true;
    HasBootstrapROM = true;
    return true;
}

void Reset()
{
    R.AF = 0x01B0;
    R.BC = 0x0013;
    R.DE = 0x00D8;
    R.HL = 0x014D;
    R.PC = (HasBootstrapROM ? 0x0000 : 0x0100);
    R.SP = 0xFFFE;
    StackBaseAddress = R.SP;
    
    ResetAPU();
    ResetLCD();
    ResetTimer();
    ResetInput();
    ResetMemory();
    ResetInterrupts();
    ResetCartridgeMBC();

    ClockSpeed = GB_CLOCK_SPEED;
    
    if (SuperEnabled) {
        ClockSpeed = SGB_CLOCK_SPEED;
    }
    else if (ColorEnabled) {
        R.A = 0x11;
        ClockSpeed = CGB_CLOCK_SPEED;
    }
}