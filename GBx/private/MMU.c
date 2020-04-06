#include <GBx/MMU.h>

#include <GBx/APU.h>
#include <GBx/Bootstrap.h>
#include <GBx/CPU.h>
#include <GBx/Cartridge.h>
#include <GBx/Debug.h>
#include <GBx/Joypad.h>
#include <GBx/Log.h>
#include <GBx/MBC.h>
#include <GBx/PPU.h>
#include <GBx/Serial.h>
#include <GBx/Timer.h>

#include <string.h>

uint8_t WRAM[WRAM_BANK_COUNT][WRAM_BANK_SIZE];

unsigned WRAMBank;

uint8_t HRAM[0x7F];

void ResetMMU()
{
    WRAMBank = 1;

    for (unsigned i = 0; i < WRAM_BANK_COUNT; ++i) {
        memset(WRAM[i], 0, sizeof(WRAM[i]));
    }

    memset(HRAM, 0, sizeof(HRAM));
}

uint8_t ReadByte(uint16_t address)
{
    if (MemoryTrackingEnabled) {
        MemoryTracker[address].Read = 0xF;
    }

    switch (address & 0xF000) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
        // $0000-$00FF - BIOS / Jump Vectors
        if (address <= 0x00FF && BootstrapROMEnabled) {
            return BootstrapROM[address];
        }

        // $0000-$3FFF - Cartridge ROM Bank 0
        return ROM[0][address];
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        // $4000-$7FFF - Switchable Cartridge ROM
        return ROM[ROMBank][address - 0x4000];
    case 0x8000:
    case 0x9000:
        // $8000-$9FFF - Video RAM
        return VRAM[VRAMBank][address - 0x8000];
    case 0xA000:
    case 0xB000:
        // $A000-$BFFF - External Cartridge RAM
        return SRAM[SRAMBank][address - 0xA000];
    case 0xC000:
        // $C000-$CFFF - Work RAM Bank 0
        return WRAM[0][address - 0xC000];
    case 0xD000:
        // $D000-$DFFF - Switchable Work RAM
        return WRAM[WRAMBank][address - 0xD000];
    case 0xE000:
    case 0xF000:
        // $E000-$FDFF - Echo RAM
        if (address <= 0xFDFF) {
            return WRAM[0][address - 0xE000];
        }
        // $FE00-$FE9F - Object Attribute Memory
        else if (address <= 0xFE9F) {
            return OAM[address - 0xFE00];
        }
        // $FEA0-$FEFF - Unusable
        else if (address <= 0xFEFF) {
            // LogWarn("Attempt to read from unusable uemory $FEA0-$FEFF at
            // $%04X", address);
            return 0;
        }
        // $FF00-$FE7F - Hardware I/O Registers
        else if (address <= 0xFF7F) {
            if (address >= 0xFF30 && address <= 0xFF3F) {
                return WaveRAM[address - 0xFF30];
            }

            switch (address) {
            case 0xFF00:
                return JOYP.raw;
            case 0xFF01:
                return SB;
            case 0xFF02:
                return SC.raw;
            case 0xFF04:
                return DIV;
            case 0xFF05:
                return TIMA;
            case 0xFF06:
                return TMA;
            case 0xFF07:
                return TAC.raw;
            case 0xFF0F:
                return CPU.IF.raw;
            case 0xFF10:
                return Tone1.raw[0] & TONE_READ_MASK0;
            case 0xFF11:
                return Tone1.raw[1] & TONE_READ_MASK1;
            case 0xFF12:
                return Tone1.raw[2] & TONE_READ_MASK2;
            case 0xFF13:
                return Tone1.raw[3] & TONE_READ_MASK3;
            case 0xFF14:
                return Tone1.raw[4] & TONE_READ_MASK4;
            case 0xFF16:
                return Tone2.raw[1] & TONE_READ_MASK1;
            case 0xFF17:
                return Tone2.raw[2] & TONE_READ_MASK2;
            case 0xFF18:
                return Tone2.raw[3] & TONE_READ_MASK3;
            case 0xFF19:
                return Tone2.raw[4] & TONE_READ_MASK4;
            case 0xFF20:
                return Noise.raw[0] & NOISE_READ_MASK0;
            case 0xFF21:
                return Noise.raw[1] & NOISE_READ_MASK1;
            case 0xFF22:
                return Noise.raw[2] & NOISE_READ_MASK2;
            case 0xFF23:
                return Noise.raw[3] & NOISE_READ_MASK3;
            case 0xFF24:
                return VolumeControl.raw;
            case 0xFF25:
                // return NR51;
            case 0xFF26:
                // return NR52;
            case 0xFF1A:
                return Wave.raw[0] & WAVE_READ_MASK0;
            case 0xFF1B:
                return Wave.raw[1] & WAVE_READ_MASK1;
            case 0xFF1C:
                return Wave.raw[2] & WAVE_READ_MASK2;
            case 0xFF1D:
                return Wave.raw[3] & WAVE_READ_MASK3;
            case 0xFF1E:
                return Wave.raw[4] & WAVE_READ_MASK4;
            case 0xFF40:
                return LCDC.raw;
            case 0xFF41:
                return STAT.raw;
            case 0xFF42:
                return SCY;
            case 0xFF43:
                return SCX;
            case 0xFF44:
                return LY;
            case 0xFF45:
                return LYC;
            case 0xFF47:
                return BGP.raw;
            case 0xFF48:
                return OBP0.raw;
            case 0xFF49:
                return OBP1.raw;
            case 0xFF4A:
                return WX;
            case 0xFF4B:
                return WY;
            default:
                LogWarn("Attempting to read from unimplemented hardware "
                        "register $%04X",
                    address);
                return 0;
            };
        }
        // $FF80-$FFFE - High RAM
        else if (address <= 0xFFFE) {
            return HRAM[address - 0xFF80];
        }
        // $FFFF - Interrupt Enable Flag
        else {
            return CPU.IE.raw;
        }
    default:
        break;
    }

    return 0;
}

uint8_t NextByte()
{
    uint8_t uint8_t = ReadByte(CPU.PC);
    ++CPU.PC;
    return uint8_t;
}

uint16_t NextWord()
{
    uint16_t uint16_t = ReadWord(CPU.PC);
    CPU.PC += 2;
    return uint16_t;
}

void WriteByte(uint16_t address, uint8_t data)
{
    if (MemoryTrackingEnabled) {
        MemoryTracker[address].Write = 0xF;
    }

    switch (address & 0xF000) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        // $0000-$7FFF - Cartridge Memory Bank Controller
        WriteMBC(address, data);
        return;
    case 0x8000:
    case 0x9000:
        // $8000-$9FFF - Video RAM
        VRAM[VRAMBank][address - 0x8000] = data;
        return;
    case 0xA000:
    case 0xB000:
        // $A000-$BFFF - External Cartridge RAM
        SRAM[SRAMBank][address - 0xA000] = data;
        return;
    case 0xC000:
        // $C000-$CFFF - Work RAM Bank 0
        WRAM[0][address - 0xC000] = data;
        return;
    case 0xD000:
        // $D000-$DFFF - Switchable Work RAM
        WRAM[WRAMBank][address - 0xD000] = data;
        return;
    case 0xE000:
    case 0xF000:
        // $E000-$FDFF - Echo RAM
        if (address <= 0xFDFF) {
            WRAM[0][address - 0xE000] = data;
            return;
        }
        // $FE00-$FE9F - Object Attribute Memory
        else if (address <= 0xFE9F) {
            OAM[address - 0xFE00] = data;
            return;
        }
        // $FEA0-$FEFF - Unusable
        else if (address <= 0xFEFF) {
            // LogWarn("Attempt to write to unusable uemory $FEA0-$FEFF at
            // $%04X", address);
            return;
        }
        // $FF00-$FE7F - Hardware I/O Registers
        else if (address <= 0xFF7F) {
            if (address >= 0xFF30 && address <= 0xFF3F) {
                WaveRAM[address - 0xFF30] = data;
                return;
            }

            switch (address) {
            case 0xFF00:
                JOYP.raw |= data & JOYP_WRITE_MASK;
                UpdateJoypad();
                if (VerboseLevel >= 2) {
                    PrintJoypad();
                }
                return;
            case 0xFF01:
                SB = data;
                return;
            case 0xFF02:
                SC.raw = data;
                return;
            case 0xFF04:
                DIV = 0;
                LogVerbose(2, "Resetting DIV");
                return;
            case 0xFF05:
                TIMA = data;
                if (VerboseLevel >= 2) {
                    PrintTimer();
                }
                return;
            case 0xFF06:
                TMA = data;
                if (VerboseLevel >= 2) {
                    PrintTimer();
                }
                return;
            case 0xFF07:
                TAC.raw = data;
                if (VerboseLevel >= 2) {
                    PrintTimer();
                }
                return;
            case 0xFF0F:
                CPU.IF.raw = data;
                if (VerboseLevel >= 2) {
                    SM83_PrintInterrupts(&CPU);
                }
                return;
            case 0xFF10:
                Tone1.raw[0] = data;
                if (VerboseLevel >= 2) {
                    PrintTone1();
                }
                return;
            case 0xFF11:
                Tone1.raw[1] = data;
                if (VerboseLevel >= 2) {
                    PrintTone1();
                }
                return;
            case 0xFF12:
                Tone1.raw[2] = data;
                if (VerboseLevel >= 2) {
                    PrintTone1();
                }
                return;
            case 0xFF13:
                Tone1.raw[3] = data;
                if (VerboseLevel >= 2) {
                    PrintTone1();
                }
                return;
            case 0xFF14:
                Tone1.raw[4] = data;
                if (VerboseLevel >= 2) {
                    PrintTone1();
                }
                return;
            case 0xFF16:
                Tone2.raw[1] = data;
                if (VerboseLevel >= 2) {
                    PrintTone2();
                }
                return;
            case 0xFF17:
                Tone2.raw[2] = data;
                if (VerboseLevel >= 2) {
                    PrintTone2();
                }
                return;
            case 0xFF18:
                Tone2.raw[3] = data;
                if (VerboseLevel >= 2) {
                    PrintTone2();
                }
                return;
            case 0xFF19:
                Tone2.raw[4] = data;
                if (VerboseLevel >= 2) {
                    PrintTone2();
                }
                return;
            case 0xFF20:
                Noise.raw[0] = data;
                if (VerboseLevel >= 2) {
                    PrintNoise();
                }
                return;
            case 0xFF21:
                Noise.raw[1] = data;
                if (VerboseLevel >= 2) {
                    PrintNoise();
                }
                return;
            case 0xFF22:
                Noise.raw[2] = data;
                if (VerboseLevel >= 2) {
                    PrintNoise();
                }
                return;
            case 0xFF23:
                Noise.raw[3] = data;
                if (VerboseLevel >= 2) {
                    PrintNoise();
                }
                return;
            case 0xFF24:
                VolumeControl.raw = data;
                if (VerboseLevel >= 2) {
                    PrintVolumeControl();
                }
                return;
            case 0xFF25:
                SoundOutputTerminal.raw = data;
                if (VerboseLevel >= 2) {
                    // PrintSoundOutputTerminal();
                }
                return;
            case 0xFF26:
                APUC.raw |= data & SOUND_CONTROL_WRITE_MASK;
                // TODO: Stop APU
                if (VerboseLevel >= 2) {
                    // PrintAPUC();
                }
                return;
            case 0xFF1A:
                Wave.raw[0] = data;
                if (VerboseLevel >= 2) {
                    PrintWave();
                }
                break;
            case 0xFF1B:
                Wave.raw[1] = data;
                if (VerboseLevel >= 2) {
                    PrintWave();
                }
                break;
            case 0xFF1C:
                Wave.raw[2] = data;
                if (VerboseLevel >= 2) {
                    PrintWave();
                }
                break;
            case 0xFF1D:
                Wave.raw[3] = data;
                if (VerboseLevel >= 2) {
                    PrintWave();
                }
                break;
            case 0xFF1E:
                Wave.raw[4] = data;
                if (VerboseLevel >= 2) {
                    PrintWave();
                }
                break;

            case 0xFF40:
                LCDC.raw = data;
                if (VerboseLevel >= 2) {
                    PrintLCDC();
                }
                break;
            case 0xFF41:
                STAT.raw ^= STAT_WRITE_MASK;
                STAT.raw |= data & STAT_WRITE_MASK;
                if (VerboseLevel >= 2) {
                    PrintSTAT();
                }
                break;
            case 0xFF42:
                SCY = data;
                LogVerbose(2, "Setting SCY=%02X", data);
                break;
            case 0xFF43:
                SCX = data;
                LogVerbose(2, "Setting SCX=%02X", data);
                break;
            case 0xFF45:
                LYC = data;
                LogVerbose(2, "Setting LYC=%02X", data);
                break;
            case 0xFF46:
                OAMDMATransfer(data);
                return;
            case 0xFF47:
                BGP.raw = data;
                if (VerboseLevel >= 2) {
                    PrintPalette("BGP", &BGP);
                }
                break;
            case 0xFF48:
                OBP0.raw = data;
                if (VerboseLevel >= 2) {
                    PrintPalette("OBP0", &OBP0);
                }
                return;
            case 0xFF49:
                OBP1.raw = data;
                if (VerboseLevel >= 2) {
                    PrintPalette("OBP1", &OBP1);
                }
                return;
            case 0xFF4A:
                WX = data;
                LogVerbose(2, "Setting WX=%02X", data);
                return;
            case 0xFF4B:
                WY = data;
                LogVerbose(2, "Setting WY=%02X", data);
                return;
            case 0xFF50:
                BootstrapROMEnabled = false;
                LogVerbose(2, "Bootstrap ROM Disabled");
                return;
            default:
                LogWarn("Attempting to write to unimplemented hardware "
                        "register $%04X",
                    address);
                return;
            };
        }
        // High RAM - FF80-FFFE
        else if (address <= 0xFFFE) {
            HRAM[address - 0xFF80] = data;
        }
        // Interrupt Enable Flag - FFFF
        else if (address == 0xFFFF) {
            CPU.IE.raw = data;
            if (VerboseLevel >= 2) {
                SM83_PrintInterrupts(&CPU);
            }
        }
    }
}

void WriteWord(uint16_t address, uint16_t data)
{
    WriteByte(address + 1, (uint8_t)(data >> 8));
    WriteByte(address, (uint8_t)(data & 0xFF));
}

void PushWord(uint16_t data)
{
    CPU.SP -= 2;
    WriteWord(CPU.SP, data);
}

uint16_t PopWord()
{
    uint16_t data = ReadWord(CPU.SP);
    CPU.SP += 2;
    return data;
}
