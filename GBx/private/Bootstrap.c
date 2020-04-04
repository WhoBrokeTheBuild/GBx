#include <GBx/Bootstrap.h>

#include <GBx/Log.h>

bool HasBootstrapROM     = false;
bool BootstrapROMEnabled = false;

uint8_t BootstrapROM[0x100];

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

    BootstrapROMEnabled = true;
    HasBootstrapROM     = true;
    return true;
}
