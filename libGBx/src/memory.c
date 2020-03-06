#include "memory.h"

#include "apu.h"
#include "bootstrap.h"
#include "cartridge.h"
#include "clock.h"
#include "interrupt.h"
#include "io.h"
#include "lcd.h"
#include "log.h"
#include "timer.h"

uint8_t WRAM0[0x7FF8];
uint8_t * WRAM = WRAM0 + 0x0FFF; // Default to Bank 1

uint8_t HRAM[127];

uint8_t readByte(uint16_t address)
{
    LogVerbose(4, "Read %02X", address);

    if (address <= 0x00FF) {
        if (BootstrapEnable) {
            return Bootstrap[address];
        } else {
            // Jump Vectors
            return ROM0[address];
        }
    }
    else if (address <= 0x3FFF) {
        // Cartridge ROM Bank 0
        return ROM0[address];
    }
    else if (address <= 0x7FFF) {
        // Switchable Cartridge ROM
        return ROM[address - 0x4000];
    }
    else if (address <= 0x9FFF) {
        return VRAM0[address - 0x8000];
    }
    else if (address <= 0xBFFF) {
        // External Cartridge RAM
        if (SRAMEnabled) {
            return SRAM[address - 0xA000];
        }
        else {
            LogFatal("Cartridge RAM Not Enabled %04X", address);
        }
    }
    else if (address <= 0xCFFF) {
        // Work RAM Bank 0
        return WRAM0[address - 0xC000];
    }
    else if (address <= 0xDFFF) {
        // Switchable Work RAM
        return WRAM[address - 0xD000];
    }
    else if (address <= 0xFDFF) {
        // Echo RAM
        return WRAM0[address - 0xE000];
    }
    else if (address <= 0xFE9F) {
        // Object Attribute Memory
        return OAM[address - 0xFE00];
    }
    else if (address <= 0xFEFF) {
        // Unusable
    }
    else if (address <= 0xFF7F) {
        // Hardware I/O Registers
        if (address >= 0xFF30 && address <= 0xFF3F) {
            return WaveRAM[address - 0xFF30];
        }

        switch (address) {
        case 0xFF00:
            // return P1.raw;
        case 0xFF01:
            // return SB;
        case 0xFF02:
            // return SC.raw;
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
            return Tone1.raw[0];
        case 0xFF11:
            return Tone1.raw[1];
        case 0xFF12:
            return Tone1.raw[2];
        case 0xFF13:
            return Tone1.raw[3];
        case 0xFF14:
            return Tone1.raw[4];
        case 0xFF16:
            return Tone2.raw[1];
        case 0xFF17:
            return Tone2.raw[2];
        case 0xFF18:
            return Tone2.raw[3];
        case 0xFF19:
            return Tone2.raw[4];
        case 0xFF20:
            return Noise.raw[0];
        case 0xFF21:
            return Noise.raw[1];
        case 0xFF22:
            return Noise.raw[2];
        case 0xFF23:
            return Noise.raw[3];
        case 0xFF24:
            return VolumeControl.raw;
        case 0xFF25:
            // return NR51;
        case 0xFF26:
            // return NR52;
        case 0xFF1A:
            return Wave.raw[0];
        case 0xFF1B:
            return Wave.raw[1];
        case 0xFF1C:
            return Wave.raw[2];
        case 0xFF1D:
            return Wave.raw[3];
        case 0xFF1E:
            return Wave.raw[4];
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
            break;
        case 0xFF4B:
            return WY;
            break;

        case 0xFF50:
            return BootstrapEnable;
        };
    }
    else if (address <= 0xFFFE) {
        return HRAM[address - 0xFF80];
    }
    else if (address == 0xFFFF) {
        // Interrupt Enable Flag
        return IE.raw;
    }

    return 0;
}

uint16_t readWord(uint16_t address) 
{
    uint16_t word = readByte(address + 1);
    word <<= 8;
    word |= readByte(address);
    return word;
}

uint8_t nextByte()
{
    uint8_t byte = readByte(R.PC);
    ++R.PC;
    return byte;
}

uint16_t nextWord()
{
    uint16_t word = readWord(R.PC);
    R.PC += 2;
    return word;
}

void writeByte(uint16_t address, uint8_t data)
{
    LogVerbose(4, "Write %02X to %02X", data, address);

    if (address <= 0x7FFF) {
        writeCartridgeMBC(address, data);
    }
    else if (address <= 0x9FFF) {
        VRAM[address - 0x8000] = data;
    }
    else if (address <= 0xBFFF) {
        // Cartridge RAM
        if (SRAMEnabled) {
            SRAM[address - 0xA000] = data;
        }
        else {
            LogFatal("Cartridge RAM Not Enabled %04X", address);
        }
    }
    else if (address <= 0xCFFF) {
        // Work RAM Bank 0
        WRAM0[address - 0xC000] = data;
    }
    else if (address <= 0xDFFF) {
        // Switchable Work RAM Bank
        WRAM[address - 0xD000] = data;
    }
    else if (address <= 0xFDFF) {
        // Echo RAM
        WRAM0[address - 0xE000] = data;
    }
    else if (address <= 0xFE9F) {
        // Object Attribute Memory
        OAM[address - 0xFE00] = data;
    }
    else if (address <= 0xFEFF) {
        // Unusable
    }
    else if (address <= 0xFF7F) {
        // Hardware I/O Registers
        if (address >= 0xFF30 && address <= 0xFF3F) {
            WaveRAM[address - 0xFF30] = data;
        }

        switch (address) {
        case 0xFF00:
            // P1.raw = data;
            break;
        case 0xFF01:
            // SB = data;
            break;
        case 0xFF02:
            // SC.raw = data;
            break;
        case 0xFF04:
            DIV = 0;
            break;
        case 0xFF05:
            TIMA = data;
            if (VerboseLevel >= 2) {
                printTimer();
            }
            break;
        case 0xFF06:
            TMA = data;
            if (VerboseLevel >= 2) {
                printTimer();
            }
            break;
        case 0xFF07:
            TAC.raw = data;
            if (VerboseLevel >= 2) {
                printTimer();
            }
            break;
        case 0xFF0F:
            IF.raw = data;
            if (VerboseLevel >= 2) {
                printIF();
            }
            break;
        case 0xFF10:
            Tone1.raw[0] = data;
            if (VerboseLevel >= 2) {
                printTone1();
            }
            break;
        case 0xFF11:
            Tone1.raw[1] = data;
            if (VerboseLevel >= 2) {
                printTone1();
            }
            break;
        case 0xFF12:
            Tone1.raw[2] = data;
            if (VerboseLevel >= 2) {
                printTone1();
            }
            break;
        case 0xFF13:
            Tone1.raw[3] = data;
            if (VerboseLevel >= 2) {
                printTone1();
            }
            break;
        case 0xFF14:
            Tone1.raw[4] = data;
            if (VerboseLevel >= 2) {
                printTone1();
            }
            break;
        case 0xFF15:
            LogWarn("Attempting to access Tone2's Sweep Controls");
            break;
        case 0xFF16:
            Tone2.raw[1] = data;
            if (VerboseLevel >= 2) {
                printTone2();
            }
            break;
        case 0xFF17:
            Tone2.raw[2] = data;
            if (VerboseLevel >= 2) {
                printTone2();
            }
            break;
        case 0xFF18:
            Tone2.raw[3] = data;
            if (VerboseLevel >= 2) {
                printTone2();
            }
            break;
        case 0xFF19:
            Tone2.raw[4] = data;
            if (VerboseLevel >= 2) {
                printTone2();
            }
            break;
        case 0xFF20:
            Noise.raw[0] = data;
            if (VerboseLevel >= 2) {
                printNoise();
            }
            break;
        case 0xFF21:
            Noise.raw[1] = data;
            if (VerboseLevel >= 2) {
                printNoise();
            }
            break;
        case 0xFF22:
            Noise.raw[2] = data;
            if (VerboseLevel >= 2) {
                printNoise();
            }
            break;
        case 0xFF23:
            Noise.raw[3] = data;
            if (VerboseLevel >= 2) {
                printNoise();
            }
            break;
        case 0xFF24:
            VolumeControl.raw = data;
            if (VerboseLevel >= 2) {
                printVolumeControl();
            }
            break;
        case 0xFF25:
            // NR51 = data;
            break;
        case 0xFF26:
            // NR52 = data;
            break;
        case 0xFF1A:
            Wave.raw[0] = data;
            if (VerboseLevel >= 2) {
                printWave();
            }
            break;
        case 0xFF1B:
            Wave.raw[1] = data;
            if (VerboseLevel >= 2) {
                printWave();
            }
            break;
        case 0xFF1C:
            Wave.raw[2] = data;
            if (VerboseLevel >= 2) {
                printWave();
            }
            break;
        case 0xFF1D:
            Wave.raw[3] = data;
            if (VerboseLevel >= 2) {
                printWave();
            }
            break;
        case 0xFF1E:
            Wave.raw[4] = data;
            if (VerboseLevel >= 2) {
                printWave();
            }
            break;
        
        case 0xFF40:
            LCDC.raw = data;
            if (VerboseLevel >= 2) {
                printLCDC();
            }
            break;
        case 0xFF41:
            STAT.raw = data;
            if (VerboseLevel >= 2) {
                printSTAT();
            }
            break;
        case 0xFF42:
            SCY = data;
            if (VerboseLevel >= 2) {
                printLCDInfo();
            }
            break;
        case 0xFF43:
            SCX = data;
            if (VerboseLevel >= 2) {
                printLCDInfo();
            }
            break;
        case 0xFF45:
            LYC = data;
            if (VerboseLevel >= 2) {
                printLCDInfo();
            }
            break;
        case 0xFF46:
            {
                uint16_t addr = data << 8;
                for (unsigned i = 0; i < 0xA0; ++i) {
                    writeByte(0xFE00 + i, readByte(addr + 1));
                    tick(4);
                }
            }
            break;
        case 0xFF47:
            BGP.raw = data;
            if (VerboseLevel >= 2) {
                printBGP();
            }
            break;
        case 0xFF48:
            OBP0.raw = data;
            if (VerboseLevel >= 2) {
                printOBP0();
            }
            break;
        case 0xFF49:
            OBP1.raw = data;
            if (VerboseLevel >= 2) {
                printOBP1();
            }
            break;
        case 0xFF4A:
            WX = data;
            if (VerboseLevel >= 2) {
                printLCDInfo();
            }
            break;
        case 0xFF4B:
            WY = data;
            if (VerboseLevel >= 2) {
                printLCDInfo();
            }
            break;
        
        case 0xFF50:
            BootstrapEnable = (data == 0);
            LogVerbose(2, "Bootstrap %s", (BootstrapEnable ? "Enabled" : "Disabled"));
            break;
        };
    }
    else if (address <= 0xFFFE) {
        HRAM[address - 0xFF80] = data;
    }
    else if (address == 0xFFFF) {
        // Interrupt Enable Flag
        IE.raw = data;
        if (VerboseLevel >= 2) {
            printIE();
        }
    }
}

void writeWord(uint16_t address, uint16_t data) 
{
    writeByte(address + 1, (uint8_t)(data >> 8));
    writeByte(address, (uint8_t)(data & 0xFF));
}

void pushByte(uint8_t data)
{
    --R.SP;
    writeByte(R.SP, data);
}

void pushWord(uint16_t data)
{
    R.SP -= 2;
    writeWord(R.SP, data);
}

uint8_t popByte()
{
    uint8_t byte = readByte(R.SP);
    ++R.SP;
    return byte;
}

uint16_t popWord()
{
    uint16_t word = readWord(R.SP);
    R.SP += 2;
    return word;
}
