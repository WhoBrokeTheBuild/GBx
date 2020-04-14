#include <GBx/Cartridge.h>

#include <GBx/Context.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Internal.h"
#include "Log.h"

void GBx_ResetCartridge(gbx_t * ctx)
{
    ctx->MBC.raw = 0x00;

    ctx->SRAMEnabled = false;
    ctx->SRAMBank = 0;
    ctx->ROMBank = 1;

    for (unsigned i = 0; i < GBX_SRAM_BANK_COUNT; ++i) {
        memset(ctx->SRAM[i], 0, sizeof(ctx->SRAM[i]));
    }
}

bool GBx_LoadCartridge(gbx_t * ctx, const char * filename)
{
    const uint16_t HEADER_OFFSET = 0x0100;

    Verbose(ctx, 1, LogInfo("Opening ROM: '%s'", filename));

    FILE * file = fopen(filename, "rb");
    if (!file) {
        LogFatal("Unable to open ROM '%s'", filename);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    Verbose(ctx, 1, LogInfo("ROM Size %zu", size));

    size_t bytesRead = fread(ctx->ROM, 1, size, file);
    fclose(file);

    Verbose(ctx, 1, LogInfo("Read %zu bytes", bytesRead));

    if (bytesRead < size) {
        LogFatal("Failed to load ROM: '%s'", filename);
        return false;
    }

    memcpy(ctx->CartridgeHeader.raw, 
        &ctx->ROM[0][HEADER_OFFSET], 
        sizeof(ctx->CartridgeHeader));

    Verbose(ctx, 1, LogInfo(
        "ROM Title: %.*s",
        (int)sizeof(ctx->CartridgeHeader.Title),
        ctx->CartridgeHeader.Title));

    ctx->ColorEnabled = (ctx->CartridgeHeader.ColorEnabled == 0x80);
    ctx->SuperEnabled = (ctx->CartridgeHeader.SuperEnabled != 0x00);

    if (ctx->ColorEnabled) {
        SM83_SetMode(ctx->CPU, SM83_MODE_CGB);
    }
    else if (ctx->SuperEnabled) {
        SM83_SetMode(ctx->CPU, SM83_MODE_SGB);
    }

    switch (ctx->CartridgeHeader.CartridgeType) {
    case 0x00:
        // ROM Only
        break;
    case 0x01:
        // MBC1
        ctx->MBCType = GBX_MBC_TYPE_MBC1;
        break;
    case 0x02:
        // MBC1+RAM
        ctx->MBCType = GBX_MBC_TYPE_MBC1;
        ctx->HaveCartridgeSRAM = true;
        break;
    case 0x03:
        // MBC1+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC1;
        ctx->HaveCartridgeSRAM = true;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x05:
        // MBC2
        ctx->MBCType = GBX_MBC_TYPE_MBC2;
        break;
    case 0x06:
        // MBC2+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC2;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x08:
        // RAM
        ctx->HaveCartridgeSRAM = true;
        break;
    case 0x09:
        // RAM+BATT
        ctx->HaveCartridgeSRAM = true;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x0B:
        // MMM01
        ctx->MBCType = GBX_MBC_TYPE_MMM01;
        break;
    case 0x0C:
        // MMM01+SRAM
        ctx->MBCType = GBX_MBC_TYPE_MMM01;
        ctx->HaveCartridgeSRAM = true;
        break;
    case 0x0D:
        // MMM01+SRAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MMM01;
        ctx->HaveCartridgeSRAM = true;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x0F:
        // MBC3+Timer+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        ctx->HaveCartridgeTimer = true;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x10:
        // MBC3+Timer+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        ctx->HaveCartridgeTimer = true;
        ctx->HaveCartridgeSRAM = true;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x11:
        // MBC3
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        break;
    case 0x12:
        // MBC3+RAM
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        ctx->HaveCartridgeSRAM = true;
        break;
    case 0x13:
        // MBC3+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        ctx->HaveCartridgeSRAM = true;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x19:
        // MBC5
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1A:
        // MBC5+RAM
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        ctx->HaveCartridgeSRAM = true;
        break;
    case 0x1B:
        // MBC5+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        ctx->HaveCartridgeSRAM = true;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x1C:
        // MBC5+Rumble
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1D:
        // MBC5+Rumble+SRAM
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        ctx->HaveCartridgeSRAM = true;
        break;
    case 0x1E:
        // MBC5+Rumble+SRAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        ctx->HaveCartridgeSRAM = true;
        ctx->HaveCartridgeBattery = true;
        break;
    case 0x20:
        // MBC6
        ctx->MBCType = GBX_MBC_TYPE_MBC6;
        break;
    case 0x22:
        // MBC7+Sensor+Rumble+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC7;
        ctx->HaveCartridgeSRAM = true;
        ctx->HaveCartridgeBattery = true;
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

    switch (ctx->CartridgeHeader.RAMType) {
    case 0x01: // 2KB
    case 0x02: // 8KB
    case 0x03: // 32KB (4 mbc_banks of 8KB)
    case 0x04: // 128KB (16 mbc_banks of 8KB)
    case 0x05: // 64KB (8 mbc_banks of 8KB)
        ctx->internal->HaveSRAM = true;
    default:
        break;
    }

    ctx->SRAMEnabled = false;

    if (ctx->VerboseLevel >= 1) {
        GBx_PrintCartridge(ctx);
    }

    return true;
}

const char * GBx_GetCartridgeTypeString(uint8_t type)
{
    switch (type) {
    case 0x00:
        return "ROM Only";
    case 0x01:
        return "MBC1";
    case 0x02:
        return "MBC1+RAM";
    case 0x03:
        return "MBC1+RAM+BATT";
    case 0x05:
        return "MBC2";
    case 0x06:
        return "MBC2+BATT";
    case 0x08:
        return "RAM";
    case 0x09:
        return "RAM+BATT";
    case 0x0B:
        return "MMM01";
    case 0x0C:
        return "MMM01+SRAM";
    case 0x0D:
        return "MMM01+SRAM+BATT";
    case 0x0F:
        return "MBC3+TIMER+BATT";
    case 0x10:
        return "MBC3+TIMER+RAM+BATT";
    case 0x11:
        return "MBC3";
    case 0x12:
        return "MBC3+RAM";
    case 0x13:
        return "MBC3+RAM+BATT";
    case 0x19:
        return "MBC5";
    case 0x1A:
        return "MBC5+RAM";
    case 0x1B:
        return "MBC5+RAM+BATT";
    case 0x1C:
        return "MBC5+RMBL";
    case 0x1D:
        return "MBC5+RMBL+SRAM";
    case 0x1E:
        return "MBC5+RMBL+SRAM+BATT";
    case 0x20:
        return "MBC6";
    case 0x22:
        return "MBC7+SNSR+RMBL+RAM+BATT";
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

const char * GBx_GetROMTypeString(uint8_t type)
{
    switch (type) {
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

const char * GBx_GetRAMTypeString(uint8_t type)
{
    switch (type) {
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
void GBx_WriteByteMBC(gbx_t * ctx, uint16_t address, uint8_t data)
{
    if (ctx->MBCType == GBX_MBC_TYPE_MBC1) {
        if (address <= 0x1FFF) {
            // RAM Enable
            ctx->SRAMEnabled = ((data & 0x0F) == 0x0A);
            LogInfo("SRAM: %s", GBx_GetEnabledString(ctx->SRAMEnabled));
        }
        else if (address <= 0x3FFF) {
            // Cartridge ROM Bank Number
            if (data == 0) {
                data = 1;
            }

            ctx->MBC.Lower = data;
            ctx->ROMBank = ctx->MBC.Full;
        }
        else if (address <= 0x5FFF) {
            // RAM Bank Number or ROM Upper Bank Number
            ctx->MBC.Upper = data;
            if (ctx->MBC.RAMMode) {
                ctx->SRAMBank = ctx->MBC.Upper;
            }
            else {
                ctx->ROMBank = ctx->MBC.Full;
            }
        }
        else {
            // ROM/RAM Mode Select
            ctx->MBC.RAMMode = (data == 1);
        }
    }
    else if (ctx->MBCType == GBX_MBC_TYPE_MBC2) {
        // Crazy
    }
    else if (ctx->MBCType == GBX_MBC_TYPE_MBC3) {
        if (address <= 0x1FFF) {
            // RAM Enable
            ctx->SRAMEnabled = ((data & 0x0F) == 0x0A);
        }
        else if (address <= 0x3FFF) {
            // Cartridge ROM Bank Number
            if (data == 0) {
                data = 1;
            }

            ctx->MBC.Full = data;
            ctx->ROMBank = ctx->MBC.Full;
        }
        else if (address <= 0x5FFF) {
        }
    }
}

void GBx_PrintCartridge(gbx_t * ctx)
{
    const char * cartridgeType = GBx_GetCartridgeTypeString(ctx->CartridgeHeader.CartridgeType);
    if (cartridgeType) {
        LogInfo("Cartridge: %s", cartridgeType);
    }
    else {
        LogWarn("Cartridge: Type %02X unknown", ctx->CartridgeHeader.CartridgeType);
    }

    const char * romType = GBx_GetROMTypeString(ctx->CartridgeHeader.ROMType);
    if (romType) {
        LogInfo("ROM: %s", romType);
    }
    else {
        LogWarn("ROM: Type %02X unknown", ctx->CartridgeHeader.ROMType);
    }

    const char * ramType = GBx_GetRAMTypeString(ctx->CartridgeHeader.RAMType);
    if (ramType) {
        LogInfo("RAM: %s", ramType);
    }
    else {
        LogWarn("RAM: Type %02X unknown", ctx->CartridgeHeader.RAMType);
    }
}

void GBx_PrintMBC(gbx_t * ctx)
{
    LogInfo("MBC: Lower=%02X Upper=%01X Full=%04X Mode=%s",
        ctx->MBC.Lower,
        ctx->MBC.Upper,
        ctx->MBC.Full,
        (ctx->MBC.RAMMode ? "RAM" : "ROM"));
}