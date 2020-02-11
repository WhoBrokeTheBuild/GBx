#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdbool.h>
#include <stdint.h>

extern char CartridgeTitle[15];

extern bool ColorEnabled;
extern bool SuperEnabled;

extern uint8_t CartridgeType;

extern bool CartridgeRAMEnabled;

extern uint8_t * CartridgeRAM0;
extern uint8_t * CartridgeRAM;

extern uint8_t * CartridgeROM0;
extern uint8_t * CartridgeROM;

typedef enum {
    MBC_NONE,
    MBC_MBC1,
    MBC_MBC2,
    MBC_MBC3,
    MBC_MBC5,
    MBC_MBC6,
    MBC_MBC7,
    MBC_MMM01,
    
} mbc_type_t;

extern mbc_type_t CartridgeMBCType;

void writeCartridgeMBC(uint16_t address, uint8_t data);

bool loadCartridge(const char * filename);
void freeCartridge();

void printBank();
void printCartridge();

#endif // CARTRIDGE_H