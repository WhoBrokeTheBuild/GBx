#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include <GBx/Types.h>

extern bool HasBootstrapROM;

extern bool BootstrapROMEnabled;

extern byte BootstrapROM[0x100];

bool LoadBootstrapROM(const char * filename);

#endif // BOOTSTRAP_H