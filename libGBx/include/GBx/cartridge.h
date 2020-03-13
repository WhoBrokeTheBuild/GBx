#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "types.h"

typedef union
{
    struct {
        byte Code[4];
        byte Magic[48];
        char Title[15];
        byte ColorEnabled;
        byte SuperEnabled;
        byte CartridgeType;
        byte ROMType;
        byte RAMType;
        byte Region;
        byte License;
        byte Version;
        byte ComplementCheck;
        byte Checksum[2];
    };
    byte raw[80];

} cartridge_header;

extern cartridge_header CartridgeHeader;

extern bool HasCartridgeBattery;
extern bool HasCartridgeTimer;
extern bool HasCartridgeSRAM;

extern bool SRAMEnabled;
extern bool ColorEnabled;
extern bool SuperEnabled;

#define SRAM_BANK_COUNT (8)
#define SRAM_BANK_SIZE  (0x2000)

byte SRAM[SRAM_BANK_COUNT][SRAM_BANK_SIZE];

extern uint SRAMBank;

#define ROM_BANK_COUNT (512)
#define ROM_BANK_SIZE  (0x4000)

extern byte ROM[ROM_BANK_COUNT][ROM_BANK_SIZE];

extern uint ROMBank;

void ResetCartridgeMBC();

void WriteCartridgeMBC(word address, byte data);

bool LoadCartridgeROM(const char * filename);
void FeeCartridgeROM();

void PrintCartridgeMBC();
void PrintCartridge();

#endif // CARTRIDGE_H