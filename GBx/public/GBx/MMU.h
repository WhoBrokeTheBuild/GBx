#ifndef MMU_H
#define MMU_H

#include <GBx/Types.h>

#define WRAM_BANK_COUNT (8)
#define WRAM_BANK_SIZE  (0x1000)

extern byte WRAM[WRAM_BANK_COUNT][WRAM_BANK_SIZE];

extern uint WRAMBank;

extern byte HRAM[0x7F];

void ResetMMU();

byte ReadByte(word address);

#define ReadWord(address) \
    (ReadByte(address) | (ReadByte((address) + 1) << 8))

byte NextByte();
word NextWord();

void WriteByte(word address, byte data);
void WriteWord(word address, word data);

void PushWord(word data);

word PopWord();

#endif // MMU_H