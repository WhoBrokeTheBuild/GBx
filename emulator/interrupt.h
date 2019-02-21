#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "log.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    union {
        bool VBlank:1;
        bool STAT:1;
        bool Timer:1;
        bool Serial:1;
        bool Joypad:1;
        uint8_t _:3;
    };
    uint8_t data;
} IE_t;

extern bool IME;
extern IE_t IE;
extern IE_t IF;

static void printIE() 
{
    LogDebug("VBlank=%d STAT=%d Timer=%d Serial=%d Joypad=%d",
        IE.VBlank, IE.STAT, IE.Timer, IE.Serial, IE.Joypad);
}

#endif // INTERRUPT_H