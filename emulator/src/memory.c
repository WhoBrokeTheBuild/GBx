#include "memory.h"

#include "alu.h"
#include "bootstrap.h"
#include "interrupt.h"
#include "io.h"
#include "log.h"
#include "register.h"
#include "sound.h"
#include "timer.h"
#include "video.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define TITLE_OFFSET                0x0134

char Title[15];

#define COLOR_ENABLE_OFFSET         0x0143
#define SUPER_ENABLE_OFFSET         0x0146

bool ColorEnabled = false;
bool SuperEnabled = false;

#define CARTRIDGE_TYPE_OFFSET       0x0147
#define ROM_TYPE_OFFSET             0x0148
#define RAM_TYPE_OFFSET             0x0149

uint8_t CartridgeType = 0x00;
uint8_t ROMType = 0x00;
uint8_t RAMType = 0x00;

typedef struct {
    struct {
        union {
            struct {
                uint8_t Lower:5;
                uint8_t Upper:2;
            };
            uint8_t Full:7;
        };
        bool Mode:1;
    };
    uint8_t data;
} bank_t;

bank_t Bank;

bool RAMEnabled = false;

uint8_t RAM0[0x0FFF];
uint8_t RAM1[0x0FFF];
uint8_t RAM2[0x0FFF];
uint8_t RAM3[0x0FFF];
uint8_t RAM4[0x0FFF];
uint8_t RAM5[0x0FFF];
uint8_t RAM6[0x0FFF];
uint8_t RAM7[0x0FFF];
uint8_t * RAM = RAM0;

uint8_t ZP[127];

uint8_t * ROM0 = NULL;
uint8_t * ROM = NULL;

uint8_t readByte(uint16_t address)
{
    LogVerbose("read %02X", address);
    if (address <= 0x00FF) {
        // Bootstrap / Jump Vectors
        if (BootstrapEnable) {
            return BOOTSTRAP[address];
        } else {
            return ROM0[address];
        }
    }
    else if (address <= 0x3FFF) {
        // ROM Bank 0
        return ROM0[address];
    }
    else if (address <= 0x7FFF) {
        // ROM Switchable Bank
        return ROM[address - 0x4000];
    }
    else if (address <= 0x97FF) {
        // Character RAM
        LogVerbose("read CharacterRAM @ %04Xh", address);
        return CharacterRAM[address - 0x8000];
    }
    else if (address <= 0x9BFF) {
        // BG Map Data 1
        LogVerbose("read BGMapData1 @ %04Xh", address);
        return BGMapData1[address - 0x9800];
    }
    else if (address <= 0x9FFF) {
        // BG Map Data 2
        LogVerbose("read BGMapData2 @ %04Xh", address);
        return BGMapData2[address - 0x9C00];
    }
    else if (address <= 0xBFFF) {
        // Cartridge RAM
    }
    else if (address <= 0xCFFF) {
        // Internal RAM Bank 0
        if (RAMEnabled) {
            return RAM0[address - 0xC000];
        }
    }
    else if (address <= 0xDFFF) {
        // Internal RAM Switchable Bank
        if (RAMEnabled) {
            return RAM[address - 0xD000];
        }
    }
    else if (address <= 0xFDFF) {
        // Echo RAM
        if (RAMEnabled) {
            return RAM0[address - 0xE000];
        }
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
        // Zero Page
        return ZP[address - 0xFF80];
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
    R.PC &= USHRT_MAX;
    return byte;
}

uint16_t nextWord()
{
    uint16_t word = readWord(R.PC);
    R.PC += 2;
    R.PC &= USHRT_MAX;
    return word;
}

void writeByte(uint16_t address, uint8_t data)
{
    LogVerbose("write %02X", address);
    if (address <= 0x7FFF) {

    }
    else if (address <= 0x97FF) {
        // Character RAM
        LogVerbose("write CharacterRAM @ %04Xh", address);
        CharacterRAM[address - 0x8000] = data;
    }
    else if (address <= 0x9BFF) {
        // BG Map Data 1
        LogVerbose("write BGMapData1 @ %04Xh", address);
        BGMapData1[address - 0x9800] = data;
    }
    else if (address <= 0x9FFF) {
        // BG Map Data 2
        LogVerbose("write BGMapData2 @ %04Xh", address);
        BGMapData1[address - 0x9C00] = data;
    }
    else if (address <= 0xBFFF) {
        // Cartridge RAM
    }
    else if (address <= 0xCFFF) {
        // Internal RAM Bank 0
        if (RAMEnabled) {
            RAM0[address - 0xC000] = data;
        }
    }
    else if (address <= 0xDFFF) {
        // Internal RAM Switchable Bank
        if (RAMEnabled) {
            RAM[address - 0xD000] = data;
        }
    }
    else if (address <= 0xFDFF) {
        // Echo RAM
        if (RAMEnabled) {
            RAM0[address - 0xE000] = data;
        }
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
        // Zero Page
        ZP[address - 0xFF80] = data;
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
    writeByte(address, (uint8_t)(data & 0x00FF));
    writeByte(address+1, (uint8_t)(data >> 8));
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

bool loadROM(const char * filename)
{
    FILE * file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    ROM0 = (uint8_t *)malloc(size);
    ROM = ROM0;

    size_t bytesRead = fread(ROM, 1, size, file);
    fclose(file);

    LogVerbose("read %zu bytes", bytesRead);

    if (bytesRead < size) {
        LogError("failed to load %s", filename);
        return false;
    }

    strncpy(Title, (char *)ROM0 + TITLE_OFFSET, sizeof(Title) - 1);
    Title[sizeof(Title) - 1] = '\0';

    ColorEnabled = (ROM0[COLOR_ENABLE_OFFSET] == 0x80);
    SuperEnabled = (ROM0[SUPER_ENABLE_OFFSET] != 0x00);

    CartridgeType = ROM0[CARTRIDGE_TYPE_OFFSET];
    ROMType = ROM0[ROM_TYPE_OFFSET];
    RAMType = ROM0[RAM_TYPE_OFFSET];

    return true;
}

void freeROM() 
{
    free(ROM);
}

void printBank()
{
    LogInfo("BANK Lower=%02X Upper=%01X Full=%04X Mode=%s", 
        Bank.Lower, Bank.Upper, Bank.Full, (Bank.Mode ? "RAM" : "ROM"));
}

void printCartridge()
{
    switch (CartridgeType) {
    case 0x00:
        LogInfo("Cartridge: ROM Only");
        break;
    case 0x01:
        LogInfo("Cartridge: ROM+MBC1");
        break;
    case 0x02:
        LogInfo("Cartridge: ROM+MBC1+RAM");
        break;
    case 0x03:
        LogInfo("Cartridge: ROM+MBC1+RAM+Battery");
        break;
    case 0x05:
        LogInfo("Cartridge: ROM+MBC2");
        break;
    case 0x06:
        LogInfo("Cartridge: ROM+MBC2+Battery");
        break;
    case 0x08:
        LogInfo("Cartridge: ROM+RAM");
        break;
    case 0x09:
        LogInfo("Cartridge: ROM+RAM+Battery");
        break;
    case 0x0B:
        LogInfo("Cartridge: ROM+MMM01");
        break;
    case 0x0C:
        LogInfo("Cartridge: ROM+MMM01+SRAM");
        break;
    case 0x0D:
        LogInfo("Cartridge: ROM+MMM01+SRAM+Battery");
        break;
    case 0x0F:
        LogInfo("Cartridge: ROM+MBC3+TIMER+Battery");
        break;
    case 0x10:
        LogInfo("Cartridge: ROM+MBC3+TIMER+RAM+Battery");
        break;
    case 0x11:
        LogInfo("Cartridge: ROM+MBC3");
        break;
    case 0x12:
        LogInfo("Cartridge: ROM+MBC3+RAM");
        break;
    case 0x13:
        LogInfo("Cartridge: ROM+MBC3+RAM+Battery");
        break;
    case 0x19:
        LogInfo("Cartridge: ROM+MBC5");
        break;
    case 0x1A:
        LogInfo("Cartridge: ROM+MBC5+RAM");
        break;
    case 0x1B:
        LogInfo("Cartridge: ROM+MBC5+RAM+Battery");
        break;
    case 0x1C:
        LogInfo("Cartridge: ROM+MBC5+Rumble");
        break;
    case 0x1D:
        LogInfo("Cartridge: ROM+MBC5+Rumble+SRAM");
        break;
    case 0x1E:
        LogInfo("Cartridge: ROM+MBC5+Rumble+SRAM+Battery");
        break;
    case 0x20:
        LogInfo("Cartridge: MBC6");
        break;
    case 0x22:
        LogInfo("Cartridge: MBC7+Sensor+Rumble+RAM+Battery");
        break;
    case 0xFC:
        LogInfo("Cartridge: Pocket Camera");
        break;
    case 0xFD:
        LogInfo("Cartridge: Bandai TAMA5");
        break;
    case 0xFE:
        LogInfo("Cartridge: Hudson HuC-3");
        break;
    case 0xFF:
        LogInfo("Cartridge: Hudson HuC-1");
        break;
    }

    switch (ROMType) {
    case 0x00:
        LogInfo("ROM: 32KB (no banks)");
        break;
    case 0x01:
        LogInfo("ROM: 64KB (4 banks)");
        break;
    case 0x02:
        LogInfo("ROM: 128KB (8 banks)");
        break;
    case 0x03:
        LogInfo("ROM: 256KB (16 banks)");
        break;
    case 0x04:
        LogInfo("ROM: 512KB (32 banks)");
        break;
    case 0x05:
        LogInfo("ROM: 1MB (64 banks)");
        break;
    case 0x06:
        LogInfo("ROM: 2MB (128 banks)");
        break;
    case 0x07:
        LogInfo("ROM: 4MB (256 banks)");
        break;
    case 0x08:
        LogInfo("ROM: 8MB (512 banks)");
        break;
    case 0x52:
        LogInfo("ROM: 1.1MB (72 banks)");
        break;
    case 0x53:
        LogInfo("ROM: 1.2MB (80 banks)");
        break;
    case 0x54:
        LogInfo("ROM: 1.5MB (96 banks)");
        break;
    }

    switch (RAMType) {
    case 0x00:
        LogInfo("RAM: None");
        break;
    case 0x01:
        LogInfo("RAM: 2KB");
        break;
    case 0x02:
        LogInfo("RAM: 8KB");
        break;
    case 0x03:
        LogInfo("RAM: 32KB (4 banks of 8KB)");
        break;
    case 0x04:
        LogInfo("RAM: 128KB (16 banks of 8KB");
        break;
    case 0x05:
        LogInfo("RAM: 64KB (8 banks of 8KB");
        break;
    }
}