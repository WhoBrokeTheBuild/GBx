#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <GBx/Types.h>

typedef union
{
    struct {
        bool VBlank:1;
        bool STAT:1;
        bool Timer:1;
        bool Serial:1;
        bool Joypad:1;
        uint :3;
    };
    byte raw;

} interrupt_flags;

// Interrupt Master Enable
extern bool IME;

// $FF0F - Interrupt Flag
extern interrupt_flags IF;

// $FFFF - Interrupt Enable
extern interrupt_flags IE;

void ResetInterrupts();

void CheckInterrupts();

void PrintIF();
void PrintIE();

#endif // INTERRUPTS_H