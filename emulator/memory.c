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
        LogInfo("read CharacterRAM @ %04Xh", address);
        return CharacterRAM[address - 0x8000];
    }
    else if (address <= 0x9BFF) {
        // BG Map Data 1
        LogInfo("read BGMapData1 @ %04Xh", address);
        return BGMapData1[address - 0x9800];
    }
    else if (address <= 0x9FFF) {
        // BG Map Data 2
        LogInfo("read BGMapData2 @ %04Xh", address);
        return BGMapData2[address - 0x9C00];
    }
    else if (address <= 0xBFFF) {
        // Cartridge RAM
    }
    else if (address <= 0xCFFF) {
        // Internal RAM Bank 0
        return RAM0[address - 0xC000];
    }
    else if (address <= 0xDFFF) {
        // Internal RAM Switchable Bank
        return RAM[address - 0xD000];
    }
    else if (address <= 0xFDFF) {
        // Echo RAM
        return RAM0[address - 0xE000];
    }
    else if (address <= 0xFE9F) {
        // Object Attribute Memory OAM
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
            return LCDC.data;
            printLCDC();
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
        printIE();
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
    LogVerbose("write %02X", address);
    if (address <= 0x00FF) {
        // Bootstrap / Jump Vectors
    }
    else if (address <= 0x3FFF) {
        // ROM Bank 0
    }
    else if (address <= 0x7FFF) {
        // ROM Switchable Bank
    }
    else if (address <= 0x97FF) {
        // Character RAM
        LogInfo("write CharacterRAM @ %04Xh", address);
        CharacterRAM[address - 0x8000] = data;
    }
    else if (address <= 0x9BFF) {
        // BG Map Data 1
        LogInfo("write BGMapData1 @ %04Xh", address);
        BGMapData1[address - 0x9800] = data;
    }
    else if (address <= 0x9FFF) {
        // BG Map Data 2
        LogInfo("write BGMapData2 @ %04Xh", address);
        BGMapData1[address - 0x9C00] = data;
    }
    else if (address <= 0xBFFF) {
        // Cartridge RAM
    }
    else if (address <= 0xCFFF) {
        // Internal RAM Bank 0
        RAM0[address - 0xC000] = data;
    }
    else if (address <= 0xDFFF) {
        // Internal RAM Switchable Bank
        RAM[address - 0xD000] = data;
    }
    else if (address <= 0xFDFF) {
        // Echo RAM
        RAM0[address - 0xE000] = data;
    }
    else if (address <= 0xFE9F) {
        // Object Attribute Memory OAM
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
        case 0xFF01:
            SB = data;
        case 0xFF02:
            SC.data = data;
        case 0xFF04:
            DIV = data;
        case 0xFF05:
            TIMA = data;
        case 0xFF06:
            TMA = data;
        case 0xFF07:
            TAC.data = data;
        case 0xFF0F:
            IF.data = data;
        case 0xFF10:
            NR10 = data;
        case 0xFF11:
            NR11 = data;
        case 0xFF12:
            NR12 = data;
        case 0xFF13:
            NR13 = data;
        case 0xFF14:
            NR14 = data;
        case 0xFF16:
            NR21 = data;
        case 0xFF17:
            NR22 = data;
        case 0xFF18:
            NR23 = data;
        case 0xFF19:
            NR24 = data;
        case 0xFF1A:
            NR30 = data;
        case 0xFF1B:
            NR31 = data;
        case 0xFF1C:
            NR32 = data;
        case 0xFF1D:
            NR33 = data;
        case 0xFF1E:
            NR34 = data;
        case 0xFF20:
            NR41 = data;
        case 0xFF21:
            NR42 = data;
        case 0xFF22:
            NR43 = data;
        case 0xFF23:
            NR44 = data;
        case 0xFF24:
            NR50 = data;
        case 0xFF25:
            NR51 = data;
        case 0xFF26:
            NR52 = data;
        
        case 0xFF40:
            LCDC.data = data;
            printLCDC();
        case 0xFF41:
            STAT.data = data;
        case 0xFF42:
            SCY = data;
        case 0xFF43:
            SCX = data;
        case 0xFF44:
            LY = data;
        case 0xFF45:
            LYC = data;
        
        case 0xFF50:
            BootstrapEnable = data;
        };
    }
    else if (address <= 0xFFFE) {
        // Zero Page
        ZP[address - 0xFF80] = data;
    }
    else if (address == 0xFFFF) {
        // Interrupt Enable Flag
        IE.data = data;
        printIE();
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

    ROM = ROM0 = (uint8_t *)malloc(size);

    size_t bytesRead = fread(ROM, 1, size, file);
    fclose(file);

    LogVerbose("read %zu bytes", bytesRead);

    if (bytesRead < size) {
        LogError("failed to load %s", filename);
        return false;
    }

    return true;
}

void freeROM() 
{
    free(ROM);
}