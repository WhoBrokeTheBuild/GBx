#include <GBx/MBC.h>
#include <GBx/Context.h>
#include <GBx/Log.h>

void GBx_MBC_Reset(GBx * ctx)
{
    ctx->MBCType = GBX_MBC_TYPE_NONE;

    switch (ctx->Header->CartridgeType) {
    case 0x00: // ROM Only
    case 0x08: // SRAM
    case 0x09: // SRAM, Battery
        break;

    case 0x01: // MBC1
    case 0x02: // MBC1, SRAM
    case 0x03: // MBC1, SRAM, Battery
        ctx->MBCType = GBX_MBC_TYPE_MBC1;
        break;

    case 0x05: // MBC2
    case 0x06: // MBC2, Battery
        ctx->MBCType = GBX_MBC_TYPE_MBC2;
        break;

    case 0x0B: // MMM01
    case 0x0C: // MMM01, SRAM
    case 0x0D: // MMM01, SRAM, Battery
        ctx->MBCType = GBX_MBC_TYPE_MMM01;
        break;

    case 0x0F: // MBC3, RTC, Battery
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        break;
    case 0x10: // MBC3, RTC, SRAM, Battery
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        break;
    case 0x11: // MBC3
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        break;
    case 0x12: // MBC3, SRAM
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        break;
    case 0x13: // MBC3, SRAM, Battery
        ctx->MBCType = GBX_MBC_TYPE_MBC3;
        break;
    case 0x19: // MBC5
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1A: // MBC5, SRAM
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1B: // MBC5, SRAM, Battery
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1C: // MBC5, Rumble
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1D: // MBC5, Rumble, SRAM
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    case 0x1E: // MBC5, Rumble, SRAM, Battery
        ctx->MBCType = GBX_MBC_TYPE_MBC5;
        break;
    // case 0x20: // MBC6
    //     ctx->MBCType = GBX_MBC_TYPE_MBC6;
    //     break;
    // case 0x22: // MBC7, Sensor, Rumble, SRAM, Battery
    //     ctx->MBCType = GBX_MBC_TYPE_MBC7;
    //     break;
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
}

void GBx_MBC_WriteByte(GBx * ctx, uint16_t address, uint8_t data)
{
    switch (ctx->MBCType) {
    case GBX_MBC_TYPE_MBC1:
        if (address <= 0x1FFF) {
            // $0000-$1FFF SRAM Enable/Disable
            ctx->SRAMEnabled = ((data & 0x0F) == 0x0A);
        }
        else if (address <= 0x3FFF) {
            // $2000-$3FFF ROM Lower Bank Number
            if (data == 0) {
                data = 1;
            }

            ctx->MBC.Lower = data;
            ctx->ROMBank = ctx->MBC.Full;
        }
        else if (address <= 0x5FFF) {
            // $4000-$5FFF SRAM Bank Number / ROM Upper Bank Number
            ctx->MBC.Upper = data;
            if (ctx->MBC.Mode) {
                ctx->SRAMBank = ctx->MBC.Upper;
            }
            else {
                ctx->ROMBank = ctx->MBC.Full;
            }
        }
        else {
            // $6000-$7FFF ROM/SRAM Mode Select
            ctx->MBC.Mode = (data == 1);
        }
        break;
    case GBX_MBC_TYPE_MBC2:
        // Crazy
        break;
    case GBX_MBC_TYPE_MBC3:
        if (address <= 0x1FFF) {
            // $0000-$1FFF SRAM Enable/Disable
            ctx->SRAMEnabled = ((data & 0x0F) == 0x0A);
            // ctx->RTCEnabled = ctx->SRAMEnabled;
        }
        else if (address <= 0x3FFF) {
            // $2000-$3FFF ROM Lower Bank Number
            if (data == 0) {
                data = 1;
            }

            ctx->MBC.Lower = data;
            ctx->ROMBank = ctx->MBC.Full;
        }
        else if (address <= 0x5FFF) {
            // $4000-$5FFF 

        }
        break;
    case GBX_MBC_TYPE_MBC5:
        if (address <= 0x1FFF) {
            // $0000-$1FFF SRAM Enable/Disable
            ctx->SRAMEnabled = ((data & 0x0F) == 0x0A);
        }
        else if (address <= 0x3FFF) {
            // $2000-$3FFF ROM Lower Bank Number
            ctx->MBC.Lower = data;
            ctx->ROMBank = ctx->MBC.Full;
        }
        else if (address <= 0x5FFF) {
            // $4000-$5FFF SRAM Bank Number / ROM Upper Bank Number
            ctx->MBC.Upper = data;
            if (ctx->MBC.Mode) {
                ctx->SRAMBank = ctx->MBC.Upper;
            }
            else {
                ctx->ROMBank = ctx->MBC.Full;
            }
        }
        else {
            // $6000-$7FFF ROM/SRAM Mode Select
            ctx->MBC.Mode = (data == 1);
        }
    case GBX_MBC_TYPE_MMM01:

    default:
        break;
    }
}

void GBx_Cartridge_PrintMBC(GBx * ctx)
{
    GBxLogInfo(
        "MBC: Lower=$%02X Upper=$%01X Full=$%04X Mode=%s",
        ctx->MBC.Lower,
        ctx->MBC.Upper,
        ctx->MBC.Full,
        (ctx->MBC.Mode ? "SRAM" : "ROM"));
}