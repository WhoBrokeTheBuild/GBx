#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <GBx/Macros.h>
#include <GBx/Types.h>

#include <assert.h>

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        // $0100 - $0103
        uint8_t Code[4];
        // $0104 - $0133
        uint8_t Magic[48];
        // $0134 - $0143
        char    Title[15];
        // $0143
        uint8_t ColorEnabled;
        // $0144 - $0145
        uint8_t NewLicenseCode[2];
        // $0146
        uint8_t SuperEnabled;
        // $0147
        uint8_t CartridgeType;
        // $0148
        uint8_t ROMType;
        // $0149
        uint8_t RAMType;
        // $014A
        uint8_t Region;
        // $014B
        uint8_t License;
        // $014C
        uint8_t Version;
        // $014D
        uint8_t ComplementCheck;
        // $014E - 014F
        uint8_t Checksum[2];
    });

    // clang-format on

    uint8_t raw[80];

} cartridge_header_t;

static_assert(sizeof(cartridge_header_t) == 80,
    "sizeof(cartridge_header_t) != 80");

extern cartridge_header_t CartridgeHeader;

extern bool HasCartridgeBattery;
extern bool HasCartridgeTimer;
extern bool HasCartridgeSRAM;

extern bool SRAMEnabled;
extern bool ColorEnabled;
extern bool SuperEnabled;

#define SRAM_BANK_COUNT (8)
#define SRAM_BANK_SIZE  (0x2000)

extern uint8_t SRAM[SRAM_BANK_COUNT][SRAM_BANK_SIZE];

extern unsigned SRAMBank;

#define ROM_BANK_COUNT (512)
#define ROM_BANK_SIZE  (0x4000)

extern uint8_t ROM[ROM_BANK_COUNT][ROM_BANK_SIZE];

extern unsigned ROMBank;

bool LoadCartridgeROM(const char * filename);

const char * GetCartridgeTypeString();
const char * GetROMTypeString();
const char * GetRAMTypeString();

void PrintCartridge();

#endif // CARTRIDGE_H