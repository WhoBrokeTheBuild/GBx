#include <GBx/MMU.h>

#include "Context.h"

uint8_t GBx_ReadByte(GBx * ctx, uint16_t address)
{
    GBx_APU * apu = &ctx->APU;
    GBx_CPU * cpu = &ctx->CPU;
    GBx_PPU * ppu = &ctx->PPU;
    GBx_Cartridge * cart = &ctx->Cartridge;
    GBx_Bootstrap * boot = &ctx->Bootstrap;

    // TODO: Memory Tracker

    switch (address & 0xF000) {
    case 0x0000:
        // $0000-$00FF Bootstrap/Jump Vectors
        if (address <= 0x00FF && boot->Enabled) {
            return boot->ROM[address];
        }
    case 0x1000:
    case 0x2000:
    case 0x3000:
        // $0000-$3FFF Cartridge ROM Bank 0
        return cart->ROM[0][address];
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        // $4000-$7FFF Cartridge ROM Switchable
        return cart->ROM[cart->ROMBank][address - 0x4000];
    case 0x8000:
    case 0x9000:
        // $8000-$9FFF Video RAM
        return ppu->VRAM[ppu->VRAMBank][address - 0x8000];
    case 0xA000:
    case 0xB000:
        // $A000-$BFFF Cartridge Static RAM
        return cart->SRAM[cart->SRAMBank][address - 0xA000];
    case 0xC000:
        // $C000-$CFFF Work RAM Bank 0
        return cpu->WRAM[0][address - 0xC000];
    case 0xD000:
        // $C000-$CFFF Work RAM Switchable
        return cpu->WRAM[cpu->WRAMBank][address - 0xC000];
    case 0xE000:
    case 0xF000:
        // $E000-$FDFF Echo RAM
        if (address <= 0xFDFF) {
            return cpu->WRAM[0][address - 0xE000];
        }

        // $FE00-$FE9F Object Attribute Memory
        if (address <= 0xFE9F) {
            return ppu->OAM[address - 0xFE00];
        }

        // $FEA0-$FEFF Unusable
        if (address <= 0xFEFF) {
            GBxOnce(GBxLogWarning(
                "Attempting to read from unusable memory at $%04X,"
                "this message will not repeat.", address));
            return 0;
        }

        // $FF40-$FF3F
        if (address >= 0xFF30 && address <= 0xFF3F) {
            return apu->WaveRAM[address - 0xFF30];
        }

        // $FF00-$FE7F Memory Mapped I/O Registers
        if (address <= 0xFE7F) {
            switch (address) {
            default:
                GBxLogFatal("Implement MMIO");
                break;
            }
        }

        // $FF80-$FFFE High RAM / Zero Page
        if (address <= 0xFFFE) {
            return cpu->HRAM[address - 0xFF80];
        }

        // $FFFF
        return cpu->IE._raw;
    }

    return 0;
}

void GBx_WriteByte(GBx * ctx, uint16_t address, uint8_t data)
{
    GBx_APU * apu = &ctx->APU;
    GBx_CPU * cpu = &ctx->CPU;
    GBx_PPU * ppu = &ctx->PPU;
    GBx_Cartridge * cart = &ctx->Cartridge;
    GBx_Bootstrap * boot = &ctx->Bootstrap;

    // TODO: Memory Tracker

    switch (address & 0xF000) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        // $0000-$7FFF Cartridge Memory Bank Controller
        GBx_Cartridge_MBCWriteByte(ctx, address, data);
        return;
    case 0x8000:
    case 0x9000:
        // $8000-$9FFF Video RAM
        ppu->VRAM[ppu->VRAMBank][address - 0x8000] = data;
        return;
    case 0xA000:
    case 0xB000:
        // $A000-$BFFF Cartridge Static RAM
        cart->SRAM[cart->SRAMBank][address - 0xA000] = data;
        return;
    case 0xC000:
        // $C000-$CFFF Work RAM Bank 0
        cpu->WRAM[0][address - 0xC000] = data;
        return;
    case 0xD000:
        // $D000-$DFFF Work RAM Switchable
        cpu->WRAM[cpu->WRAMBank][address - 0xD000] = data;
        return;
    case 0xE000:
    case 0xF000:
        // $E000-$FDFF Echo RAM
        if (address <= 0xFDFF) {
            cpu->WRAM[0][address - 0xE000] = data;
            return;
        }

        // $FE00-$FE9F Object Attribute Memory
        if (address <= 0xFE9F) {
            ppu->OAM[address - 0xFE00] = data;
            return;
        }

        // $FEA0-$FEFF Unusable
        if (address <= 0xFEFF) {
            GBxOnce(GBxLogWarning(
                "Attempting to write $%02X to unusable memory at $%04X,"
                "this message will not repeat.", data, address));
            return;
        }

        // $FF30-$FF3F Wave RAM
        if (address >= 0xFF30 && address <= 0xFF3F) {
            apu->WaveRAM[address - 0xFF30] = data;
            return;
        }

        // $FF00-$FE7F Memory Mapped I/O Registers
        if (address <= 0xFF7F) {
            switch (address) {
            default:
                GBxLogFatal("Implement MMIO");
                break;
            }
        }

        // $FF80-$FFFE
        if (address <= 0xFFFE) {
            cpu->HRAM[address - 0xFF80] = data;
            return;
        }

        // $FFFF Interrupt Enable Flag
        cpu->IE._raw = data;
    }
}
