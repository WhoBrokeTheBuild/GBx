#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include <stdbool.h>
#include <stdint.h>

extern bool BootstrapEnable;

extern uint8_t BOOTSTRAP[0x100];

void bootstrap();

#endif // BOOTSTRAP_H