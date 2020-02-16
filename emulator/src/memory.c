#include "memory.h"

#include "bootstrap.h"
#include "cartridge.h"
#include "interrupt.h"
#include "io.h"
#include "log.h"
#include "register.h"
#include "sound.h"
#include "timer.h"
#include "video.h"

uint8_t InternalRAM0[0x7FF8];
uint8_t * InternalRAM = InternalRAM0 + 0x0FFF; // Default to Bank 1

uint8_t ZeroPage[127];

uint8_t readByte(uint16_t address)
{
    // LogVerbose("read %02X", address);

    if (address <= 0x00FF) {
        if (BootstrapEnable) {
            return BOOTSTRAP[address];
        } else {
            // Jump Vectors
            return CartridgeROM0[address];
        }
    }
    else if (address <= 0x3FFF) {
        // Cartridge ROM Bank 0
        return CartridgeROM0[address];
    }
    else if (address <= 0x7FFF) {
        // Cartridge ROM Switchable Bank
        return CartridgeROM[address - 0x4000];
    }
    else if (address <= 0x9FFF) {
        return VideoRAM0[address - 0x8000];
    }
    else if (address <= 0xBFFF) {
        // Cartridge RAM
        if (CartridgeRAMEnabled) {
            return CartridgeRAM[address - 0xA000];
        }
        else {
            LogFatal("Cartridge RAM Not Enabled %04X", address);
        }
    }
    else if (address <= 0xCFFF) {
        // Internal RAM Bank 0
        return InternalRAM0[address - 0xC000];
    }
    else if (address <= 0xDFFF) {
        // Internal RAM Switchable Bank
        return InternalRAM[address - 0xD000];
    }
    else if (address <= 0xFDFF) {
        // Echo RAM
        return InternalRAM0[address - 0xE000];
    }
    else if (address <= 0xFE9F) {
        // Object Attribute Memory OAM
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
            return P1.data;
        case 0xFF01:
            return SB;
        case 0xFF02:
            return SC.data;
        case 0xFF04:
            return DIV;
        case 0xFF05:
            return TIMA;
        case 0xFF06:
            return TMA;
        case 0xFF07:
            return TAC.data;
        case 0xFF0F:
            if (DebugMode) {
                printIF();
            }
            return IF.data;
        case 0xFF10:
            return NR10;
        case 0xFF11:
            return NR11;
        case 0xFF12:
            return NR12;
        case 0xFF13:
            return NR13;
        case 0xFF14:
            return NR14;
        case 0xFF16:
            return NR21;
        case 0xFF17:
            return NR22;
        case 0xFF18:
            return NR23;
        case 0xFF19:
            return NR24;
        case 0xFF1A:
            return NR30;
        case 0xFF1B:
            return NR31;
        case 0xFF1C:
            return NR32;
        case 0xFF1D:
            return NR33;
        case 0xFF1E:
            return NR34;
        case 0xFF20:
            return NR41;
        case 0xFF21:
            return NR42;
        case 0xFF22:
            return NR43;
        case 0xFF23:
            return NR44;
        case 0xFF24:
            return NR50;
        case 0xFF25:
            return NR51;
        case 0xFF26:
            return NR52;

        case 0xFF40:
            if (DebugMode) {
                printLCDC();
            }
            return LCDC.data;
        case 0xFF41:
            return STAT.data;
        case 0xFF42:
            return SCY;
        case 0xFF43:
            return SCX;
        case 0xFF44:
            return LY;
        case 0xFF45:
            return LYC;
        case 0xFF47:
            return BGP.data;
        case 0xFF48:
            return OBP0.data;
        case 0xFF49:
            return OBP1.data;
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
        return ZeroPage[address - 0xFF80];
    }
    else if (address == 0xFFFF) {
        // Interrupt Enable Flag
        if (DebugMode) {
            printIE();
        }
        return IE.data;
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
    // LogVerbose("write %02X", address);

    if (address <= 0x7FFF) {
        writeCartridgeMBC(address, data);
    }
    else if (address <= 0x9FFF) {
        VideoRAM0[address - 0x8000] = data;
    }
    else if (address <= 0xBFFF) {
        // Cartridge RAM
        if (CartridgeRAMEnabled) {
            CartridgeRAM[address - 0xA000] = data;
        }
        else {
            LogFatal("Cartridge RAM Not Enabled %04X", address);
        }
    }
    else if (address <= 0xCFFF) {
        // Internal RAM Bank 0
        InternalRAM0[address - 0xC000] = data;
    }
    else if (address <= 0xDFFF) {
        // Internal RAM Switchable Bank
        InternalRAM[address - 0xD000] = data;
    }
    else if (address <= 0xFDFF) {
        // Echo RAM
        InternalRAM0[address - 0xE000] = data;
    }
    else if (address <= 0xFE9F) {
        // Object Attribute Memory OAM
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
            P1.data = data;
            break;
        case 0xFF01:
            SB = data;
            break;
        case 0xFF02:
            SC.data = data;
            break;
        case 0xFF04:
            DIV = 0;
            break;
        case 0xFF05:
            TIMA = data;
            break;
        case 0xFF06:
            TMA = data;
            break;
        case 0xFF07:
            TAC.data = data;
            break;
        case 0xFF0F:
            IF.data = data;
            if (DebugMode) {
                LogInfo("%02X", data);
                printIF();
            }
            break;
        case 0xFF10:
            NR10 = data;
            break;
        case 0xFF11:
            NR11 = data;
            break;
        case 0xFF12:
            NR12 = data;
            break;
        case 0xFF13:
            NR13 = data;
            break;
        case 0xFF14:
            NR14 = data;
            break;
        case 0xFF16:
            NR21 = data;
            break;
        case 0xFF17:
            NR22 = data;
            break;
        case 0xFF18:
            NR23 = data;
            break;
        case 0xFF19:
            NR24 = data;
            break;
        case 0xFF1A:
            NR30 = data;
            break;
        case 0xFF1B:
            NR31 = data;
            break;
        case 0xFF1C:
            NR32 = data;
            break;
        case 0xFF1D:
            NR33 = data;
            break;
        case 0xFF1E:
            NR34 = data;
            break;
        case 0xFF20:
            NR41 = data;
            break;
        case 0xFF21:
            NR42 = data;
            break;
        case 0xFF22:
            NR43 = data;
            break;
        case 0xFF23:
            NR44 = data;
            break;
        case 0xFF24:
            NR50 = data;
            break;
        case 0xFF25:
            NR51 = data;
            break;
        case 0xFF26:
            NR52 = data;
            break;
        
        case 0xFF40:
            LCDC.data = data;
            if (DebugMode) {
                printLCDC();
            }
            break;
        case 0xFF41:
            STAT.data = data;
            if (DebugMode) {
                printSTAT();
            }
            break;
        case 0xFF42:
            SCY = data;
            if (DebugMode) {
                printLCDInfo();
            }
            break;
        case 0xFF43:
            SCX = data;
            if (DebugMode) {
                printLCDInfo();
            }
            break;
        case 0xFF45:
            LYC = data;
            if (DebugMode) {
                printLCDInfo();
            }
            break;
        case 0xFF46:
            {
                uint16_t addr = data << 8;
                for (unsigned i = 0; i < 0xA0; ++i) {
                    writeByte(0xFE00 + i, readByte(addr + 1));
                }
            }
            break;
        case 0xFF47:
            BGP.data = data;
            break;
        case 0xFF48:
            OBP0.data = data;
            break;
        case 0xFF49:
            OBP1.data = data;
            break;
        case 0xFF4A:
            WX = data;
            if (DebugMode) {
                printLCDInfo();
            }
            break;
        case 0xFF4B:
            WY = data;
            if (DebugMode) {
                printLCDInfo();
            }
            break;
        
        case 0xFF50:
            BootstrapEnable = (data > 0);
            break;
        };
    }
    else if (address <= 0xFFFE) {
        ZeroPage[address - 0xFF80] = data;
    }
    else if (address == 0xFFFF) {
        // Interrupt Enable Flag
        IE.data = data;
        if (DebugMode) {
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