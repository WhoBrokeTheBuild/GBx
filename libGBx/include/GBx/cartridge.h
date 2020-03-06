#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdbool.h>
#include <stdint.h>

typedef union {
    struct {
        uint8_t Code[4];
        uint8_t Magic[48];
        uint8_t Title[15];
        uint8_t ColorEnabled;
        uint8_t SuperEnabled;
        uint8_t CartridgeType;
        uint8_t ROMType;
        uint8_t RAMType;
        uint8_t Region;
        uint8_t License;
        uint8_t ROMMask;
        uint8_t ComplementCheck;
        uint8_t Checksum[2];
    };
    uint8_t raw[80];
} cartridge_header_t;

extern cartridge_header_t CartridgeHeader;

extern bool ColorEnabled;
extern bool SuperEnabled;

extern bool HasCartridgeBattery;
extern bool HasCartridgeTimer;
extern bool HasSRAM;
extern bool HasCartridgeSRAM;

extern bool SRAMEnabled;

extern uint8_t * SRAM0;
extern uint8_t * SRAM;

extern uint8_t * ROM0;
extern uint8_t * ROM;

void writeCartridgeMBC(uint16_t address, uint8_t data);

bool loadCartridge(const char * filename);
void freeCartridge();

void printBank();
void printCartridge();

#endif // CARTRIDGE_H