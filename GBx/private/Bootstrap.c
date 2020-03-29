#include <GBx/Bootstrap.h>

#include <GBx/APU.h>
#include <GBx/Cartridge.h>
#include <GBx/CPU.h>
#include <GBx/Joypad.h>
#include <GBx/Interrupts.h>
#include <GBx/Log.h>
#include <GBx/MMU.h>
#include <GBx/LCD.h>
#include <GBx/Timer.h>

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
