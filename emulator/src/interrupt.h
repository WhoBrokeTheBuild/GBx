#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>

typedef union {
    struct {
        bool     VBlank:1;
        bool     STAT:1;
        bool     Timer:1;
        bool     Serial:1;
        bool     Joypad:1;
        unsigned :3;
    };
    uint8_t data;
} int_flags_t;

extern bool IME;

extern int_flags_t IE;
extern int_flags_t IF;

void checkInterrupts();

void printIE();
void printIF();

#endif // INTERRUPT_H