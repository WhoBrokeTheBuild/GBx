#include "bios.h"

#include "apu.h"
#include "cartridge.h"
#include "cpu.h"
#include "interrupt.h"
#include "lcd.h"
#include "log.h"
#include "timer.h"

bool BIOSEnable = false;

uint8_t BIOS[0x100];

bool loadBIOS(const char * filename)
{
    LogVerbose(1, "Opening BIOS ROM: '%s'", filename);

    FILE * file = fopen(filename, "rb");

    size_t bytesRead = fread(BIOS, 1, sizeof(BIOS), file);
    fclose(file);

    if (bytesRead < sizeof(BIOS)) {
        LogFatal("BIOS ROM too small: '%s'", filename);
    }

    BIOSEnable = true;
    return true;
}

void resetDMG()
{
    IME = true;
    R.AF = 0x01B0;
    R.BC = 0x0013;
    R.DE = 0x00D8;
    R.HL = 0x014D;
    R.PC = 0x0150;
    R.SP = 0xFFFE;
    TIMA = 0x00;
    TMA = 0x00;
    TAC.raw = 0x00;
    Tone1.raw[0] = 0x80; // NR10
    Tone1.raw[1] = 0xBF; // NR11
    Tone1.raw[2] = 0xF3; // NR12
    // NR13
    Tone1.raw[4] = 0xBF; // NR14
    Tone2.raw[0] = 0x3F; // NR21
    Tone2.raw[1] = 0x00; // NR22
    Tone2.raw[3] = 0xBF; // NR24
    Wave.raw[0] = 0x7F; // NR30
    Wave.raw[1] = 0xFF; // NR31
    Wave.raw[2] = 0x9F; // NR32
    Wave.raw[3] = 0xBF; // NR33
    // NR34
    Noise.raw[0] = 0xFF; // NR41
    Noise.raw[1] = 0x00; // NR42
    Noise.raw[2] = 0x00; // NR43
    Noise.raw[3] = 0xBF; // NR44
    VolumeControl.raw = 0x77; // NR50
    // = 0xF3; // NR51
    // = 0xF1; // NR52
    LCDC.raw = 0x91;
    STAT.Mode = STAT_MODE_SEARCH_SPRITE;
    SCY = 0x00;
    SCX = 0x00;
    LYC = 0x00;
    BGP.raw = 0xFC;
    OBP0.raw = 0xFF;
    OBP1.raw = 0xFF;
    WY = 0x00;
    WX = 0x00;
    IF.raw = 0x00;
    IE.raw = 0x00;
}

void resetSGB()
{

}

void resetCGB()
{
    
}

void reset()
{
    // resetCartridgeMBC();
    
    if (SuperEnabled) {
        resetSGB();
    }
    else if (ColorEnabled) {
        resetCGB();
    }
    else {
        resetDMG();
    }
}