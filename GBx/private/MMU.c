#include <GBx/MMU.h>
#include <GBx/PPU.h>
#include <GBx/APU.h>
#include <GBx/Context.h>
#include <GBx/Joypad.h>
#include <GBx/Timer.h>
#include <GBx/Cartridge.h>

#include <string.h>

#include "Internal.h"
#include "Log.h"

void GBx_ResetMMU(gbx_t * ctx)
{
    ctx->WRAMBank = 1;
    for (unsigned i = 0; i < GBX_WRAM_BANK_COUNT; ++i) {
        memset(ctx->WRAM[i], 0, sizeof(ctx->WRAM[i]));
    }

    memset(ctx->HRAM, 0, sizeof(ctx->HRAM));
}

uint8_t GBx_ReadByte(gbx_t * ctx, uint16_t address)
{
    if (ctx->internal->MemoryTrackingEnabled) {
        ctx->internal->MemoryTracker[address].Read = 0xF;
    }

    switch (address & 0xF000) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
        // $0000-$00FF - BIOS / Jump Vectors
        if (address <= 0x00FF && ctx->BootstrapEnabled) {
            return ctx->Bootstrap[address];
        }

        // $0000-$3FFF - Cartridge ROM Bank 0
        return ctx->ROM[0][address];
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        // $4000-$7FFF - Switchable Cartridge ROM
        return ctx->ROM[ctx->ROMBank][address - 0x4000];
    case 0x8000:
    case 0x9000:
        // $8000-$9FFF - Video RAM
        return ctx->VRAM[ctx->VRAMBank][address - 0x8000];
    case 0xA000:
    case 0xB000:
        // $A000-$BFFF - External Cartridge RAM
        return ctx->SRAM[ctx->SRAMBank][address - 0xA000];
    case 0xC000:
        // $C000-$CFFF - Work RAM Bank 0
        return ctx->WRAM[0][address - 0xC000];
    case 0xD000:
        // $D000-$DFFF - Switchable Work RAM
        return ctx->WRAM[ctx->WRAMBank][address - 0xD000];
    case 0xE000:
    case 0xF000:
        // $E000-$FDFF - Echo RAM
        if (address <= 0xFDFF) {
            return ctx->WRAM[0][address - 0xE000];
        }
        // $FE00-$FE9F - Object Attribute Memory
        else if (address <= 0xFE9F) {
            return ctx->OAM[address - 0xFE00];
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
                return ctx->WaveRAM[address - 0xFF30];
            }

            switch (address) {
            case 0xFF00:
                return ctx->JOYP.raw;
            case 0xFF01:
                return ctx->SB;
            case 0xFF02:
                return ctx->SC.raw;
            case 0xFF04:
                return ctx->DIV;
            case 0xFF05:
                return ctx->TIMA;
            case 0xFF06:
                return ctx->TMA;
            case 0xFF07:
                return ctx->TAC.raw;
            case 0xFF0F:
                return ctx->CPU->IF.raw;
            case 0xFF10:
                return ctx->Tone1.raw[0] & GBX_TONE_READ_MASK0;
            case 0xFF11:
                return ctx->Tone1.raw[1] & GBX_TONE_READ_MASK1;
            case 0xFF12:
                return ctx->Tone1.raw[2] & GBX_TONE_READ_MASK2;
            case 0xFF13:
                return ctx->Tone1.raw[3] & GBX_TONE_READ_MASK3;
            case 0xFF14:
                return ctx->Tone1.raw[4] & GBX_TONE_READ_MASK4;
            case 0xFF16:
                return ctx->Tone2.raw[1] & GBX_TONE_READ_MASK1;
            case 0xFF17:
                return ctx->Tone2.raw[2] & GBX_TONE_READ_MASK2;
            case 0xFF18:
                return ctx->Tone2.raw[3] & GBX_TONE_READ_MASK3;
            case 0xFF19:
                return ctx->Tone2.raw[4] & GBX_TONE_READ_MASK4;
            case 0xFF20:
                return ctx->Noise.raw[0] & GBX_NOISE_READ_MASK0;
            case 0xFF21:
                return ctx->Noise.raw[1] & GBX_NOISE_READ_MASK1;
            case 0xFF22:
                return ctx->Noise.raw[2] & GBX_NOISE_READ_MASK2;
            case 0xFF23:
                return ctx->Noise.raw[3] & GBX_NOISE_READ_MASK3;
            case 0xFF24:
                return ctx->VolumeControl.raw;
            case 0xFF25:
                // return NR51;
            case 0xFF26:
                // return NR52;
            case 0xFF1A:
                return ctx->Wave.raw[0] & GBX_WAVE_READ_MASK0;
            case 0xFF1B:
                return ctx->Wave.raw[1] & GBX_WAVE_READ_MASK1;
            case 0xFF1C:
                return ctx->Wave.raw[2] & GBX_WAVE_READ_MASK2;
            case 0xFF1D:
                return ctx->Wave.raw[3] & GBX_WAVE_READ_MASK3;
            case 0xFF1E:
                return ctx->Wave.raw[4] & GBX_WAVE_READ_MASK4;
            case 0xFF40:
                return ctx->LCDC.raw;
            case 0xFF41:
                return ctx->STAT.raw;
            case 0xFF42:
                return ctx->SCY;
            case 0xFF43:
                return ctx->SCX;
            case 0xFF44:
                return ctx->LY;
            case 0xFF45:
                return ctx->LYC;
            case 0xFF47:
                return ctx->BGP.raw;
            case 0xFF48:
                return ctx->OBP0.raw;
            case 0xFF49:
                return ctx->OBP1.raw;
            case 0xFF4A:
                return ctx->WX;
            case 0xFF4B:
                return ctx->WY;
            default:
                LogWarn("Attempting to read from unimplemented hardware "
                        "register $%04X",
                    address);
                return 0;
            };
        }
        // $FF80-$FFFE - High RAM
        else if (address <= 0xFFFE) {
            return ctx->HRAM[address - 0xFF80];
        }
        // $FFFF - Interrupt Enable Flag
        else {
            return ctx->CPU->IE.raw;
        }
    default:
        break;
    }

    return 0;
}

void GBx_WriteByte(gbx_t * ctx, uint16_t address, uint8_t data)
{
    if (ctx->internal->MemoryTrackingEnabled) {
        ctx->internal->MemoryTracker[address].Write = 0xF;
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
        GBx_WriteByteMBC(ctx, address, data);
        return;
    case 0x8000:
    case 0x9000:
        // $8000-$9FFF - Video RAM
        ctx->VRAM[ctx->VRAMBank][address - 0x8000] = data;
        return;
    case 0xA000:
    case 0xB000:
        // $A000-$BFFF - External Cartridge RAM
        ctx->SRAM[ctx->SRAMBank][address - 0xA000] = data;
        return;
    case 0xC000:
        // $C000-$CFFF - Work RAM Bank 0
        ctx->WRAM[0][address - 0xC000] = data;
        return;
    case 0xD000:
        // $D000-$DFFF - Switchable Work RAM
        ctx->WRAM[ctx->WRAMBank][address - 0xD000] = data;
        return;
    case 0xE000:
    case 0xF000:
        // $E000-$FDFF - Echo RAM
        if (address <= 0xFDFF) {
            ctx->WRAM[0][address - 0xE000] = data;
            return;
        }
        // $FE00-$FE9F - Object Attribute Memory
        else if (address <= 0xFE9F) {
            ctx->OAM[address - 0xFE00] = data;
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
                ctx->WaveRAM[address - 0xFF30] = data;
                return;
            }

            switch (address) {
            case 0xFF00:
                ctx->JOYP.raw ^= GBX_JOYP_WRITE_MASK;
                ctx->JOYP.raw |= data & GBX_JOYP_WRITE_MASK;
                GBx_UpdateJoypad(ctx);
                Verbose(ctx, 2, GBx_PrintJoypad(ctx));
                return;
            case 0xFF01:
                ctx->SB = data;
                return;
            case 0xFF02:
                ctx->SC.raw = data;
                return;
            case 0xFF04:
                ctx->DIV = 0;
                Verbose(ctx, 2, LogInfo("Resetting DIV"));
                return;
            case 0xFF05:
                ctx->TIMA = data;
                Verbose(ctx, 2, GBx_PrintTimer(ctx));
                return;
            case 0xFF06:
                ctx->TMA = data;
                Verbose(ctx, 2, GBx_PrintTimer(ctx));
                return;
            case 0xFF07:
                ctx->TAC.raw = data;
                Verbose(ctx, 2, GBx_PrintTimer(ctx));
                return;
            case 0xFF0F:
                ctx->CPU->IF.raw = data;
                Verbose(ctx, 2, SM83_PrintInterrupts(ctx->CPU));
                return;
            case 0xFF10:
                ctx->Tone1.raw[0] = data;
                Verbose(ctx, 2, GBx_PrintTone1(ctx));
                return;
            case 0xFF11:
                ctx->Tone1.raw[1] = data;
                Verbose(ctx, 2, GBx_PrintTone1(ctx));
                return;
            case 0xFF12:
                ctx->Tone1.raw[2] = data;
                Verbose(ctx, 2, GBx_PrintTone1(ctx));
                return;
            case 0xFF13:
                ctx->Tone1.raw[3] = data;
                Verbose(ctx, 2, GBx_PrintTone1(ctx));
                return;
            case 0xFF14:
                ctx->Tone1.raw[4] = data;
                Verbose(ctx, 2, GBx_PrintTone1(ctx));
                return;
            case 0xFF16:
                ctx->Tone2.raw[1] = data;
                Verbose(ctx, 2, GBx_PrintTone2(ctx));
                return;
            case 0xFF17:
                ctx->Tone2.raw[2] = data;
                Verbose(ctx, 2, GBx_PrintTone2(ctx));
                return;
            case 0xFF18:
                ctx->Tone2.raw[3] = data;
                Verbose(ctx, 2, GBx_PrintTone2(ctx));
                return;
            case 0xFF19:
                ctx->Tone2.raw[4] = data;
                Verbose(ctx, 2, GBx_PrintTone2(ctx));
                return;
            case 0xFF20:
                ctx->Noise.raw[0] = data;
                Verbose(ctx, 2, GBx_PrintNoise(ctx));
                return;
            case 0xFF21:
                ctx->Noise.raw[1] = data;
                Verbose(ctx, 2, GBx_PrintNoise(ctx));
                return;
            case 0xFF22:
                ctx->Noise.raw[2] = data;
                Verbose(ctx, 2, GBx_PrintNoise(ctx));
                return;
            case 0xFF23:
                ctx->Noise.raw[3] = data;
                Verbose(ctx, 2, GBx_PrintNoise(ctx));
                return;
            case 0xFF24:
                ctx->VolumeControl.raw = data;
                Verbose(ctx, 2, GBx_PrintVolumeControl(ctx));
                return;
            case 0xFF25:
                ctx->SoundOutputTerminal.raw = data;
                // Verbose(ctx, 2, GBx_PrintSoundOutputTerminal(ctx));
                return;
            case 0xFF26:
                ctx->APUC.raw |= data & GBX_APUC_WRITE_MASK;
                // Verbose(ctx, 2, GBx_PrintAPUC(ctx));
                // TODO: Stop APU
                return;
            case 0xFF1A:
                ctx->Wave.raw[0] = data;
                Verbose(ctx, 2, GBx_PrintWave(ctx));
                break;
            case 0xFF1B:
                ctx->Wave.raw[1] = data;
                Verbose(ctx, 2, GBx_PrintWave(ctx));
                break;
            case 0xFF1C:
                ctx->Wave.raw[2] = data;
                Verbose(ctx, 2, GBx_PrintWave(ctx));
                break;
            case 0xFF1D:
                ctx->Wave.raw[3] = data;
                Verbose(ctx, 2, GBx_PrintWave(ctx));
                break;
            case 0xFF1E:
                ctx->Wave.raw[4] = data;
                Verbose(ctx, 2, GBx_PrintWave(ctx));
                break;
            case 0xFF40:
                ctx->LCDC.raw = data;
                Verbose(ctx, 2, GBx_PrintLCDC(ctx));
                break;
            case 0xFF41:
                ctx->STAT.raw ^= GBX_STAT_WRITE_MASK;
                ctx->STAT.raw |= data & GBX_STAT_WRITE_MASK;
                Verbose(ctx, 2, GBx_PrintSTAT(ctx));
                break;
            case 0xFF42:
                ctx->SCY = data;
                Verbose(ctx, 2, LogInfo("Setting SCY=%02X", data));
                break;
            case 0xFF43:
                ctx->SCX = data;
                Verbose(ctx, 2, LogInfo("Setting SCX=%02X", data));
                break;
            case 0xFF45:
                ctx->LYC = data;
                Verbose(ctx, 2, LogInfo("Setting LYC=%02X", data));
                break;
            case 0xFF46:
                GBx_DMATransferOAM(ctx, data);
                return;
            case 0xFF47:
                ctx->BGP.raw = data;
                Verbose(ctx, 2, GBx_PrintPalettes(ctx));
                break;
            case 0xFF48:
                ctx->OBP0.raw = data;
                Verbose(ctx, 2, GBx_PrintPalettes(ctx));
                return;
            case 0xFF49:
                ctx->OBP1.raw = data;
                Verbose(ctx, 2, GBx_PrintPalettes(ctx));
                return;
            case 0xFF4A:
                ctx->WX = data;
                Verbose(ctx, 2, LogInfo("Setting WX=%02X", data));
                return;
            case 0xFF4B:
                ctx->WY = data;
                Verbose(ctx, 2, LogInfo("Setting WY=%02X", data));
                return;
            case 0xFF50:
                ctx->BootstrapEnabled = false;
                Verbose(ctx, 2, LogInfo("Bootstrap ROM Disabled"));
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
            ctx->HRAM[address - 0xFF80] = data;
        }
        // Interrupt Enable Flag - FFFF
        else if (address == 0xFFFF) {
            ctx->CPU->IE.raw = data;
            Verbose(ctx, 2, SM83_PrintInterrupts(ctx->CPU));
        }
    }
}

uint16_t GBx_ReadWord(gbx_t * ctx, uint16_t address)
{
    return GBx_ReadByte(ctx, address)
        | (GBx_ReadByte(ctx, address + 1) << 8);
}

void GBx_WriteWord(gbx_t * ctx, uint16_t address, uint16_t data)
{
    GBx_WriteByte(ctx, address + 1, (uint8_t)(data >> 8));
    GBx_WriteByte(ctx, address, (uint8_t)(data & 0xFF));
}