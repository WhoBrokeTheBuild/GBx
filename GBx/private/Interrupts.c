#include <GBx/Interrupts.h>

#include <GBx/CPU.h>
#include <GBx/LCD.h>
#include <GBx/MMU.h>

#include <GBx/Log.h>

bool IME = true;

interrupt_flags IF;
interrupt_flags IE;

void ResetInterrupts()
{
    IME = true;
    IF.raw = 0x00;
    IE.raw = 0x00;
}

void callInterrupt(word addr)
{
    IME = false;
    Tick(8);

    PushWord(R.PC);
    Tick(8);

    R.PC = addr;
    Tick(4);
}

void CheckInterrupts()
{
    if (IME) {
        interrupt_flags enabledInts = { 
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

void PrintIF()
{
    LogInfo("IF VBlank=%d STAT=%d Timer=%d Serial=%d Joypad=%d",
        IF.VBlank, IF.STAT, IF.Timer, IF.Serial, IF.Joypad);
}

void PrintIE() 
{
    LogInfo("IE VBlank=%d STAT=%d Timer=%d Serial=%d Joypad=%d",
        IE.VBlank, IE.STAT, IE.Timer, IE.Serial, IE.Joypad);
}
