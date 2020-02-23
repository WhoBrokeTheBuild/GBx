#include "bootstrap.h"

#include "apu.h"
#include "interrupt.h"
#include "lcd.h"
#include "register.h"
#include "timer.h"

bool BootstrapEnable = false;

uint8_t BOOTSTRAP[0x100] = {
    0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32,
    0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
    0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3,
    0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
    0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A,
    0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
    0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06,
    0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
    0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99,
    0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
    0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64,
    0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
    0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90,
    0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
    0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62,
    0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
    0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42,
    0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
    0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04,
    0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
    0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9,
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
    0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
    0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
    0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13,
    0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
    0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20,
    0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50,
};

void bootstrap()
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
    TAC.data = 0x00;
    Tone1.data[0] = 0x80; // NR10
    Tone1.data[1] = 0xBF; // NR11
    Tone1.data[2] = 0xF3; // NR12
    // NR13
    Tone1.data[4] = 0xBF; // NR14
    Tone2.data[0] = 0x3F; // NR21
    Tone2.data[1] = 0x00; // NR22
    Tone2.data[3] = 0xBF; // NR24
    Wave.data[0] = 0x7F; // NR30
    Wave.data[1] = 0xFF; // NR31
    Wave.data[2] = 0x9F; // NR32
    Wave.data[3] = 0xBF; // NR33
    // NR34
    Noise.data[0] = 0xFF; // NR41
    Noise.data[1] = 0x00; // NR42
    Noise.data[2] = 0x00; // NR43
    Noise.data[3] = 0xBF; // NR44
    VolumeControl.data = 0x77; // NR50
    // = 0xF3; // NR51
    // = 0xF1; // NR52
    LCDC.data = 0x91;
    SCY = 0x00;
    SCX = 0x00;
    LYC = 0x00;
    BGP.data = 0xFC;
    OBP0.data = 0xFF;
    OBP1.data = 0xFF;
    WY = 0x00;
    WX = 0x00;
    IF.data = 0x00;
    IE.data = 0x00;
}