#ifndef GBX_CONTEXT_H
#define GBX_CONTEXT_H

#include <GBx/GBx.h>
#include <GBx/Constants.h>

#include <GBx/APU.h>
#include <GBx/Cartridge.h>
#include <GBx/CPU.h>
#include <GBx/Interrupt.h>
#include <GBx/Joypad.h>
#include <GBx/Serial.h>
#include <GBx/MBC.h>
#include <GBx/PPU.h>
#include <GBx/Timer.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GBx
{
    ///
    /// General
    ///

    GBx_Model Model;

    ///
    /// Audio
    ///

    // NR10-NR14 Tone and Sweep
    GBx_ToneControl Tone1;

    // NR21-NR24 Tone
    GBx_ToneControl Tone2;

    // NR30-NR34 Wave
    GBx_WaveControl Wave;

    // PCM Waveform Data
    uint8_t WaveRAM[GBX_WAVE_RAM_SIZE];

    // NR41-NR44 Noise
    GBx_NoiseControl Noise;

    // NR50
    GBx_VolumeControl VolumeControl;

    // NR51
    GBx_OutputTerminalControl OutputTerminalControl;

    // NR52
    GBx_APUControl APUC;

    ///
    /// Bootstrap
    ///

    // Bootstrap / BIOS ROM
    uint8_t BootROM[GBX_BOOT_ROM_SIZE];

    bool BootstrapMapped;

    bool HaveBootstrap;

    bool HaveBootstrapCGB;

    ///
    /// Cartridge
    ///

    GBx_CartridgeHeader * Header;

    // Memory Bank Controller Config
    GBx_MemoryBankControl MBC;

    // Memory Bank Controller Type
    GBx_MemoryBankControlType MBCType;

    // Cartridge ROM
    uint8_t ROM[GBX_ROM_BANK_COUNT][GBX_ROM_BANK_SIZE];

    unsigned ROMBank;

    // Static RAM
    uint8_t SRAM[GBX_SRAM_BANK_COUNT][GBX_SRAM_BANK_SIZE];

    unsigned SRAMBank;

    bool SRAMEnabled;

    bool SRAMMapped;

    // Real Time Clock
    GBx_RealTimeClock RTC;

    bool RTCEnabled;

    bool RTCMapped;

    unsigned RTCBank;

    ///
    /// CPU
    ///

    bool CPUEnabled;

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

    // Work RAM
    uint8_t WRAM[GBX_WRAM_BANK_COUNT][GBX_WRAM_BANK_SIZE];

    unsigned WRAMBank;

    // High RAM / Zero Page
    uint8_t HRAM[GBX_HRAM_SIZE];

    ///
    /// Interrupt
    ///

    // Interrupt Master Enable Flag
    bool IME;

    // Enable IME after the next instruction
    bool RequestEnableIME;

    // Interrupt Request Flags
    GBx_InterruptFlags IF;

    // Interrupt Enable Flags
    GBx_InterruptFlags IE;

    ///
    /// Timer / Divider
    ///

    // Divider
    uint8_t DIV;

    // Timer Counter
    uint8_t TIMA;

    // Timer Modulo
    uint8_t TMA;

    GBx_TimerControl TAC;

    unsigned TimerCycles;

    unsigned DividerCycles;

    ///
    /// PPU
    ///

    bool PPUEnabled;

    unsigned PPUModeCycles;

    bool DMATransferOAM;

    GBx_LCDControl LCDC;

    GBx_LCDStatus STAT;

    // Video RAM
    uint8_t VRAM[GBX_VRAM_BANK_COUNT][GBX_VRAM_BANK_SIZE];

    unsigned VRAMBank;

    // Object Attribute Memory
    uint8_t OAM[GBX_OAM_SIZE];

    // Raw Pixel Data
    uint8_t PixelData[GBX_SCREEN_SIZE];

    // Scroll Y
    uint8_t SCY;

    // Scroll X
    uint8_t SCX;

    // Current Line
    uint8_t LY;

    // Coincidence when LY == LYC
    uint8_t LYC;

    // Window Y
    uint8_t WY;

    // Window X
    uint8_t WX;

    // DMG

    // Background Palette Data
    GBx_PaletteMonochrome BGP;

    // Sprite Palette 0 Data
    GBx_PaletteMonochrome OBP0;

    // Sprite Palette 1 Data
    GBx_PaletteMonochrome OBP1;

    // CGB

    // // Background Color Palette Data
    // GBx_PaletteColor BCPS;

    // // Background Palette Index
    // GBx_PaletteIndex BCPI;

    // // Sprite Color Palette Data
    // GBx_PaletteColor OCPS;

    // // Sprite Palette Index
    // GBx_PaletteIndex OCPI;

    ///
    /// Input/Output
    ///

    GBx_Joypad JOYP;

    GBx_SerialControl SC;

    uint8_t SB;

    GBx_SerialReadCallback SerialReadCallback;

    GBx_SerialWriteCallback SerialWriteCallback;

    void * SerialUserData;

    ///
    /// Debug
    ///

    uint8_t * MemoryReadTracker;

    uint8_t * MemoryWriteTracker;

    uint16_t LastInstructionAddress;

} GBx;

#ifdef __cplusplus
}
#endif

#endif // GBX_CONTEXT_H