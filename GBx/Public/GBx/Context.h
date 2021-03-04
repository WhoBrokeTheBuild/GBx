#ifndef GBX_CONTEXT_H
#define GBX_CONTEXT_H

#include <GBx/GBx.h>
#include <GBx/Constants.h>

#include <GBx/Types/APU.h>
#include <GBx/Types/Cartridge.h>
#include <GBx/Types/CPU.h>
#include <GBx/Types/Interrupt.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum GBx_Mode
{
    GBX_MODE_DMG, // Original GameBoy
    GBX_MODE_SGB, // Super GameBoy
    GBX_MODE_CGB, // Color GameBoy
    
} GBx_Mode;

struct GBx
{
    ///
    /// General
    ///

    GBx_Mode Mode;

    // Output Volume, 0.0 - 1.0
    float Volume;

    bool CPUEnabled;

    bool PPUEnabled;

    bool SRAMEnabled;

    bool BootstrapMapped;

    bool HaveBootstrap;

    bool HaveBootstrapCGB;

    ///
    /// Audio
    ///

    // NR10-NR14 Tone and Sweep
    GBx_ToneControl Tone1;

    // NR21-NR24 Tone
    GBx_ToneControl Tone2;

    // NR30-NR34 Wave
    GBx_WaveControl Wave;

    // NR41-NR44 Noise
    GBx_NoiseControl Noise;

    // NR50
    GBx_VolumeControl VolumeControl;

    // NR51
    GBx_OutputTerminalControl OutputTerminalControl;

    // NR52
    GBx_APUControl APUC;

    ///
    /// Cartridge
    ///

    GBx_CartridgeHeader * Header;

    // Memory Bank Controller Config
    GBx_MBC MBC;

    // Memory Bank Controller Type
    GBx_MBCType MBCType;

    bool HaveSRAM;

    bool HaveBattery;

    bool HaveTimer;

    ///
    /// Registers
    ///

    union {
        struct {
            GBx_Flags F;
            uint8_t A;
        };

        uint16_t AF;
    };

    union {
        struct {
            uint8_t C;
            uint8_t B;
        };

        uint16_t BC;
    };

    union {
        struct {
            uint8_t E;
            uint8_t D;
        };

        uint16_t DE;
    };

    union {
        struct {
            uint8_t L;
            uint8_t H;
        };

        uint16_t HL;
    };

    // Stack Pointer
    uint16_t SP;

    // Program Counter
    uint16_t PC;

    // Interrupt Master Enable Flag
    bool IME;

    // Enable IME after the next instruction
    bool RequestEnableIME;

    // Interrupt Request Flags
    GBx_InterruptFlags IF;

    // Interrupt Enable Flags
    GBx_InterruptFlags IE;

    ///
    /// Memory
    ///

    // Bootstrap / BIOS ROM
    uint8_t BootROM[GBX_BOOT_ROM_SIZE];

    // Cartridge ROM
    uint8_t ROM[GBX_ROM_BANK_COUNT][GBX_ROM_BANK_SIZE];
    unsigned ROMBank;

    // Video RAM
    uint8_t VRAM[GBX_VRAM_BANK_COUNT][GBX_VRAM_BANK_SIZE];
    unsigned VRAMBank;

    // Static RAM
    uint8_t SRAM[GBX_SRAM_BANK_COUNT][GBX_SRAM_BANK_SIZE];
    unsigned SRAMBank;

    // Work RAM
    uint8_t WRAM[GBX_WRAM_BANK_COUNT][GBX_WRAM_BANK_SIZE];
    unsigned WRAMBank;

    // Object Attribute Memory
    uint8_t OAM[GBX_OAM_SIZE];

    // PCM Waveform Data
    uint8_t WaveRAM[GBX_WAVE_RAM_SIZE];

    // Raw Pixel Data
    uint8_t Pixels[GBX_SCREEN_SIZE];

    // High RAM / Zero Page
    uint8_t HRAM[GBX_HRAM_SIZE];

    ///
    /// Debug
    ///

    uint16_t LastInstructionAddress;

};

typedef struct GBx GBx;

#ifdef __cplusplus
}
#endif

#endif // GBX_CONTEXT_H