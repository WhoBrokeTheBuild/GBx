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
    if (IME) {
        InterruptFlags_t enabledInts = { 
            .data = IE.data & IF.data,
        };

        if (enabledInts.data > 0) {
            LogInfo("INT?!");
        }

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