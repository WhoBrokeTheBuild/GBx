#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stdint.h>

uint8_t readByte(uint16_t address);
void writeByte(uint16_t address, uint8_t data);

bool loadROM(const char * filename);
void freeROM();

#endif // MEMORY_H