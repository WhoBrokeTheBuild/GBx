#include "memory.h"

#include "apu.h"
#include "bootstrap.h"
#include "cartridge.h"
#include "clock.h"
#include "input.h"
#include "interrupt.h"
#include "lcd.h"
#include "log.h"
#include "serial.h"
#include "timer.h"

#include <string.h>

#define WRAM_BANK_COUNT (8)
#define WRAM_BANK_SIZE  (0x1000)

byte WRAM[WRAM_BANK_COUNT][WRAM_BANK_SIZE];
uint WRAMBank;

byte HRAM[0x7F];

void ResetMemory()
{
    WRAMBank = 1;

    for (unsigned i = 0; i < WRAM_BANK_COUNT; ++i) {
        memset(WRAM[i], 0, sizeof(WRAM[i]));
    }

    memset(HRAM, 0, sizeof(HRAM));
}

byte ReadByte(word address)
{
    // LogVerbose(4, "Read Memory at %04X", address);

    // BIOS / Jump Vectors - 0000-00FF
    if (address <= 0x00FF) {
        if (BootstrapROMEnabled) {
            return BootstrapROM[address];
        } else {
            return ROM[0][address];
        }
    }
    // Cartridge ROM Bank 0 - 0100-3FFF
    else if (address <= 0x3FFF) {
        return ROM[0][address];
    }
    // Switchable Cartridge ROM - 4000-7FFF
    else if (address <= 0x7FFF) {
        return ROM[ROMBank][address - 0x4000];
    }
    // Video RAM - 8000-9FFF
    else if (address <= 0x9FFF) {
        // if (STAT.Mode != STAT_MODE_DATA_TRANSFER) {
            return VRAM[VRAMBank][address - 0x8000];
        // }
        // else {
        //     // LogWarn("Attempting to read from VRAM while in mode %d", STAT.Mode);
        //     return 0xFF;
        // }
    }
    // External Cartridge RAM - A000-BFFF
    else if (address <= 0xBFFF) {
        // if (SRAMEnabled) {
            return SRAM[SRAMBank][address - 0xA000];
        // }
        // else {
        //     LogFatal("Cartridge RAM Not Enabled %04X", address);
        // }
    }
    // Work RAM Bank 0 - C000-CFFF
    else if (address <= 0xCFFF) {
        return WRAM[0][address - 0xC000];
    }
    // Switchable Work RAM - D000-DFFF
    else if (address <= 0xDFFF) {
        return WRAM[WRAMBank][address - 0xD000];
    }
    // Echo RAM - E000-FDFF
    else if (address <= 0xFDFF) {
        return WRAM[0][address - 0xE000];
    }
    // Object Attribute Memory - FE00-FE9F
    else if (address <= 0xFE9F) {
        // if (STAT.Mode != STAT_MODE_DATA_TRANSFER && STAT.Mode != STAT_MODE_SEARCH_SPRITE) {
            return OAM[address - 0xFE00];
        // }
        // else {
        //     // LogWarn("Attempting to read from OAM while in mode %d", STAT.Mode);
        //     return 0xFF;
        // }
    }
    // Unusable - FEA0-FEFF
    else if (address <= 0xFEFF) {
    }
    // Hardware I/O Registers - FF00-FE7F
    else if (address <= 0xFF7F) {
        if (address >= 0xFF30 && address <= 0xFF3F) {
            return WaveRAM[address - 0xFF30];
        }

        switch (address) {
        case 0xFF00:
            return JOYP.raw;
        case 0xFF01:
            return SB;
        case 0xFF02:
            return SC.raw;
        case 0xFF04:
            return DIV;
        case 0xFF05:
            return TIMA;
        case 0xFF06:
            return TMA;
        case 0xFF07:
            return TAC.raw;
        case 0xFF0F:
            return IF.raw;
        case 0xFF10:
            return Tone1.raw[0] * TONE_READ_MASK0;
        case 0xFF11:
            return Tone1.raw[1] * TONE_READ_MASK1;
        case 0xFF12:
            return Tone1.raw[2] * TONE_READ_MASK2;
        case 0xFF13:
            return Tone1.raw[3] * TONE_READ_MASK3;
        case 0xFF14:
            return Tone1.raw[4] * TONE_READ_MASK4;
        case 0xFF16:
            return Tone2.raw[1] * TONE_READ_MASK1;
        case 0xFF17:
            return Tone2.raw[2] * TONE_READ_MASK2;
        case 0xFF18:
            return Tone2.raw[3] * TONE_READ_MASK3;
        case 0xFF19:
            return Tone2.raw[4] * TONE_READ_MASK4;
        case 0xFF20:
            return Noise.raw[0] * NOISE_READ_MASK0;
        case 0xFF21:
            return Noise.raw[1] * NOISE_READ_MASK1;
        case 0xFF22:
            return Noise.raw[2] * NOISE_READ_MASK2;
        case 0xFF23:
            return Noise.raw[3] * NOISE_READ_MASK3;
        case 0xFF24:
            return VolumeControl.raw;
        case 0xFF25:
            // return NR51;
        case 0xFF26:
            // return NR52;
        case 0xFF1A:
            return Wave.raw[0] * WAVE_READ_MASK0;
        case 0xFF1B:
            return Wave.raw[1] * WAVE_READ_MASK1;
        case 0xFF1C:
            return Wave.raw[2] * WAVE_READ_MASK2;
        case 0xFF1D:
            return Wave.raw[3] * WAVE_READ_MASK3;
        case 0xFF1E:
            return Wave.raw[4] * WAVE_READ_MASK4;
        case 0xFF40:
            return LCDC.raw;
        case 0xFF41:
            return STAT.raw;
        case 0xFF42:
            return SCY;
        case 0xFF43:
            return SCX;
        case 0xFF44:
            return LY;
        case 0xFF45:
            return LYC;
        case 0xFF47:
            return BGP.raw;
        case 0xFF48:
            return OBP0.raw;
        case 0xFF49:
            return OBP1.raw;
        case 0xFF4A:
            return WX;
        case 0xFF4B:
            return WY;
        };
    }
    // High RAM - FF80-FFFE
    else if (address <= 0xFFFE) {
        return HRAM[address - 0xFF80];
    }
    // Interrupt Enable Flag - FFFF
    else if (address == 0xFFFF) {
        return IE.raw;
    }

    return 0;
}

word ReadWord(word address) 
{
    word word = ReadByte(address + 1);
    word <<= 8;
    word |= ReadByte(address);
    return word;
}

byte NextByte()
{
    byte byte = ReadByte(R.PC);
    ++R.PC;
    return byte;
}

word NextWord()
{
    word word = ReadWord(R.PC);
    R.PC += 2;
    return word;
}

void WriteByte(word address, byte data)
{
    // LogVerbose(4, "Write %02X to Memory at %04X", data, address);

    // Cartridge Memory Bank Controller - 0000-7FFF
    if (address <= 0x7FFF) {
        WriteCartridgeMBC(address, data);
    }
    // Video RAM - 8000-9FFF
    else if (address <= 0x9FFF) {
        // if (STAT.Mode != STAT_MODE_DATA_TRANSFER) {
            VRAM[VRAMBank][address - 0x8000] = data;
        // }
        // else {
        //     LogWarn("Attempting to write to VRAM while in mode %d", STAT.Mode);
        // }
    }
    // External Cartridge RAM - A000-BFFF
    else if (address <= 0xBFFF) {
        // if (SRAMEnabled) {
            SRAM[SRAMBank][address - 0xA000] = data;
        // }
        // else {
        //     LogFatal("Cartridge RAM Not Enabled %04X", address);
        // }
    }
    // Work RAM Bank 0 - C000-CFFF
    else if (address <= 0xCFFF) {
        WRAM[0][address - 0xC000] = data;
    }
    // Switchable Work RAM - D000-DFFF
    else if (address <= 0xDFFF) {
        WRAM[WRAMBank][address - 0xD000] = data;
    }
    // Echo RAM - E000-FDFF
    else if (address <= 0xFDFF) {
        WRAM[0][address - 0xE000] = data;
    }
    // Object Attribute Memory - FE00-FE9F
    else if (address <= 0xFE9F) {
        // if (STAT.Mode != STAT_MODE_DATA_TRANSFER && STAT.Mode != STAT_MODE_SEARCH_SPRITE) {
           OAM[address - 0xFE00] = data;
        // }
        // else {
        //    LogWarn("Attempting to write to OAM while in mode %d", STAT.Mode);
        // }
    }
    // Unusable - FEA0-FEFF
    else if (address <= 0xFEFF) {
    }
    // Hardware I/O Registers - FF00-FE7F
    else if (address <= 0xFF7F) {
        if (address >= 0xFF30 && address <= 0xFF3F) {
            WaveRAM[address - 0xFF30] = data;
        }

        switch (address) {
        case 0xFF00:
            JOYP.raw ^= JOYP_WRITE_MASK;
            JOYP.raw |= data & JOYP_WRITE_MASK;
            UpdateInput();
            if (VerboseLevel >= 2) {
                PrintInput();
            }
            break;
        case 0xFF01:
            SB = data;
            break;
        case 0xFF02:
            SC.raw = data;
            break;
        case 0xFF04:
            DIV = 0;
            LogVerbose(2, "Resetting DIV");
            break;
        case 0xFF05:
            TIMA = data;
            if (VerboseLevel >= 2) {
                PrintTimer();
            }
            break;
        case 0xFF06:
            TMA = data;
            if (VerboseLevel >= 2) {
                PrintTimer();
            }
            break;
        case 0xFF07:
            TAC.raw = data;
            if (VerboseLevel >= 2) {
                PrintTimer();
            }
            break;
        case 0xFF0F:
            IF.raw = data;
            if (VerboseLevel >= 2) {
                PrintIF();
            }
            break;
        case 0xFF10:
            Tone1.raw[0] = data;
            if (VerboseLevel >= 2) {
                PrintTone1();
            }
            break;
        case 0xFF11:
            Tone1.raw[1] = data;
            if (VerboseLevel >= 2) {
                PrintTone1();
            }
            break;
        case 0xFF12:
            Tone1.raw[2] = data;
            if (VerboseLevel >= 2) {
                PrintTone1();
            }
            break;
        case 0xFF13:
            Tone1.raw[3] = data;
            if (VerboseLevel >= 2) {
                PrintTone1();
            }
            break;
        case 0xFF14:
            Tone1.raw[4] = data;
            if (VerboseLevel >= 2) {
                PrintTone1();
            }
            break;
        case 0xFF15:
            LogWarn("Attempting to access Tone2's Sweep Controls");
            break;
        case 0xFF16:
            Tone2.raw[1] = data;
            if (VerboseLevel >= 2) {
                PrintTone2();
            }
            break;
        case 0xFF17:
            Tone2.raw[2] = data;
            if (VerboseLevel >= 2) {
                PrintTone2();
            }
            break;
        case 0xFF18:
            Tone2.raw[3] = data;
            if (VerboseLevel >= 2) {
                PrintTone2();
            }
            break;
        case 0xFF19:
            Tone2.raw[4] = data;
            if (VerboseLevel >= 2) {
                PrintTone2();
            }
            break;
        case 0xFF20:
            Noise.raw[0] = data;
            if (VerboseLevel >= 2) {
                PrintNoise();
            }
            break;
        case 0xFF21:
            Noise.raw[1] = data;
            if (VerboseLevel >= 2) {
                PrintNoise();
            }
            break;
        case 0xFF22:
            Noise.raw[2] = data;
            if (VerboseLevel >= 2) {
                PrintNoise();
            }
            break;
        case 0xFF23:
            Noise.raw[3] = data;
            if (VerboseLevel >= 2) {
                PrintNoise();
            }
            break;
        case 0xFF24:
            VolumeControl.raw = data;
            if (VerboseLevel >= 2) {
                PrintVolumeControl();
            }
            break;
        case 0xFF25:
            SoundOutputTerminal.raw = data;
            if (VerboseLevel >= 2) {
                // PrintSoundOutputTerminal();
            }
            break;
        case 0xFF26:
            APUC.raw ^= SOUND_CONTROL_WRITE_MASK;
            APUC.raw |= data & SOUND_CONTROL_WRITE_MASK;
            if (!APUC.Enabled) {
                // TODO: Disable all sound
            }
            if (VerboseLevel >= 2) {
                // PrintAPUC();
            }
            break;
        case 0xFF1A:
            Wave.raw[0] = data;
            if (VerboseLevel >= 2) {
                PrintWave();
            }
            break;
        case 0xFF1B:
            Wave.raw[1] = data;
            if (VerboseLevel >= 2) {
                PrintWave();
            }
            break;
        case 0xFF1C:
            Wave.raw[2] = data;
            if (VerboseLevel >= 2) {
                PrintWave();
            }
            break;
        case 0xFF1D:
            Wave.raw[3] = data;
            if (VerboseLevel >= 2) {
                PrintWave();
            }
            break;
        case 0xFF1E:
            Wave.raw[4] = data;
            if (VerboseLevel >= 2) {
                PrintWave();
            }
            break;
        
        case 0xFF40:
            LCDC.raw = data;
            if (VerboseLevel >= 2) {
                PrintLCDC();
            }
            break;
        case 0xFF41:
            STAT.raw ^= STAT_WRITE_MASK;
            STAT.raw |= data & STAT_WRITE_MASK;
            if (VerboseLevel >= 2) {
                PrintSTAT();
            }
            break;
        case 0xFF42:
            SCY = data;
            LogVerbose(2, "Setting SCY=%02X", data);
            break;
        case 0xFF43:
            SCX = data;
            LogVerbose(2, "Setting SCX=%02X", data);
            break;
        case 0xFF45:
            LYC = data;
            LogVerbose(2, "Setting LYC=%02X", data);
            break;
        case 0xFF46:
            {
                CPUEnabled = false;

                word addr = data << 8;
                for (unsigned i = 0; i < 0xA0; ++i) {
                    OAM[i] = ReadByte(addr + i);
                    Tick(4);
                }

                CPUEnabled = true;
            }
            break;
        case 0xFF47:
            BGP.raw = data;
            if (VerboseLevel >= 2) {
                PrintPalettes();
            }
            break;
        case 0xFF48:
            OBP0.raw = data;
            if (VerboseLevel >= 2) {
                PrintLCDCoordinates();
            }
            break;
        case 0xFF49:
            OBP1.raw = data;
            if (VerboseLevel >= 2) {
                PrintLCDCoordinates();
            }
            break;
        case 0xFF4A:
            WX = data;
            LogVerbose(2, "Setting WX=%02X", data);
            break;
        case 0xFF4B:
            WY = data;
            LogVerbose(2, "Setting WY=%02X", data);
            break;
        
        case 0xFF50:
            BootstrapROMEnabled = false;
            LogVerbose(2, "Bootstrap ROM Disabled");
            break;
        };
    }
    // High RAM - FF80-FFFE
    else if (address <= 0xFFFE) {
        HRAM[address - 0xFF80] = data;
    }
    // Interrupt Enable Flag - FFFF
    else if (address == 0xFFFF) {
        IE.raw = data;
        if (VerboseLevel >= 2) {
            PrintIE();
        }
    }
}

void WriteWord(word address, word data) 
{
    WriteByte(address + 1, (byte)(data >> 8));
    WriteByte(address, (byte)(data & 0xFF));
}

void PushByte(byte data)
{
    --R.SP;
    WriteByte(R.SP, data);
}

void PushWord(word data)
{
    R.SP -= 2;
    WriteWord(R.SP, data);
}

byte PopByte()
{
    byte data = ReadByte(R.SP);
    ++R.SP;
    return data;
}

word PopWord()
{
    word data = ReadWord(R.SP);
    R.SP += 2;
    return data;
}
