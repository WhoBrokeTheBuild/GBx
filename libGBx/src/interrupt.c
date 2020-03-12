#include "interrupt.h"

#include "cpu.h"
#include "clock.h"
#include "lcd.h"
#include "log.h"
#include "memory.h"

bool IME = true;

interrupt_flags_t IF;
interrupt_flags_t IE;

void callInterrupt(uint16_t addr)
{
    IME = false;
    tick(8);

    pushWord(R.PC);
    tick(8);

    R.PC = addr;
    tick(4);
}

void checkInterrupts()
{
    if (IME) {
        interrupt_flags_t enabledInts = { 
            .raw = IE.raw & IF.raw,
        };

        if (enabledInts.VBlank) {
            LogVerbose(2, "VBlank Interrupt");
            IF.VBlank = false;
            callInterrupt(0x40);
        }

        if (enabledInts.STAT) {
            LogVerbose(2, "STAT Interrupt");
            IF.STAT = false;
            callInterrupt(0x48);
        }

        if (enabledInts.Timer) {
            LogVerbose(2, "Timer Interrupt");
            IF.Timer = false;
            callInterrupt(0x50);
        }

        if (enabledInts.Serial) {
            LogVerbose(2, "Serial Interrupt");
            IF.Serial = false;
            callInterrupt(0xf8);
        }

        if (enabledInts.Joypad) {
            LogVerbose(2, "Joypad Interrupt");
            IF.Joypad = false;
            callInterrupt(0x60);
        }
    }
}

void printIF() 
{
    LogInfo("IF VBlank=%d STAT=%d Timer=%d Serial=%d Joypad=%d",
        IF.VBlank, IF.STAT, IF.Timer, IF.Serial, IF.Joypad);
}

void printIE() 
{
    LogInfo("IE VBlank=%d STAT=%d Timer=%d Serial=%d Joypad=%d",
        IE.VBlank, IE.STAT, IE.Timer, IE.Serial, IE.Joypad);
}
