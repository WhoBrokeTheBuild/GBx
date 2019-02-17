#include "memory.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t RAM0[0x0FFF];
uint8_t RAM1[0x0FFF];
uint8_t RAM2[0x0FFF];
uint8_t RAM3[0x0FFF];
uint8_t RAM4[0x0FFF];
uint8_t RAM5[0x0FFF];
uint8_t RAM6[0x0FFF];
uint8_t RAM7[0x0FFF];
uint8_t * RAM = RAM0;

uint8_t * ROM0 = NULL;
uint8_t * ROM = NULL;

uint8_t readByte(uint16_t address)
{
    if (address <= 0x00FF) {
        // Restart & Interrupt Vectors
    } else if (address <= 0x014F) {
        // ROM Header
        return ROM[address];
    } else if (address <= 0x3FFF) {
        // ROM Bank 0
        return ROM0[address];
    } else if (address <= 0x7FFF) {
        // ROM Switchable Bank
        return ROM[address - 0x4000];
    } else if (address <= 0x97FF) {
        // Character RAM
    } else if (address <= 0x9BFF) {
        // BG Map Data 1
    } else if (address <= 0x9FFF) {
        // BG Map Data 2
    } else if (address <= 0xBFFF) {
        // Cartridge RAM
    } else if (address <= 0xCFFF) {
        // Internal RAM Bank 0
        return RAM0[address - 0xC000];
    } else if (address <= 0xDFFF) {
        // Internal RAM Switchable Bank
        return RAM[address - 0xD000];
    } else if (address <= 0xFDFF) {
        // Echo RAM
    } else if (address <= 0xFE9F) {
        // Object Attribute Memory OAM
    } else if (address <= 0xFEFF) {
        // Unusable
    } else if (address <= 0xFF7F) {
        // Hardware I/O Registers
    } else if (address <= 0xFFFE) {
        // Zero Page
    } else if (address == 0xFFFF) {
        // Interrupt Enable Flag
    }

    return 0;
}

void writeByte(uint16_t address, uint8_t data)
{

}

bool loadROM(const char * filename)
{
    FILE * file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    ROM = ROM0 = (uint8_t *)malloc(size);

    size_t bytesRead = fread(ROM, 1, size, file);
    fclose(file);

    LogVerbose("read %zu bytes", bytesRead);

    if (bytesRead < size) {
        LogError("failed to load %s", filename);
        return false;
    }

    return true;
}

void freeROM() 
{
    free(ROM);
}