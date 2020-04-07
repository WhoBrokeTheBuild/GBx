#include <SM83/SM83.h>

#include "Log.h"

void SM83_CallInterrupt(sm83_t * cpu, uint16_t addr)
{
    cpu->Enabled = true;
    cpu->IME = false;
    SM83_Tick(cpu, 2);

    SM83_PushWord(cpu, cpu->PC);
    SM83_Tick(cpu, 2);

    cpu->PC = addr;
    SM83_Tick(cpu, 1);
}

void SM83_CheckInterrupts(sm83_t * cpu)
{
    if (!cpu->IME) {
        return;
    }

    sm83_interrupt_flags_t enabledInts = {
        .raw = cpu->IE.raw & cpu->IF.raw,
    };

    if (enabledInts.Int40) {
        Verbose(cpu, 2, LogInfo("Interrupt 0x40"));

        cpu->IF.Int40 = false;
        SM83_CallInterrupt(cpu, 0x40);
    }

    if (enabledInts.Int48) {
        Verbose(cpu, 2, LogInfo("Interrupt 0x48"));

        cpu->IF.Int48 = false;
        SM83_CallInterrupt(cpu, 0x48);
    }

    if (enabledInts.Int50) {
        Verbose(cpu, 2, LogInfo("Interrupt 0x50"));

        cpu->IF.Int50 = false;
        SM83_CallInterrupt(cpu, 0x50);
    }

    if (enabledInts.Int58) {
        Verbose(cpu, 2, LogInfo("Interrupt 0x58"));

        cpu->IF.Int58 = false;
        SM83_CallInterrupt(cpu, 0x58);
    }

    if (enabledInts.Int60) {
        Verbose(cpu, 2, LogInfo("Interrupt 0x60"));

        cpu->IF.Int60 = false;
        SM83_CallInterrupt(cpu, 0x60);
    }
}

void SM83_PrintInterrupts(sm83_t * cpu)
{
    LogInfo("IME %s", (cpu->IME ? "Enabled" : "Disabled"));

    LogInfo("IF 0x40=%d 0x48=%d 0x50=%d 0x58=%d 0x60=%d",
        cpu->IF.Int40,
        cpu->IF.Int48,
        cpu->IF.Int50,
        cpu->IF.Int58,
        cpu->IF.Int60);

    LogInfo("IF 0x40=%d 0x48=%d 0x50=%d 0x58=%d 0x60=%d",
        cpu->IE.Int40,
        cpu->IE.Int48,
        cpu->IE.Int50,
        cpu->IE.Int58,
        cpu->IE.Int60);
}
