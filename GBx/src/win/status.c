#include "status.h"
#include "../debug.h"

#include <GBx/apu.h>
#include <GBx/cpu.h>
#include <GBx/cartridge.h>
#include <GBx/clock.h>
#include <GBx/instruction.h>
#include <GBx/interrupt.h>
#include <GBx/lcd.h>
#include <GBx/memory.h>
#include <GBx/timer.h>

void StatusTabRender()
{
    int startX, startY;
    DebugGetCursor(&startX, &startY);

    DebugPrintln("STATUS");
    DebugPrintln("CPU: %s", 
        (CPUEnabled ? "ENABLED" : "DISABLED"));

    DebugPrintln("LCD: %s", 
        (LCDC.Enabled ? "ENABLED" : "DISABLED"));

    DebugPrintln("APU: %s", 
        (APUC.Enabled ? "ENABLED" : "DISABLED"));

    DebugPrintln("TAC: %s", 
        (TAC.Enabled ? "ENABLED" : "DISABLED"));
    
    DebugNewline();

    DebugPrintln("REGISTERS");
    DebugPrintln("A: $%02X  F: $%02X", R.A, R.F);
    DebugPrintln("B: $%02X  C: $%02X", R.B, R.C);
    DebugPrintln("D: $%02X  E: $%02X", R.D, R.E);
    DebugPrintln("H: $%02X  L: $%02X", R.H, R.L);
    DebugPrintln("PC: $%04X", R.PC);
    DebugPrintln("SP: $%04X", R.SP);
    DebugPrintln("FLAGS: [%c%c%c%c]",
        (R.FZ ? 'Z' : '-'), 
        (R.FN ? 'N' : '-'), 
        (R.FH ? 'H' : '-'), 
        (R.FC ? 'C' : '-'));

    DebugNewline();

    DebugPrintln("DIV:  $%02X", DIV);
    DebugPrintln("TMA:  $%02X", TMA);
    DebugPrintln("TIMA: $%02X", TIMA);
    DebugPrintln("TIMER: %d HZ", GetTimerSpeed());

    DebugNewline();

    DebugPrintln("INTERRUPTS");

    DebugPrintln("VBLANK: %s", 
        (IE.VBlank ? "ENABLED" : "DISABLED"));

    DebugPrintln("STAT:   %s", 
        (IE.STAT ? "ENABLED" : "DISABLED"));

    DebugPrintln("TIMER:  %s", 
        (IE.Timer ? "ENABLED" : "DISABLED"));

    DebugPrintln("SERIAL: %s", 
        (IE.Serial ? "ENABLED" : "DISABLED"));

    DebugPrintln("JOYPAD: %s", 
        (IE.Joypad ? "ENABLED" : "DISABLED"));

    DebugNewline();

    DebugSetCursor(startX + (DEBUG_CHAR_WIDTH * 20), startY);

    DebugPrintln("DEVICE: %s", 
        (ColorEnabled ? "CGB" : (SuperEnabled ? "SGB" : "DMG")));

    DebugPrintln("CLOCK: %d HZ", ClockSpeed);
    DebugPrintln("TITLE: %.*s", 15, CartridgeHeader.Title);
    DebugPrintln("CARTRIDGE: %s", GetCartridgeTypeString());
    DebugPrintln("ROM: %s", GetROMTypeString());
    DebugPrintln("RAM: %s", GetRAMTypeString());

    DebugNewline();

    DebugPrintln("LCD MODE: %s", GetLCDModeString(STAT.Mode));
    DebugPrintln("LY:  $%02X (%d)", LY, LY);
    DebugPrintln("LYC: $%02X (%d)", LYC, LYC);
    DebugPrintln("SCX: $%02X (%d)", SCX, SCX);
    DebugPrintln("SCY: $%02X (%d)", SCY, SCY);
    DebugPrintln("WX:  $%02X (%d)", WX, WX);
    DebugPrintln("WY:  $%02X (%d)", WY, WY);

    const int STACK_PREVIEW_LENGTH = 16;
    const int STACK_PREVIEW_ENTRY_SIZE = 26;

    int stackLogWidth = (DEBUG_CHAR_WIDTH * STACK_PREVIEW_ENTRY_SIZE);
    int stackLogHeight = (DEBUG_LINE_HEIGHT * STACK_PREVIEW_LENGTH);

    DebugSetCursor(startX + (DEBUG_CHAR_WIDTH * 60), startY);

    DebugPrintln("STACK [%04X]", StackBaseAddress);

    DebugPanel(stackLogWidth + (DEBUG_PANEL_PADDING * 2),
               stackLogHeight + (DEBUG_PANEL_PADDING * 2));

    char stackLogEntry[32];

    word addr = R.SP;
    for (int i = 0; i < STACK_PREVIEW_LENGTH; ++i) {
        if (addr == StackBaseAddress) {
            break;
        }

        word ptr = ReadWord(addr);

        Disassemble(stackLogEntry, sizeof(stackLogEntry), ptr);
        DebugPrintln("[%04X] $%04X: %s", addr, ptr, stackLogEntry);

        addr += 2;
    }

    DebugSetCursor(startX + (DEBUG_CHAR_WIDTH * 60), 
        startY + stackLogHeight + (DEBUG_LINE_HEIGHT * 6));

    DebugPrintln("INSTRUCTION LOG");

    int instLogWidth = (DEBUG_CHAR_WIDTH * STACK_PREVIEW_ENTRY_SIZE);
    int instLogHeight = (DEBUG_LINE_HEIGHT * INSTRUCTION_LOG_LENGTH);

    DebugPanel(instLogWidth + (DEBUG_PANEL_PADDING * 2),
               instLogHeight + (DEBUG_PANEL_PADDING * 2));

    for (int i = 0; i < GetInstructionLogSize(); ++i) {
        DebugPrintln(GetInstructionLogEntry(i));
    }
}
