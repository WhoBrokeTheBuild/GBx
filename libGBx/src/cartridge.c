#include "cartridge.h"

#include "clock.h"
#include "log.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_OFFSET 0x0100

typedef enum
{
    MBC_NONE,
    MBC_MBC1,
    MBC_MBC2,
    MBC_MBC3,
    MBC_MBC5,
    MBC_MBC6,
    MBC_MBC7,
    MBC_MMM01,

} mbc_type;

cartridge_header CartridgeHeader;

bool ColorEnabled = false;
bool SuperEnabled = false;

bool HasCartridgeBattery = false;
bool HasCartridgeTimer   = false;
bool HasCartridgeSRAM    = false;

mbc_type MBCType = MBC_NONE;

typedef struct
{
    struct {
        union {
            struct {
                byte Lower:5;
                byte Upper:2;
            };
            byte Full:7;
        };
        bool RAMMode:1;
    };
    byte raw;

} mbc_bank;

mbc_bank MBCBank;

bool SRAMEnabled;

byte SRAM[SRAM_BANK_COUNT][SRAM_BANK_SIZE];

uint SRAMBank;

byte ROM[ROM_BANK_COUNT][ROM_BANK_SIZE];

uint ROMBank;

void ResetCartridgeMBC()
{
    MBCBank.raw = 0x00;

    SRAMEnabled = false;
    SRAMBank = 0;

    for (uint i = 0; i < SRAM_BANK_COUNT; ++i) {
        memset(SRAM[i], 0, sizeof(SRAM[i]));
    }

    ROMBank = 1;
}

void WriteCartridgeMBC(word address, byte data)
{
    if (MBCType == MBC_MBC1) {
        if (address <= 0x1FFF) {
            // RAM Enable
            SRAMEnabled = ((data & 0x0F) == 0x0A);
            LogInfo("SRAM Enabled:%s", SRAMEnabled ? "true" : "false");
        }
        else if (address <= 0x3FFF) {
            // Cartridge ROM Bank Number
            if (data == 0) {
                data = 1;
            }
            
            MBCBank.Lower = data;
            ROMBank = MBCBank.Full;
        }
        else if (address <= 0x5FFF) {
            // RAM Bank Number or ROM Upper Bank Number
            MBCBank.Upper = data;
            if (MBCBank.RAMMode) {
                SRAMBank = MBCBank.Upper;
            }
            else {
                ROMBank = MBCBank.Full;
            }
        }
        else {
            // ROM/RAM Mode Select
            MBCBank.RAMMode = (data == 1);
        }
    }
    else if (MBCType == MBC_MBC2) {
        // Crazy
    }
    else if (MBCType == MBC_MBC3) {
        if (address <= 0x1FFF) {
            // RAM Enable
            SRAMEnabled = ((data & 0x0F) == 0x0A);
        }
        else if (address <= 0x3FFF) {
            // Cartridge ROM Bank Number
            if (data == 0) {
                data = 1;
            }
            
            MBCBank.Full = data;
            ROMBank = MBCBank.Full;
        }
        else if (address <= 0x5FFF) {
            
        }
    }
}

bool LoadCartridgeROM(const char * filename)
{
    LogVerbose(1, "Opening ROM: '%s'", filename);

    FILE * file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    LogVerbose(1, "ROM Size %zu", size);

    size_t bytesRead = fread(ROM, 1, size, file);
    fclose(file);

    LogVerbose(1, "Read %zu bytes", bytesRead);

    if (bytesRead < size) {
        LogFatal("Failed to load ROM: '%s'", filename);
        return false;
    }

    memcpy(CartridgeHeader.raw, &ROM[0][HEADER_OFFSET], sizeof(CartridgeHeader));

    LogVerbose(1, "ROM Title: %.*s", (int)sizeof(CartridgeHeader.Title), CartridgeHeader.Title);

    ColorEnabled = (CartridgeHeader.ColorEnabled == 0x80);
    SuperEnabled = (CartridgeHeader.SuperEnabled != 0x00);
    
    if (SuperEnabled) {
        ClockSpeed = SGB_CLOCK_SPEED;
    }

    switch (CartridgeHeader.CartridgeType) {
    case 0x00:
        // ROM Only
        break;
    case 0x01:
        // MBC1
        MBCType = MBC_MBC1;
        break;
    case 0x02:
        // MBC1+RAM
        MBCType = MBC_MBC1;
        HasCartridgeSRAM = true;
        break;
    case 0x03:
        // MBC1+RAM+Battery
        MBCType = MBC_MBC1;
        HasCartridgeSRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x05:
        // MBC2
        MBCType = MBC_MBC2;
        break;
    case 0x06:
        // MBC2+Battery
        MBCType = MBC_MBC2;
        HasCartridgeBattery = true;
        break;
    case 0x08:
        // RAM
        HasCartridgeSRAM = true;
        break;
    case 0x09:
        // RAM+Battery
        HasCartridgeSRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x0B:
        // MMM01
        MBCType = MBC_MMM01;
        break;
    case 0x0C:
        // MMM01+SRAM
        MBCType = MBC_MMM01;
        HasCartridgeSRAM = true;
        break;
    case 0x0D:
        // MMM01+SRAM+Battery
        MBCType = MBC_MMM01;
        HasCartridgeSRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x0F:
        // MBC3+Timer+Battery
        MBCType = MBC_MBC3;
        HasCartridgeTimer = true;
        HasCartridgeBattery = true;
        break;
    case 0x10:
        // MBC3+Timer+RAM+Battery
        MBCType = MBC_MBC3;
        HasCartridgeTimer = true;
        HasCartridgeSRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x11:
        // MBC3
        MBCType = MBC_MBC3;
        break;
    case 0x12:
        // MBC3+RAM
        MBCType = MBC_MBC3;
        HasCartridgeSRAM = true;
        break;
    case 0x13:
        // MBC3+RAM+Battery
        MBCType = MBC_MBC3;
        HasCartridgeSRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x19:
        // MBC5
        MBCType = MBC_MBC5;
        break;
    case 0x1A:
        // MBC5+RAM
        MBCType = MBC_MBC5;
        HasCartridgeSRAM = true;
        break;
    case 0x1B:
        // MBC5+RAM+Battery
        MBCType = MBC_MBC5;
        HasCartridgeSRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x1C:
        // MBC5+Rumble
        MBCType = MBC_MBC5;
        break;
    case 0x1D:
        // MBC5+Rumble+SRAM
        MBCType = MBC_MBC5;
        HasCartridgeSRAM = true;
        break;
    case 0x1E:
        // MBC5+Rumble+SRAM+Battery
        MBCType = MBC_MBC5;
        HasCartridgeSRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x20:
        // MBC6
        MBCType = MBC_MBC6;
        break;
    case 0x22:
        // MBC7+Sensor+Rumble+RAM+Battery
        MBCType = MBC_MBC7;
        HasCartridgeSRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0xFC:
        // Pocket Camera
        break;
    case 0xFD:
        // Bandai TAMA5
        break;
    case 0xFE:
        // Hudson HuC-3
        break;
    case 0xFF:
        // Hudson HuC-1
        break;
    default:
        break;
    }

    switch (CartridgeHeader.RAMType) {
    case 0x01: // 2KB
    case 0x02: // 8KB
    case 0x03: // 32KB (4 mbc_banks of 8KB)
    case 0x04: // 128KB (16 mbc_banks of 8KB)
    case 0x05: // 64KB (8 mbc_banks of 8KB)
        SRAMEnabled = true;
    default:
        break;
    }

    if (VerboseLevel >= 1) {
        PrintCartridge();
    }

    ResetCartridgeMBC();

    return true;
}

const char * GetCartridgeTypeString()
{
    switch (CartridgeHeader.CartridgeType) {
    case 0x00:
        return "ROM Only";
    case 0x01:
        return "MBC1";
    case 0x02:
        return "MBC1+RAM";
    case 0x03:
        return "MBC1+RAM+Battery";
    case 0x05:
        return "MBC2";
    case 0x06:
        return "MBC2+Battery";
    case 0x08:
        return "RAM";
    case 0x09:
        return "RAM+Battery";
    case 0x0B:
        return "MMM01";
    case 0x0C:
        return "MMM01+SRAM";
    case 0x0D:
        return "MMM01+SRAM+Battery";
    case 0x0F:
        return "MBC3+TIMER+Battery";
    case 0x10:
        return "MBC3+TIMER+RAM+Battery";
    case 0x11:
        return "MBC3";
    case 0x12:
        return "MBC3+RAM";
    case 0x13:
        return "MBC3+RAM+Battery";
    case 0x19:
        return "MBC5";
    case 0x1A:
        return "MBC5+RAM";
    case 0x1B:
        return "MBC5+RAM+Battery";
    case 0x1C:
        return "MBC5+Rumble";
    case 0x1D:
        return "MBC5+Rumble+SRAM";
    case 0x1E:
        return "MBC5+Rumble+SRAM+Battery";
    case 0x20:
        return "MBC6";
    case 0x22:
        return "MBC7+Sensor+Rumble+RAM+Battery";
    case 0xFC:
        return "Pocket Camera";
    case 0xFD:
        return "Bandai TAMA5";
    case 0xFE:
        return "Hudson HuC-3";
    case 0xFF:
        return "Hudson HuC-1";
    default:
        break;
    }

    return NULL;
}

const char * GetROMTypeString()
{
    switch (CartridgeHeader.ROMType) {
    case 0x00:
        return "32KB (no banks)";
    case 0x01:
        return "64KB (4 banks)";
    case 0x02:
        return "128KB (8 banks)";
    case 0x03:
        return "256KB (16 banks)";
    case 0x04:
        return "512KB (32 banks)";
    case 0x05:
        return "1MB (64 banks)";
    case 0x06:
        return "2MB (128 banks)";
    case 0x07:
        return "4MB (256 banks)";
    case 0x08:
        return "8MB (512 banks)";
    case 0x52:
        return "1.1MB (72 banks)";
    case 0x53:
        return "1.2MB (80 banks)";
    case 0x54:
        return "1.5MB (96 banks)";
    default:
        break;
    }

    return NULL;
}

const char * GetRAMTypeString()
{
    switch (CartridgeHeader.RAMType) {
    case 0x00:
        return "None";
    case 0x01:
        return "2KB";
    case 0x02:
        return "8KB";
    case 0x03:
        return "32KB (4 bank of 8KB)";
    case 0x04:
        return "128KB (16 bank of 8KB";
    case 0x05:
        return "64KB (8 bank of 8KB";
    default:
        break;
    }

    return NULL;
}

void PrintCartridgeMBC()
{
    LogInfo("MBC: Lower=%02X Upper=%01X Full=%04X Mode=%s", 
        MBCBank.Lower, MBCBank.Upper, MBCBank.Full, 
        (MBCBank.RAMMode ? "RAM" : "ROM"));
}

void PrintCartridge()
{
    const char * cartridgeType = GetCartridgeTypeString();
    if (cartridgeType) {
        LogInfo("Cartridge: %s", cartridgeType);
    }
    else {
        LogWarn("Cartridge: Type %02X unknown", CartridgeHeader.CartridgeType);
    }

    const char * romType = GetROMTypeString();
    if (romType) {
        LogInfo("ROM: %s", romType);
    }
    else {
        LogWarn("ROM: Type %02X unknown", CartridgeHeader.ROMType);
    }

    const char * ramType = GetRAMTypeString();
    if (ramType) {
        LogInfo("RAM: %s", ramType);
    }
    else {
        LogWarn("RAM: Type %02X unknown", CartridgeHeader.RAMType);
    }
}