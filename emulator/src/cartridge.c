#include "cartridge.h"

#include "clock.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_OFFSET 0x0100

typedef enum {
    MBC_NONE,
    MBC_MBC1,
    MBC_MBC2,
    MBC_MBC3,
    MBC_MBC5,
    MBC_MBC6,
    MBC_MBC7,
    MBC_MMM01,
    
} mbc_type_t;

cartridge_header_t CartridgeHeader;

bool ColorEnabled = false;
bool SuperEnabled = false;

uint8_t CartridgeType = 0x00;
uint8_t ROMType       = 0x00;
uint8_t RAMType       = 0x00;

bool HasCartridgeBattery = false;
bool HasCartridgeTimer   = false;
bool HasCartridgeRAM     = false;
bool HasCartridgeSRAM    = false;

mbc_type_t MBCType = MBC_NONE;

typedef struct {
    struct {
        union {
            struct {
                uint8_t Lower:5;
                uint8_t Upper:2;
            };
            uint8_t Full:7;
        };
        bool RAMMode:1;
    };
    uint8_t data;
} bank_t;

bank_t Bank;

bool CartridgeRAMEnabled = false;

uint8_t * CartridgeRAM0 = NULL;
uint8_t * CartridgeRAM = NULL;

uint8_t * CartridgeROM0 = NULL;
uint8_t * CartridgeROM = NULL;

void writeCartridgeMBC(uint16_t address, uint8_t data)
{
    if (MBCType == MBC_MBC1) {
        if (address <= 0x1FFF) {
            // RAM Enable
            LogInfo("%04X %02X", address, data);
            CartridgeRAMEnabled = ((data & 0x0F) == 0x0A);
            LogInfo("Cart RAM Enabled:%s", CartridgeRAMEnabled ? "true" : "false");
        }
        else if (address <= 0x3FFF) {
            // Cartridge ROM Bank Number
            if (data == 0) {
                data = 1;
            }
            
            Bank.Lower = data;
            CartridgeROM = CartridgeROM0 + (0x4000 * Bank.Full);
        }
        else if (address <= 0x5FFF) {
            // RAM Bank Number or CartridgeROM Upper Bank Number
            Bank.Upper = data;
            if (Bank.RAMMode) {
                CartridgeRAM = CartridgeRAM0 + (0x2000 * Bank.Upper);
            }
            else {
                CartridgeROM = CartridgeROM0 + (0x4000 * Bank.Full);
            }
        }
        else {
            // CartridgeROM/RAM Mode Select
            Bank.RAMMode = (data == 1);
        }
    }
    else if (MBCType == MBC_MBC2) {
        // Crazy
    }
    else if (MBCType == MBC_MBC3) {
        if (address <= 0x1FFF) {
            // RAM Enable
            CartridgeRAMEnabled = ((data & 0x0F) == 0x0A);
        }
        else if (address <= 0x3FFF) {
            // Cartridge ROM Bank Number
            if (data == 0) {
                data = 1;
            }
            
            Bank.Full = data;
            CartridgeROM = CartridgeROM0 + (0x4000 * Bank.Full);
        }
        else if (address <= 0x5FFF) {
        }
    }
}

bool loadCartridge(const char * filename)
{
    LogVerbose(1, "Opening ROM: '%s'", filename);

    FILE * file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    LogVerbose(1, "ROM Size %zu", size);

    CartridgeROM0 = (uint8_t *)malloc(size);
    CartridgeROM = CartridgeROM0 + 0x4000; // Default to Bank 1

    size_t bytesRead = fread(CartridgeROM0, 1, size, file);
    fclose(file);

    LogVerbose(1, "Read %zu bytes", bytesRead);

    if (bytesRead < size) {
        LogFatal("Failed to load ROM: '%s'", filename);
        return false;
    }

    memcpy(CartridgeHeader.data, CartridgeROM0 + HEADER_OFFSET, sizeof(CartridgeHeader));

    LogVerbose(1, "ROM Title: %.*s", 15, CartridgeHeader.Title);

    ColorEnabled = (CartridgeHeader.ColorEnabled == 0x80);
    SuperEnabled = (CartridgeHeader.SuperEnabled != 0x00);
    
    if (SuperEnabled) {
        ClockSpeed = SGB_CLOCK_SPEED;
    }

    // CartridgeHeader.ROMType

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
        HasCartridgeRAM = true;
        break;
    case 0x03:
        // MBC1+RAM+Battery
        MBCType = MBC_MBC1;
        HasCartridgeRAM = true;
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
        HasCartridgeRAM = true;
        break;
    case 0x09:
        // RAM+Battery
        HasCartridgeRAM = true;
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
        HasCartridgeRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x11:
        // MBC3
        MBCType = MBC_MBC3;
        break;
    case 0x12:
        // MBC3+RAM
        MBCType = MBC_MBC3;
        HasCartridgeRAM = true;
        break;
    case 0x13:
        // MBC3+RAM+Battery
        MBCType = MBC_MBC3;
        HasCartridgeRAM = true;
        HasCartridgeBattery = true;
        break;
    case 0x19:
        // MBC5
        MBCType = MBC_MBC5;
        break;
    case 0x1A:
        // MBC5+RAM
        MBCType = MBC_MBC5;
        HasCartridgeRAM = true;
        break;
    case 0x1B:
        // MBC5+RAM+Battery
        MBCType = MBC_MBC5;
        HasCartridgeRAM = true;
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
        HasCartridgeRAM = true;
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
    case 0x01:
        // 2KB
        CartridgeRAM0 = (uint8_t *)malloc(2048);
        CartridgeRAMEnabled = true;
        break;
    case 0x02:
        // 8KB
        CartridgeRAM0 = (uint8_t *)malloc(8120);
        CartridgeRAMEnabled = true;
        break;
    case 0x03:
        // 32KB (4 banks of 8KB)
        CartridgeRAM0 = (uint8_t *)malloc(32768);
        CartridgeRAMEnabled = true;
        break;
    case 0x04:
        // 128KB (16 banks of 8KB
        CartridgeRAM0 = (uint8_t *)malloc(131072);
        CartridgeRAMEnabled = true;
        break;
    case 0x05:
        // 64KB (8 banks of 8KB
        CartridgeRAM0 = (uint8_t *)malloc(65536);
        CartridgeRAMEnabled = true;
        break;
    default:
        break;
    }

    if (VerboseLevel >= 1) {
        printCartridge();
    }

    return true;
}

void freeCartridge() 
{
    free(CartridgeROM0);
    CartridgeROM0 = NULL;

    free(CartridgeRAM0);
    CartridgeRAM0 = NULL;
}

void printBank()
{
    LogInfo("Bank: Lower=%02X Upper=%01X Full=%04X Mode=%s", 
        Bank.Lower, Bank.Upper, Bank.Full, 
        (Bank.RAMMode ? "RAM" : "ROM"));
}

void printCartridge()
{
    switch (CartridgeType) {
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
        LogInfo("Cartridge: Type %02X unknown", CartridgeType);
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
    default:
        LogInfo("ROM: Type %02X unknown", ROMType);
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
    default:
        LogInfo("RAM: Type %02X unknown", RAMType);
        break;
    }
}