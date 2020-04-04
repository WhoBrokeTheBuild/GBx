#include <GBx/MBC.h>

#include <GBx/Cartridge.h>
#include <GBx/Log.h>
#include <GBx/Util.h>

#include <string.h>

mbc_type_t MBCType;

mbc_bank_control_t MBC;

void ResetMBC()
{
    MBC.raw = 0x00;

    SRAMEnabled = false;
    SRAMBank    = 0;
    ROMBank     = 1;

    for (unsigned i = 0; i < SRAM_BANK_COUNT; ++i) {
        memset(SRAM[i], 0, sizeof(SRAM[i]));
    }
}

void WriteMBC(uint16_t address, uint8_t data)
{
    if (MBCType == MBC_TYPE_MBC1) {
        if (address <= 0x1FFF) {
            // RAM Enable
            SRAMEnabled = ((data & 0x0F) == 0x0A);
            LogInfo("SRAM: %s", GetEnabledString(SRAMEnabled));
        }
        else if (address <= 0x3FFF) {
            // Cartridge ROM Bank Number
            if (data == 0) {
                data = 1;
            }

            MBC.Lower = data;
            ROMBank   = MBC.Full;
        }
        else if (address <= 0x5FFF) {
            // RAM Bank Number or ROM Upper Bank Number
            MBC.Upper = data;
            if (MBC.RAMMode) {
                SRAMBank = MBC.Upper;
            }
            else {
                ROMBank = MBC.Full;
            }
        }
        else {
            // ROM/RAM Mode Select
            MBC.RAMMode = (data == 1);
        }
    }
    else if (MBCType == MBC_TYPE_MBC2) {
        // Crazy
    }
    else if (MBCType == MBC_TYPE_MBC3) {
        if (address <= 0x1FFF) {
            // RAM Enable
            SRAMEnabled = ((data & 0x0F) == 0x0A);
        }
        else if (address <= 0x3FFF) {
            // Cartridge ROM Bank Number
            if (data == 0) {
                data = 1;
            }

            MBC.Full = data;
            ROMBank  = MBC.Full;
        }
        else if (address <= 0x5FFF) {
        }
    }
}

void PrintMBC()
{
    LogInfo("MBC: Lower=%02X Upper=%01X Full=%04X Mode=%s",
        MBC.Lower,
        MBC.Upper,
        MBC.Full,
        (MBC.RAMMode ? "RAM" : "ROM"));
}