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

void PrintCartridgeMBC()
{
    LogInfo("MBC: Lower=%02X Upper=%01X Full=%04X Mode=%s", 
        MBCBank.Lower, MBCBank.Upper, MBCBank.Full, 
        (MBCBank.RAMMode ? "RAM" : "ROM"));
}

void PrintCartridge()
{
    switch (CartridgeHeader.CartridgeType) {
    case 0x00:
        LogInfo("Cartridge: ROM Only");
        break;
    case 0x01:
        LogInfo("Cartridge: MBC1");
        break;
    case 0x02:
        LogInfo("Cartridge: MBC1+RAM");
        break;
    case 0x03:
        LogInfo("Cartridge: MBC1+RAM+Battery");
        break;
    case 0x05:
        LogInfo("Cartridge: MBC2");
        break;
    case 0x06:
        LogInfo("Cartridge: MBC2+Battery");
        break;
    case 0x08:
        LogInfo("Cartridge: RAM");
        break;
    case 0x09:
        LogInfo("Cartridge: RAM+Battery");
        break;
    case 0x0B:
        LogInfo("Cartridge: MMM01");
        break;
    case 0x0C:
        LogInfo("Cartridge: MMM01+SRAM");
        break;
    case 0x0D:
        LogInfo("Cartridge: MMM01+SRAM+Battery");
        break;
    case 0x0F:
        LogInfo("Cartridge: MBC3+TIMER+Battery");
        break;
    case 0x10:
        LogInfo("Cartridge: MBC3+TIMER+RAM+Battery");
        break;
    case 0x11:
        LogInfo("Cartridge: MBC3");
        break;
    case 0x12:
        LogInfo("Cartridge: MBC3+RAM");
        break;
    case 0x13:
        LogInfo("Cartridge: MBC3+RAM+Battery");
        break;
    case 0x19:
        LogInfo("Cartridge: MBC5");
        break;
    case 0x1A:
        LogInfo("Cartridge: MBC5+RAM");
        break;
    case 0x1B:
        LogInfo("Cartridge: MBC5+RAM+Battery");
        break;
    case 0x1C:
        LogInfo("Cartridge: MBC5+Rumble");
        break;
    case 0x1D:
        LogInfo("Cartridge: MBC5+Rumble+SRAM");
        break;
    case 0x1E:
        LogInfo("Cartridge: MBC5+Rumble+SRAM+Battery");
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
    default:
        LogInfo("Cartridge: Type %02X unknown", CartridgeHeader.CartridgeType);
        break;
    }

    switch (CartridgeHeader.ROMType) {
    case 0x00:
        LogInfo("ROM: 32KB (no mbc_banks)");
        break;
    case 0x01:
        LogInfo("ROM: 64KB (4 mbc_banks)");
        break;
    case 0x02:
        LogInfo("ROM: 128KB (8 mbc_banks)");
        break;
    case 0x03:
        LogInfo("ROM: 256KB (16 mbc_banks)");
        break;
    case 0x04:
        LogInfo("ROM: 512KB (32 mbc_banks)");
        break;
    case 0x05:
        LogInfo("ROM: 1MB (64 mbc_banks)");
        break;
    case 0x06:
        LogInfo("ROM: 2MB (128 mbc_banks)");
        break;
    case 0x07:
        LogInfo("ROM: 4MB (256 mbc_banks)");
        break;
    case 0x08:
        LogInfo("ROM: 8MB (512 mbc_banks)");
        break;
    case 0x52:
        LogInfo("ROM: 1.1MB (72 mbc_banks)");
        break;
    case 0x53:
        LogInfo("ROM: 1.2MB (80 mbc_banks)");
        break;
    case 0x54:
        LogInfo("ROM: 1.5MB (96 mbc_banks)");
        break;
    default:
        LogInfo("ROM: Type %02X unknown", CartridgeHeader.ROMType);
        break;
    }

    switch (CartridgeHeader.RAMType) {
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
        LogInfo("RAM: 32KB (4 mbc_banks of 8KB)");
        break;
    case 0x04:
        LogInfo("RAM: 128KB (16 mbc_banks of 8KB");
        break;
    case 0x05:
        LogInfo("RAM: 64KB (8 mbc_banks of 8KB");
        break;
    default:
        LogInfo("RAM: Type %02X unknown", CartridgeHeader.RAMType);
        break;
    }
}