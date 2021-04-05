#include <GBx/Cartridge.h>
#include <GBx/Context.h>
#include <GBx/Log.h>

#include <stdio.h>
#include <string.h>

void GBx_Cartridge_Init(GBx * ctx)
{
    ctx->Header = (GBx_CartridgeHeader *)&ctx->ROM[0][GBX_ROM_HEADER_OFFSET];

    memset(ctx->ROM, 0, sizeof(ctx->ROM));

    ctx->MBCType = GBX_MBC_TYPE_NONE;

    GBx_Cartridge_Reset(ctx);
}

void GBx_Cartridge_Reset(GBx * ctx)
{
    ctx->SRAMEnabled = false;

    ctx->MBC._raw = 0x00;

    ctx->ROMBank = 1;

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

const char * GBx_Cartridge_GetTypeString(uint8_t type)
{

    switch (type) {
    case 0x00:
        return "ROM Only";
    case 0x01:
        return "MBC1";
    case 0x02:
        return "MBC1,SRAM";
    case 0x03:
        return "MBC1,SRAM,Battery";
    case 0x05:
        return "MBC2";
    case 0x06:
        return "MBC2,Battery";
    case 0x08:
        return "SRAM";
    case 0x09:
        return "SRAM,Battery";
    case 0x0B:
        return "MMM01";
    case 0x0C:
        return "MMM01,SRAM";
    case 0x0D:
        return "MMM01,SRAM,Battery";
    case 0x0F:
        return "MBC3,RTC,Battery";
    case 0x10:
        return "MBC3,RTC,SRAM,Battery";
    case 0x11:
        return "MBC3";
    case 0x12:
        return "MBC3,SRAM";
    case 0x13:
        return "MBC3,SRAM,Battery";
    case 0x19:
        return "MBC5";
    case 0x1A:
        return "MBC5,SRAM";
    case 0x1B:
        return "MBC5,SRAM,Battery";
    case 0x1C:
        return "MBC5,Rumble";
    case 0x1D:
        return "MBC5,Rumble,SRAM";
    case 0x1E:
        return "MBC5,Rumble,SRAM,Battery";
    case 0x20:
        return "MBC6";
    case 0x22:
        return "MBC7,Sensor,Rumble,SRAM,Battery";
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
