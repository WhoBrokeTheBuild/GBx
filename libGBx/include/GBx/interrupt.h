#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>

typedef union
{
    struct {
        bool     VBlank:1;
        bool     STAT:1;
        bool     Timer:1;
        bool     Serial:1;
        bool     Joypad:1;
        unsigned :3;
    };
    uint8_t raw;

} interrupt_flags_t;

// Interrupt Master Enable

extern bool IME;

// FF0F - Interrupt Flag

extern interrupt_flags_t IF;

// FFFF - Interrupt Enable

extern interrupt_flags_t IE;

void checkInterrupts();

void printIF();
void printIE();

#endif // INTERRUPT_H