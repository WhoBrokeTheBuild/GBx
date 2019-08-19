#include "interrupt.h"
#include "log.h"
#include "cpu.h"
#include "memory.h"
#include "register.h"
#include "video.h"

bool IME = true;
InterruptFlags_t IE;
InterruptFlags_t IF;

void checkInterrupts()
{
    if (IME && IE.data > 0 && IF.data > 0) {
        InterruptFlags_t enabledInts = { 
            .data = IE.data & IF.data,
        };

        if (enabledInts.VBlank) {
            enabledInts.VBlank = false;
            IME = false;
            pushWord(R.PC);
            R.PC = 0x40;
        }

        if (enabledInts.STAT) {
            enabledInts.STAT = false;
            IME = false;
            pushWord(R.PC);
            R.PC = 0x48;
        }

        if (enabledInts.Timer) {
            enabledInts.Timer = false;
            IME = false;
            pushWord(R.PC);
            R.PC = 50;
        }

        if (enabledInts.Serial) {
            enabledInts.Serial = false;
            IME = false;
            pushWord(R.PC);
            R.PC = 59;
        }

        if (enabledInts.Joypad) {
            enabledInts.Joypad = false;
            IME = false;
            pushWord(R.PC);
            R.PC = 60;
        }
    }
}