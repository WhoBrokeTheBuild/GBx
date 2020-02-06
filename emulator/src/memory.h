#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stdint.h>

uint8_t readByte(uint16_t address);
uint16_t readWord(uint16_t address);

uint8_t nextByte();
uint16_t nextWord();

void writeByte(uint16_t address, uint8_t data);
void writeWord(uint16_t address, uint16_t data);

void pushByte(uint8_t data);
void pushWord(uint16_t data);

uint8_t popByte();
uint16_t popWord();

bool loadROM(const char * filename);
void freeROM();

#endif // MEMORY_H