#include "interrupt.h"

#include "cpu.h"
#include "lcd.h"
#include "log.h"
#include "memory.h"
#include "register.h"

bool IME = true;

int_flags_t IE;
int_flags_t IF;

void checkInterrupts()
{
    if (IME) {
        int_flags_t enabledInts = { 
            .data = IE.data & IF.data,
        };

        if (enabledInts.VBlank) {
            LogVerbose(2, "VBlank Interrupt");
            IF.VBlank = false;
            pushWord(R.PC);
            R.PC = 0x40;
        }

        if (enabledInts.STAT) {
            LogVerbose(2, "STAT Interrupt");
            IF.STAT = false;
            pushWord(R.PC);
            R.PC = 0x48;
        }

        if (enabledInts.Timer) {
            LogVerbose(2, "Timer Interrupt");
            IF.Timer = false;
            pushWord(R.PC);
            R.PC = 50;
        }

        if (enabledInts.Serial) {
            LogVerbose(2, "Serial Interrupt");
            IF.Serial = false;
            pushWord(R.PC);
            R.PC = 59;
        }

        if (enabledInts.Joypad) {
            LogVerbose(2, "Joypad Interrupt");
            IF.Joypad = false;
            pushWord(R.PC);
            R.PC = 60;
        }
    }
}

void printIE() 
{
    LogInfo("IE VBlank=%d STAT=%d Timer=%d Serial=%d Joypad=%d",
        IE.VBlank, IE.STAT, IE.Timer, IE.Serial, IE.Joypad);
}

void printIF() 
{
    LogInfo("IF VBlank=%d STAT=%d Timer=%d Serial=%d Joypad=%d",
        IF.VBlank, IF.STAT, IF.Timer, IF.Serial, IF.Joypad);
}