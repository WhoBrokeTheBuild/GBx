#ifndef GBX_CONSTANTS_H
#define GBX_CONSTANTS_H

#define GBX_BOOT_ROM_SIZE_DMG   0x100
#define GBX_BOOT_ROM_SIZE_CGB   0x900

#define GBX_BOOT_ROM_SIZE       GBX_BOOT_ROM_SIZE_CGB

#define GBX_ROM_BANK_COUNT      512
#define GBX_ROM_BANK_SIZE       0x4000

#define GBX_VRAM_BANK_COUNT     2
#define GBX_VRAM_BANK_SIZE      0x2000

#define GBX_SRAM_BANK_COUNT     8
#define GBX_SRAM_BANK_SIZE      0x2000

#define GBX_WRAM_BANK_COUNT     8
#define GBX_WRAM_BANK_SIZE      0x1000

#define GBX_OAM_SIZE            0xA0

#define GBX_WAVE_RAM_SIZE       0x10

#define GBX_HRAM_SIZE           0x7F

#define GBX_ROM_HEADER_OFFSET   0x0100

#define GBX_SCREEN_WIDTH        160
#define GBX_SCREEN_HEIGHT       144
#define GBX_SCREEN_COMPONENTS   3   // RGB

#define GBX_SCREEN_SIZE                                                       \
    (GBX_SCREEN_WIDTH * GBX_SCREEN_HEIGHT * GBX_SCREEN_COMPONENTS)

#define GBX_STAT_MODE_HBLANK    0
#define GBX_STAT_MODE_VBLANK    1
#define GBX_STAT_MODE_SEARCH    2
#define GBX_STAT_MODE_DRAW      3

#define GBX_CLOCK_SPEED_DMG     4194304
#define GBX_CLOCK_SPEED_SGB     4295454
#define GBX_CLOCK_SPEED_CGB     8388608

#endif // GBX_CONSTANTS_H