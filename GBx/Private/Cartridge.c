#include <GBx/Cartridge.h>
#include <GBx/Context.h>
#include <GBx/Log.h>

#include <stdio.h>
#include <string.h>

void GBx_Cartridge_Init(GBx * ctx)
{
    ctx->Header = (GBx_CartridgeHeader *)&ctx->ROM[0][GBX_ROM_HEADER_OFFSET];

    memset(ctx->ROM, 0, sizeof(ctx->ROM));

    ctx->HaveSRAM = false;
    ctx->HaveBattery = false;
    ctx->HaveTimer = false;
    
    ctx->MBCType = GBX_MBC_TYPE_NONE;

    GBx_Cartridge_Reset(ctx);
}

void GBx_Cartridge_Reset(GBx * ctx)
{
    ctx->SRAMEnabled = false;

    ctx->MBC._raw = 0x00;

    ctx->ROMBank = 0;

    memset(ctx->SRAM, 0, sizeof(ctx->SRAM));
    ctx->SRAMBank = 0;
}

bool GBx_Cartridge_Load(GBx * ctx, const char * filename)
{
    GBxLogLoad("Opening ROM: '%s'", filename);

    FILE * file = fopen(filename, "rb");
    if (!file) {
        GBxLogError("Unable to open ROM '%s'", filename);
        return false;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size < GBX_ROM_BANK_SIZE) {
        GBxLogError("Unable to read ROM header '%s'", filename);
        return false;
    }

    size_t bytesRead = fread(ctx->ROM, 1, size, file);
    fclose(file);

    GBxLogVerbose("Read %zu/%zu bytes", bytesRead, size);

    if (bytesRead < size) {
        GBxLogError("Failed to load entire ROM: '%s'", filename);
        return false;
    }

    uint8_t sum = 0;
    for (uint16_t addr = 0x0134; addr <= 0x014C; ++addr) {
        sum = sum - ctx->ROM[0][addr] - (uint8_t)1;
    }

    if (ctx->Header->ComplementCheck != sum) {
        GBxLogError("Failed to verify ROM complement check, %u != %u", 
            ctx->Header->ComplementCheck, sum);
        return false;
    }

    int titleLength = sizeof(ctx->Header->Title);
    GBxLogVerbose("ROM Title: %.*s",  titleLength,  ctx->Header->Title);

    ctx->Mode = GBX_MODE_DMG;
    if (ctx->Header->ColorEnabled == 0x80) {
        ctx->Mode = GBX_MODE_CGB;
    }
    else if (ctx->Header->SuperEnabled != 0x00) {
        ctx->Mode = GBX_MODE_SGB;
    }

    switch (ctx->Header->CartridgeType) {
    case 0x00: // ROM Only
        break;
    case 0x01: // MBC1
        ctx->MBCType = GBX_MBC_TYPE_MBC1;
        break;
    case 0x02: // MBC1+RAM
        ctx->MBCType = GBX_MBC_TYPE_MBC1;
        ctx->HaveSRAM = true;
        break;
    case 0x03: // MBC1+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC1;
        ctx->HaveSRAM = true;
        ctx->HaveBattery = true;
        break;
    case 0x05: // MBC2
        ctx->MBCType = GBX_MBC_TYPE_MBC2;
        break;
    case 0x06: // MBC2+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC2;
        ctx->HaveBattery = true;
        break;
    case 0x08: // RAM
        ctx->HaveSRAM = true;
        break;
    case 0x09: // RAM+BATT
        ctx->HaveSRAM = true;
        ctx->HaveBattery = true;
        break;
    case 0x0B: // MMM01
        ctx->MBCType = GBX_MBC_TYPE_MMM01;
        break;
    case 0x0C: // MMM01+SRAM
        ctx->MBCType = GBX_MBC_TYPE_MMM01;
        ctx->HaveSRAM = true;
        break;
    case 0x0D: // MMM01+SRAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MMM01;
        ctx->HaveSRAM = true;
        ctx->HaveBattery = true;
        break;
    case 0x0F: // MBC3+Timer+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        ctx->HaveTimer = true;
        ctx->HaveBattery = true;
        break;
    case 0x10: // MBC3+Timer+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        ctx->HaveTimer = true;
        ctx->HaveSRAM = true;
        ctx->HaveBattery = true;
        break;
    case 0x11: // MBC3
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        break;
    case 0x12: // MBC3+RAM
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        ctx->HaveSRAM = true;
        break;
    case 0x13: // MBC3+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        ctx->HaveSRAM = true;
        ctx->HaveBattery = true;
        break;
    case 0x19: // MBC5
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1A: // MBC5+RAM
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        ctx->HaveSRAM = true;
        break;
    case 0x1B: // MBC5+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        ctx->HaveSRAM = true;
        ctx->HaveBattery = true;
        break;
    case 0x1C: // MBC5+Rumble
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1D: // MBC5+Rumble+SRAM
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        ctx->HaveSRAM = true;
        break;
    case 0x1E: // MBC5+Rumble+SRAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        ctx->HaveSRAM = true;
        ctx->HaveBattery = true;
        break;
    case 0x20: // MBC6
        ctx->MBCType = GBX_MBC_TYPE_MBC6;
        break;
    case 0x22: // MBC7+Sensor+Rumble+RAM+BATT
        ctx->MBCType = GBX_MBC_TYPE_MBC7;
        ctx->HaveSRAM = true;
        ctx->HaveBattery = true;
        break;
    case 0xFC: // Pocket Camera
        break;
    case 0xFD: // Bandai TAMA5
        break;
    case 0xFE: // Hudson HuC-3
        break;
    case 0xFF: // Hudson HuC-1
        break;
    default:
        break;
    }

    // switch (ctx->Header->SRAMType) {
    // case 0x01: // 2KB
    // case 0x02: // 8KB
    // case 0x03: // 32KB (4 banks of 8KB)
    // case 0x04: // 128KB (16 banks of 8KB)
    // case 0x05: // 64KB (8 banks of 8KB)
    //     ctx->HaveSRAM = true;
    // default:
    //     break;
    // }

    // Disabled by default, enabled by MMIO
    ctx->SRAMEnabled = false;
    
    GBx_Cartridge_PrintHeader(ctx);

    return true;
}

void GBx_Cartridge_MBCWriteByte(GBx * ctx, uint16_t address, uint8_t data)
{
    // TODO:
}

const char * GBx_Cartridge_GetTypeString(uint8_t type)
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

const char * GBx_Cartridge_GetROMTypeString(uint8_t type)
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

const char * GBx_Cartridge_GetSRAMTypeString(uint8_t type)
{
    switch (type) {
    case 0x00:
        return "None";
    case 0x01:
        return "2KB";
    case 0x02:
        return "8KB";
    case 0x03:
        return "32KB (4 banks of 8KB)";
    case 0x04:
        return "128KB (16 banks of 8KB";
    case 0x05:
        return "64KB (8 banks of 8KB";
    default:
        break;
    }

    return NULL;
}

void GBx_Cartridge_PrintHeader(GBx * ctx)
{
    const char * type = NULL;

    type = GBx_Cartridge_GetTypeString(ctx->Header->CartridgeType);
    if (type) {
        GBxLogInfo("Cartridge Type: %s", type);
    }
    else {
        GBxLogInfo("Cartridge Type $%02X unknown", ctx->Header->CartridgeType);
    }

    type = GBx_Cartridge_GetROMTypeString(ctx->Header->ROMType);
    if (type) {
        GBxLogInfo("ROM Type: %s", type);
    }
    else {
        GBxLogInfo("ROM Type $%02X unknown", ctx->Header->ROMType);
    }

    type = GBx_Cartridge_GetSRAMTypeString(ctx->Header->SRAMType);
    if (type) {
        GBxLogInfo("SRAM Type: %s", type);
    }
    else {
        GBxLogInfo("SRAM Type $%02X unknown", ctx->Header->SRAMType);
    }
}

void GBx_Cartridge_PrintMBC(GBx * ctx)
{
    GBxLogInfo(
        "MBC: Lower=$%02X Upper=$%01X Full=$%04X Mode=%s",
        ctx->MBC.Lower,
        ctx->MBC.Upper,
        ctx->MBC.Full,
        (ctx->MBC.Mode ? "RAM" : "ROM"));
}