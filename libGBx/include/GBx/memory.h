#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

void ResetMemory();

byte ReadByte(word address);
word ReadWord(word address);

byte NextByte();
word NextWord();

void WriteByte(word address, byte data);
void WriteWord(word address, word data);

void PushByte(byte data);
void PushWord(word data);

byte PopByte();
word PopWord();

#endif // MEMORY_H