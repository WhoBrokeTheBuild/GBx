#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <GBx/Types.h>
#include <GBx/Macros.h>

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t Code[4];
        uint8_t Magic[48];
        char    Title[15];
        uint8_t ColorEnabled;
        uint8_t SuperEnabled;
        uint8_t CartridgeType;
        uint8_t ROMType;
        uint8_t RAMType;
        uint8_t Region;
        uint8_t License;
        uint8_t Version;
        uint8_t ComplementCheck;
        uint8_t Checksum[2];
    });

    // clang-format on

    uint8_t raw[80];

} cartridge_header_t;

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