#include <GBx/MBC.h>

#include <GBx/Cartridge.h>
#include <GBx/Log.h>
#include <GBx/Util.h>

#include <string.h>

mbc_type_t MBCType;

mbc_bank_control_t MBCBankControl;

void ResetMBC()
{
    MBCBankControl.raw = 0x00;

    SRAMEnabled = false;
    SRAMBank = 0;
    ROMBank = 1;

    for (unsigned i = 0; i < SRAM_BANK_COUNT; ++i) {
        memset(SRAM[i], 0, sizeof(SRAM[i]));
    }
}

void WriteMBC(word address, byte data)
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
            
            MBCBankControl.Lower = data;
            ROMBank = MBCBankControl.Full;
        }
        else if (address <= 0x5FFF) {
            // RAM Bank Number or ROM Upper Bank Number
            MBCBankControl.Upper = data;
            if (MBCBankControl.RAMMode) {
                SRAMBank = MBCBankControl.Upper;
            }
            else {
                ROMBank = MBCBankControl.Full;
            }
        }
        else {
            // ROM/RAM Mode Select
            MBCBankControl.RAMMode = (data == 1);
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
            
            MBCBankControl.Full = data;
            ROMBank = MBCBankControl.Full;
        }
        else if (address <= 0x5FFF) {
            
        }
    }
}

void PrintMBC()
{
    LogInfo("MBC: Lower=%02X Upper=%01X Full=%04X Mode=%s", 
        MBCBankControl.Lower, 
        MBCBankControl.Upper, 
        MBCBankControl.Full, 
        (MBCBankControl.RAMMode ? "RAM" : "ROM"));
}