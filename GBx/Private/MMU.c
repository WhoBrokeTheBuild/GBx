#include <GBx/MMU.h>
#include <GBx/Log.h>

uint8_t GBx_ReadByte(GBx * ctx, uint16_t address)
{
    // TODO: Memory Tracker

    switch (address & 0xF000) {
    case 0x0000:
        // $0000-$00FF Bootstrap/Jump Vectors
        if (ctx->BootstrapMapped && address <= 0x00FF) {
            return ctx->BootROM[address];
        }
    case 0x1000:
    case 0x2000:
    case 0x3000:
        // $0000-$3FFF Cartridge ROM Bank 0
        return ctx->ROM[0][address];
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        // $4000-$7FFF Cartridge ROM Switchable
        return ctx->ROM[ctx->ROMBank][address - 0x4000];
    case 0x8000:
    case 0x9000:
        // $8000-$9FFF Video RAM
        return ctx->VRAM[ctx->VRAMBank][address - 0x8000];
    case 0xA000:
    case 0xB000:
        // $A000-$BFFF Cartridge Static RAM
        return ctx->SRAM[ctx->SRAMBank][address - 0xA000];
    case 0xC000:
        // $C000-$CFFF Work RAM Bank 0
        return ctx->WRAM[0][address - 0xC000];
    case 0xD000:
        // $C000-$CFFF Work RAM Switchable
        return ctx->WRAM[ctx->WRAMBank][address - 0xC000];
    case 0xE000:
    case 0xF000:
        // $E000-$FDFF Echo RAM
        if (address <= 0xFDFF) {
            return ctx->WRAM[0][address - 0xE000];
        }

        // $FE00-$FE9F Object Attribute Memory
        if (address <= 0xFE9F) {
            return ctx->OAM[address - 0xFE00];
        }

        // $FEA0-$FEFF Unusable
        if (address <= 0xFEFF) {
            GBxOnce(GBxLogWarning(
                "Attempting to read from unusable memory at $%04X,"
                "this message will not repeat.", address));
            return 0;
        }


        // $FF00-$FE7F Hardware I/O Registers
        if (address <= 0xFF7F) {
            // $FF40-$FF3F Wave RAM
            if (address >= 0xFF30 && address <= 0xFF3F) {
                return ctx->WaveRAM[address - 0xFF30];
            }

            switch (address) {
            case 0xFF00:
                return 0x80;
                // return ctx->JOYP._raw;

            case 0xFF01:
                // return ctx->Serial.S8;

            case 0xFF02:
                // return ctx->Serial.SC._raw;

            case 0xFF04:
                // return ctx->DIV;

            case 0xFF05:
                // return ctx->TIMA;

            case 0xFF06:
                // return ctx->TMA;

            case 0xFF07:
                // return ctx->TAC._raw;

            // 0xFF08 - 0xFF0E

            case 0xFF0F:
                return ctx->IF._raw;
                break;

            // NR10
            case 0xFF10:
                return (ctx->Tone1._raw[0] | GBX_TONE_READ_MASK_0);

            // NR11
            case 0xFF11:
                return (ctx->Tone1._raw[1] | GBX_TONE_READ_MASK_1);

            // NR12
            case 0xFF12:
                return (ctx->Tone1._raw[2] | GBX_TONE_READ_MASK_2);

            // NR13
            case 0xFF13:
                return (ctx->Tone1._raw[3] | GBX_TONE_READ_MASK_3);

            // NR14
            case 0xFF14:
                return (ctx->Tone1._raw[4] | GBX_TONE_READ_MASK_4);

            // NR15
            // 0xFF15

            // NR21
            case 0xFF16:
                return (ctx->Tone2._raw[1] | GBX_TONE_READ_MASK_1);

            // NR22
            case 0xFF17:
                return (ctx->Tone2._raw[2] | GBX_TONE_READ_MASK_2);

            // NR23
            case 0xFF18:
                return (ctx->Tone2._raw[3] | GBX_TONE_READ_MASK_3);

            // NR24
            case 0xFF19:
                return (ctx->Tone2._raw[4] | GBX_TONE_READ_MASK_4);

            // NR30
            case 0xFF1A:
                return (ctx->Wave._raw[0] | GBX_WAVE_READ_MASK_0);

            // NR31
            case 0xFF1B:
                return (ctx->Wave._raw[1] | GBX_WAVE_READ_MASK_1);
                
            // NR32
            case 0xFF1C:
                return (ctx->Wave._raw[2] | GBX_WAVE_READ_MASK_2);
                
            // NR33
            case 0xFF1D:
                return (ctx->Wave._raw[3] | GBX_WAVE_READ_MASK_3);
                
            // NR34
            case 0xFF1E:
                return (ctx->Wave._raw[4] | GBX_WAVE_READ_MASK_4);

            // NR40
            // 0xFF1F

            // NR41
            case 0xFF20:
                return (ctx->Noise._raw[0] | GBX_NOISE_READ_MASK_0);

            // NR42
            case 0xFF21:
                return (ctx->Noise._raw[1] | GBX_NOISE_READ_MASK_1);

            // NR43
            case 0xFF22:
                return (ctx->Noise._raw[2] | GBX_NOISE_READ_MASK_2);

            // NR44
            case 0xFF23:
                return (ctx->Noise._raw[3] | GBX_NOISE_READ_MASK_3);

            // NR50
            case 0xFF24:
                return ctx->VolumeControl._raw;;

            // NR51
            case 0xFF25:
                return ctx->OutputTerminalControl._raw;

            // NR52
            case 0xFF26:
                return ctx->APUC._raw;

            default:
                return 0xFF;
            }
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
            return ctx->HRAM[address - 0xFF80];
        }

        // $FFFF
        return ctx->IE._raw;
    }

    return 0;
}

void GBx_WriteByte(GBx * ctx, uint16_t address, uint8_t data)
{
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
        ctx->VRAM[ctx->VRAMBank][address - 0x8000] = data;
        return;
    case 0xA000:
    case 0xB000:
        // $A000-$BFFF Cartridge Static RAM
        ctx->SRAM[ctx->SRAMBank][address - 0xA000] = data;
        return;
    case 0xC000:
        // $C000-$CFFF Work RAM Bank 0
        ctx->WRAM[0][address - 0xC000] = data;
        return;
    case 0xD000:
        // $D000-$DFFF Work RAM Switchable
        ctx->WRAM[ctx->WRAMBank][address - 0xD000] = data;
        return;
    case 0xE000:
    case 0xF000:
        // $E000-$FDFF Echo RAM
        if (address <= 0xFDFF) {
            ctx->WRAM[0][address - 0xE000] = data;
            return;
        }

        // $FE00-$FE9F Object Attribute Memory
        if (address <= 0xFE9F) {
            ctx->OAM[address - 0xFE00] = data;
            return;
        }

        // $FEA0-$FEFF Unusable
        if (address <= 0xFEFF) {
            GBxOnce(GBxLogWarning(
                "Attempting to write $%02X to unusable memory at $%04X,"
                "this message will not repeat.", data, address));
            return;
        }

        // $FF00-$FE7F Memory Mapped I/O Registers
        if (address <= 0xFF7F) {

            // $FF27-$FF2F Unused
            if (address >= 0xFF27 && address <= 0xFF2F) {
                return;
            }
            
            // $FF30-$FF3F Wave RAM
            if (address >= 0xFF30 && address <= 0xFF3F) {
                ctx->WaveRAM[address - 0xFF30] = data;
                return;
            }

            switch (address) {
            case 0xFF00:
                // return ctx->JOYP._raw;

            case 0xFF01:
                // return ctx->Serial.S8;

            case 0xFF02:
                // return ctx->Serial.SC._raw;

            case 0xFF04:
                // return ctx->DIV;

            case 0xFF05:
                // return ctx->TIMA;

            case 0xFF06:
                // return ctx->TMA;

            case 0xFF07:
                // return ctx->TAC._raw;

            // 0xFF08 - 0xFF0E

            case 0xFF0F:
                ctx->IF._raw = data;
                break;

            // NR10
            case 0xFF10:
                ctx->Tone1._raw[0] = data;
                return;

            // NR11
            case 0xFF11:
                ctx->Tone1._raw[1] = data;
                return;

            // NR12
            case 0xFF12:
                ctx->Tone1._raw[2] = data;
                return;

            // NR13
            case 0xFF13:
                ctx->Tone1._raw[3] = data;
                return;

            // NR14
            case 0xFF14:
                ctx->Tone1._raw[4] = data;
                return;

            // NR15
            // 0xFF15

            // NR21
            case 0xFF16:
                ctx->Tone2._raw[1] = data;
                return;

            // NR22
            case 0xFF17:
                ctx->Tone2._raw[2] = data;
                return;

            // NR23
            case 0xFF18:
                ctx->Tone2._raw[3] = data;
                return;

            // NR24
            case 0xFF19:
                ctx->Tone2._raw[4] = data;
                return;

            // NR30
            case 0xFF1A:
                ctx->Wave._raw[0] = data;
                return;

            // NR31
            case 0xFF1B:
                ctx->Wave._raw[1] = data;
                return;
                
            // NR32
            case 0xFF1C:
                ctx->Wave._raw[2] = data;
                return;
                
            // NR33
            case 0xFF1D:
                ctx->Wave._raw[3] = data;
                return;
                
            // NR34
            case 0xFF1E:
                ctx->Wave._raw[4] = data;
                return;

            // NR40
            // 0xFF1F

            // NR41
            case 0xFF20:
                ctx->Noise._raw[0] = data;
                return;

            // NR42
            case 0xFF21:
                ctx->Noise._raw[1] = data;
                return;

            // NR43
            case 0xFF22:
                ctx->Noise._raw[2] = data;
                return;

            // NR44
            case 0xFF23:
                ctx->Noise._raw[3] = data;
                return;

            // NR50
            case 0xFF24:
                ctx->VolumeControl._raw = data;
                return;

            // NR51
            case 0xFF25:
                ctx->OutputTerminalControl._raw = data;
                return;

            // NR52
            case 0xFF26:
                ctx->APUC._raw |= (data & GBX_APUC_WRITE_MASK);

            default:
                return;
            }
        }

        // $FF80-$FFFE
        if (address <= 0xFFFE) {
            ctx->HRAM[address - 0xFF80] = data;
            return;
        }

        // $FFFF Interrupt Enable Flag
        ctx->IE._raw = data;
    }
}
