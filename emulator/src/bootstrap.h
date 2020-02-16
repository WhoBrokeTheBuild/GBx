#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include <stdint.h>
#include <stdbool.h>

extern bool BootstrapEnable;

extern uint8_t BOOTSTRAP[0x100];

void bootstrap();

#endif // BOOTSTRAP_H