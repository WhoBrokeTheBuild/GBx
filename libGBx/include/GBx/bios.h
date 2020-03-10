#ifndef BIOS_H
#define BIOS_H

#include <stdbool.h>
#include <stdint.h>

extern bool BIOSEnable;

extern uint8_t BIOS[0x100];

bool loadBIOS(const char * filename);

void reset();

#endif // BIOS_H