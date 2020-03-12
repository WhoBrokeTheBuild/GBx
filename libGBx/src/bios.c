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

    STAT.Mode = STAT_MODE_HBLANK;
    
    BIOSEnable = true;
    return true;
}

void resetDMG()
{
    IME = true;
    IF.raw = 0x00;
    IE.raw = 0x00;

    R.AF = 0x01B0;
    R.BC = 0x0013;
    R.DE = 0x00D8;
    R.HL = 0x014D;
    R.PC = 0x0150;
    R.SP = 0xFFFE;

    TIMA = 0x00;
    TMA = 0x00;
    TAC.raw = 0x00;

    LCDC.raw = 0x91;
    STAT.Mode = STAT_MODE_HBLANK;

    SCY = 0x00;
    SCX = 0x00;
    LY = 0x00;
    LYC = 0x00;
    WY = 0x00;
    WX = 0x00;

    BGP.raw = 0b11100100;
    OBP0.raw = 0b11100100;
    OBP1.raw = 0b11100100;

    // NR10-NR14
    Tone1.raw[0] = 0x80;
    Tone1.raw[1] = 0xBF;
    Tone1.raw[2] = 0xF3;
    Tone1.raw[4] = 0xBF;

    Tone1Timer = 0;
    Tone1DutyIndex = 0;

    // NR21-NR24
    Tone2.raw[0] = 0x3F;
    Tone2.raw[1] = 0x00;
    Tone2.raw[3] = 0xBF;

    Tone2Timer = 0;
    Tone2DutyIndex = 0;

    // NR30-NR34
    Wave.raw[0] = 0x7F;
    Wave.raw[1] = 0xFF;
    Wave.raw[2] = 0x9F;
    Wave.raw[3] = 0xBF;
    Wave.raw[4] = 0x00;

    WaveTimer = 0;

    // NR41-NR44
    Noise.raw[0] = 0xFF;
    Noise.raw[1] = 0x00;
    Noise.raw[2] = 0x00;
    Noise.raw[3] = 0xBF;

    NoiseTimer = 0;

    // NR50
    VolumeControl.raw = 0x77;

    // NR51
    SoundOutputTerminal.raw = 0xF3;

    // NR52
    SoundControl.raw = 0xF1;
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