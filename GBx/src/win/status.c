#include "status.h"
#include "../debug.h"

#include <GBx/apu.h>
#include <GBx/cpu.h>
#include <GBx/cartridge.h>
#include <GBx/clock.h>
#include <GBx/instruction.h>
#include <GBx/interrupt.h>
#include <GBx/lcd.h>
#include <GBx/timer.h>

void StatusTabRender()
{
    char buffer[1024];

    int x = DEBUG_WINDOW_CONTENT_X;
    int y = DEBUG_WINDOW_CONTENT_Y + DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "CPU: %s", 
        (CPUEnabled ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "LCD: %s", 
        (LCDC.Enabled ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "APU: %s", 
        (SoundControl.Enabled ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "TAC: %s", 
        (TAC.Enabled ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "A: %02X  F: %02X", R.A, R.F);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "B: %02X  C: %02X", R.B, R.C);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "D: %02X  E: %02X", R.D, R.E);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "H: %02X  L: %02X", R.H, R.L);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "PC: %04X", R.PC);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "SP: %04X", R.SP);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "FLAGS: [%c%c%c%c]",
        (R.FZ ? 'Z' : '-'), 
        (R.FN ? 'N' : '-'), 
        (R.FH ? 'H' : '-'), 
        (R.FC ? 'C' : '-'));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "DIV: %02X", DIV);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "TMA: %02X", TMA);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "TIMA: %02X", TIMA);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "TIMER: %d HZ", GetTimerSpeed());
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    RenderDebugString(x, y, "INTERRUPTS:");
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "VBLANK: %s", 
        (IE.VBlank ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "STAT: %s", 
        (IE.STAT ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "TIMER: %s", 
        (IE.Timer ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "SERIAL: %s", 
        (IE.Serial ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "JOYPAD: %s", 
        (IE.Joypad ? "ENABLED" : "DISABLED"));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    y += DEBUG_LINE_HEIGHT;

    SDL_Rect instBox = {
        .x = x,
        .y = y,
        .w = (20 * DEBUG_CHARACTER_WIDTH),
        .h = (INSTRUCTION_LOG_LENGTH * DEBUG_LINE_HEIGHT) 
            + (DEBUG_CHARACTER_HEIGHT * 2),
    };

    x += DEBUG_CHARACTER_WIDTH;

    SDL_RenderDrawRect(GetDebugWindowRenderer(), &instBox);

    y += (DEBUG_LINE_HEIGHT * INSTRUCTION_LOG_LENGTH);
    for (int i = 0; i < GetInstructionLogSize(); ++i) {
        RenderDebugString(x, y, GetInstructionLogEntry(i));
        y -= DEBUG_LINE_HEIGHT;
    }

    x = DEBUG_WINDOW_CONTENT_X + (DEBUG_CHARACTER_WIDTH * 20);
    y = DEBUG_WINDOW_CONTENT_Y + DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "DEVICE: %s", 
        (ColorEnabled ? "CGB" : (SuperEnabled ? "SGB" : "DMG")));
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "CLOCK: %d HZ", ClockSpeed);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "TITLE: %.*s", 15, CartridgeHeader.Title);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "CARTRIDGE: %s", GetCartridgeTypeString());
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "ROM: %s", GetROMTypeString());
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "RAM: %s", GetRAMTypeString());
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

}
