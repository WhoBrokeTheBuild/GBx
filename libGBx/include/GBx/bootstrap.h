#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include <stdbool.h>
#include <stdint.h>

extern bool BootstrapEnable;

extern uint8_t Bootstrap[0x100];

bool loadBootstrap(const char * filename);

void bootstrap();

#endif // BOOTSTRAP_H