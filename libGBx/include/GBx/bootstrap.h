#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include "types.h"

extern bool HasBootstrapROM;
extern bool BootstrapROMEnabled;

extern byte BootstrapROM[0x100];

bool LoadBootstrapROM(const char * filename);

void Reset();

#endif // BOOTSTRAP_H