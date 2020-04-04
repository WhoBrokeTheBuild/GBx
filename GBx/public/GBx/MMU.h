#ifndef MMU_H
#define MMU_H

#include <GBx/Types.h>

#define WRAM_BANK_COUNT (8)
#define WRAM_BANK_SIZE  (0x1000)

extern uint8_t WRAM[WRAM_BANK_COUNT][WRAM_BANK_SIZE];

extern unsigned WRAMBank;

extern uint8_t HRAM[0x7F];

void ResetMMU();

uint8_t ReadByte(uint16_t address);

#define ReadWord(address) (ReadByte(address) | (ReadByte((address) + 1) << 8))

uint8_t  NextByte();
uint16_t NextWord();

void WriteByte(uint16_t address, uint8_t data);
void WriteWord(uint16_t address, uint16_t data);

void PushWord(uint16_t data);

uint16_t PopWord();

#endif // MMU_H