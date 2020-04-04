#include <SM83/SM83.h>

#include "Log.h"

void SM83_Init(sm83_t * cpu)
{
    cpu->Enabled = true;

    cpu->AF = 0x01B0;
    cpu->BC = 0x0013;
    cpu->DE = 0x00D8;
    cpu->HL = 0x014D;
    cpu->PC = 0x0100;
    cpu->SP = 0xFFFE;

    cpu->IME = true;
    cpu->RequestEnableIME = false;

    cpu->IF.raw = 0x00;
    cpu->IE.raw = 0x00;

    cpu->LastInstructionAddress = cpu->PC;
    cpu->StackBaseAddress = cpu->SP;

    cpu->TotalTicks = 0;

    cpu->UserData = NULL;
    cpu->Tick = NULL;
    cpu->ReadByte = NULL;
    cpu->WriteByte = NULL;

    SM83_SetMode(cpu, SM83_MODE_DMG);
}

void SM83_Tick(sm83_t * cpu, unsigned mcycles)
{
    cpu->TotalTicks += mcycles;
    cpu->Tick(cpu->UserData, mcycles);
}

void SM83_SetMode(sm83_t * cpu, sm83_mode_t mode)
{
    static const int DMG_CLOCK_SPEED = 4194304; // Hz
    static const int SGB_CLOCK_SPEED = 4295454; // Hz
    static const int CGB_CLOCK_SPEED = 8388608; // Hz

    cpu->Mode = mode;

    switch (cpu->Mode) {
    case SM83_MODE_DMG:
        cpu->ClockSpeed = DMG_CLOCK_SPEED;
        break;
    case SM83_MODE_SGB:
        cpu->ClockSpeed = SGB_CLOCK_SPEED;
        break;
    case SM83_MODE_CGB:
        cpu->ClockSpeed = CGB_CLOCK_SPEED;
        break;
    }
}