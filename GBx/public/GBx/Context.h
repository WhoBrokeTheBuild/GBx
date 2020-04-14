#ifndef GBX_CONTEXT_H
#define GBX_CONTEXT_H

#include <SM83/SM83.h>

#include <GBx/Types.h>

#define GBX_WAVE_RAM_SIZE (0x10)

#define GBX_BOOTSTRAP_ROM_SIZE (0x900)

#define GBX_ROM_BANK_COUNT (512)
#define GBX_ROM_BANK_SIZE  (0x4000)

#define GBX_SRAM_BANK_COUNT (8)
#define GBX_SRAM_BANK_SIZE  (0x2000)

#define GBX_WRAM_BANK_COUNT (8)
#define GBX_WRAM_BANK_SIZE  (0x1000)

#define GBX_HRAM_SIZE (0x7F)

#define GBX_VRAM_BANK_COUNT (2)
#define GBX_VRAM_BANK_SIZE  (0x2000)

#define GBX_OAM_SIZE (0xA0)

#define GBX_BACKBUFFER_WIDTH      (256)
#define GBX_BACKBUFFER_HEIGHT     (256)
#define GBX_BACKBUFFER_COMPONENTS (3) // RGB

#define GBX_BACKBUFFER_SIZE                                                    \
    (GBX_BACKBUFFER_WIDTH * GBX_BACKBUFFER_HEIGHT * GBX_BACKBUFFER_COMPONENTS)

#define GBX_LCD_WIDTH  (160)
#define GBX_LCD_HEIGHT (144)

typedef struct gbx_internal gbx_internal_t;

typedef struct gbx
{
    // General

    int VerboseLevel;

    bool UseOriginalColors;

    // APU

    gbx_tone_control_t Tone1;

    gbx_tone_control_t Tone2;

    gbx_wave_control_t Wave;

    uint8_t WaveRAM[GBX_WAVE_RAM_SIZE];

    gbx_noise_control_t Noise;

    gbx_volume_control_t VolumeControl;

    gbx_sound_output_terminal_t SoundOutputTerminal;

    gbx_sound_control_t APUC;

    // Bootstrap

    uint8_t Bootstrap[GBX_BOOTSTRAP_ROM_SIZE];

    bool BootstrapEnabled;

    // Cartridge

    gbx_cartridge_header_t CartridgeHeader;

    uint8_t ROM[GBX_ROM_BANK_COUNT][GBX_ROM_BANK_SIZE];

    unsigned ROMBank;

    uint8_t SRAM[GBX_SRAM_BANK_COUNT][GBX_SRAM_BANK_SIZE];

    unsigned SRAMBank;

    gbx_mbc_type_t MBCType;

    gbx_mbc_bank_control_t MBC;

    bool SRAMEnabled;

    bool HaveCartridgeBattery;
    bool HaveCartridgeTimer;
    bool HaveCartridgeSRAM;

    bool ColorEnabled;
    bool SuperEnabled;

    // CPU

    sm83_t * CPU;

    // Joypad

    bool RightPressed;
    bool LeftPressed;
    bool UpPressed;
    bool DownPressed;
    bool APressed;
    bool BPressed;
    bool SelectPressed;
    bool StartPressed;

    gbx_joypad_state_t JOYP;

    // MMU

    uint8_t WRAM[GBX_WRAM_BANK_COUNT][GBX_WRAM_BANK_SIZE];

    unsigned WRAMBank;

    uint8_t HRAM[GBX_HRAM_SIZE];

    // PPU
    
    uint8_t VRAM[GBX_VRAM_BANK_COUNT][GBX_VRAM_BANK_SIZE];

    unsigned VRAMBank;

    uint8_t OAM[GBX_OAM_SIZE];

    uint8_t Backbuffer[GBX_BACKBUFFER_SIZE];

    uint8_t SCY;
    uint8_t SCX;
    uint8_t LY;
    uint8_t LYC;
    uint8_t WX;
    uint8_t WY;

    gbx_palette_t BGP;
    gbx_palette_t OBP0;
    gbx_palette_t OBP1;

    gbx_lcd_control_t LCDC;
    gbx_lcd_status_t STAT;

    // Serial

    uint8_t SB;

    gbx_serial_control_t SC;

    // Timer

    gbx_timer_control_t TAC;

    uint8_t DIV;
    uint8_t TIMA;
    uint8_t TMA;

    // Internal

    gbx_internal_t * internal;

} gbx_t;

#endif // GBX_CONTEXT_H