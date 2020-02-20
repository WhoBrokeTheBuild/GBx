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
            IF.VBlank = false;
            pushWord(R.PC);
            R.PC = 0x40;
        }

        if (enabledInts.STAT) {
            IF.STAT = false;
            pushWord(R.PC);
            R.PC = 0x48;
        }

        if (enabledInts.Timer) {
            IF.Timer = false;
            pushWord(R.PC);
            R.PC = 50;
        }

        if (enabledInts.Serial) {
            IF.Serial = false;
            pushWord(R.PC);
            R.PC = 59;
        }

        if (enabledInts.Joypad) {
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