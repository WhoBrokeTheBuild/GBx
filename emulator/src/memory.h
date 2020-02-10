#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stdint.h>

extern char Title[15];

extern bool ColorEnabled;
extern bool SuperEnabled;

extern uint8_t CartridgeType;

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

void printBank();
void printCartridge();

#endif // MEMORY_H