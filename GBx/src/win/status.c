#include "status.h"
#include "../debug.h"
#include "../ui.h"

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
    UIGetCursor(&startX, &startY);

    UIPrintln("STATUS");
    UIPrintln("CPU: %s", 
        (CPUEnabled ? "ENABLED" : "DISABLED"));

    UIPrintln("LCD: %s", 
        (LCDC.Enabled ? "ENABLED" : "DISABLED"));

    UIPrintln("APU: %s", 
        (APUC.Enabled ? "ENABLED" : "DISABLED"));

    UIPrintln("TAC: %s", 
        (TAC.Enabled ? "ENABLED" : "DISABLED"));
    
    UINewline();

    UIPrintln("REGISTERS");
    UIPrintln("A: $%02X  F: $%02X", R.A, R.F);
    UIPrintln("B: $%02X  C: $%02X", R.B, R.C);
    UIPrintln("D: $%02X  E: $%02X", R.D, R.E);
    UIPrintln("H: $%02X  L: $%02X", R.H, R.L);
    UIPrintln("PC: $%04X", R.PC);
    UIPrintln("SP: $%04X", R.SP);
    UIPrintln("FLAGS: [%c%c%c%c]",
        (R.FZ ? 'Z' : '-'), 
        (R.FN ? 'N' : '-'), 
        (R.FH ? 'H' : '-'), 
        (R.FC ? 'C' : '-'));

    UINewline();

    UIPrintln("DIV:  $%02X", DIV);
    UIPrintln("TMA:  $%02X", TMA);
    UIPrintln("TIMA: $%02X", TIMA);
    UIPrintln("TIMER: %d HZ", GetTimerSpeed());

    UINewline();

    UIPrintln("INTERRUPTS");

    UIPrintln("VBLANK: %s", 
        (IE.VBlank ? "ENABLED" : "DISABLED"));

    UIPrintln("STAT:   %s", 
        (IE.STAT ? "ENABLED" : "DISABLED"));

    UIPrintln("TIMER:  %s", 
        (IE.Timer ? "ENABLED" : "DISABLED"));

    UIPrintln("SERIAL: %s", 
        (IE.Serial ? "ENABLED" : "DISABLED"));

    UIPrintln("JOYPAD: %s", 
        (IE.Joypad ? "ENABLED" : "DISABLED"));

    UINewline();

    UISetCursor(startX + (UI_CHAR_WIDTH * 20), startY);

    UIPrintln("DEVICE: %s", 
        (ColorEnabled ? "CGB" : (SuperEnabled ? "SGB" : "DMG")));

    UIPrintln("CLOCK: %d HZ", ClockSpeed);
    UIPrintln("TITLE: %.*s", 15, CartridgeHeader.Title);
    UIPrintln("CARTRIDGE: %s", GetCartridgeTypeString());
    UIPrintln("ROM: %s", GetROMTypeString());
    UIPrintln("RAM: %s", GetRAMTypeString());

    UINewline();

    UIPrintln("LCD MODE: %s", GetLCDModeString(STAT.Mode));
    UIPrintln("LY:  $%02X (%d)", LY, LY);
    UIPrintln("LYC: $%02X (%d)", LYC, LYC);
    UIPrintln("SCX: $%02X (%d)", SCX, SCX);
    UIPrintln("SCY: $%02X (%d)", SCY, SCY);
    UIPrintln("WX:  $%02X (%d)", WX, WX);
    UIPrintln("WY:  $%02X (%d)", WY, WY);

    const int STACK_PREVIEW_LENGTH = 16;
    const int STACK_PREVIEW_ENTRY_SIZE = 26;

    int stackLogWidth = (UI_CHAR_WIDTH * STACK_PREVIEW_ENTRY_SIZE);
    int stackLogHeight = (UI_LINE_HEIGHT * STACK_PREVIEW_LENGTH);

    UISetCursor(startX + (UI_CHAR_WIDTH * 60), startY);

    UIPrintln("STACK [%04X]", StackBaseAddress);

    UIPanel(stackLogWidth + (UI_PANEL_PADDING * 2),
               stackLogHeight + (UI_PANEL_PADDING * 2));

    char stackLogEntry[32];

    word addr = R.SP;
    for (int i = 0; i < STACK_PREVIEW_LENGTH; ++i) {
        if (addr == StackBaseAddress) {
            break;
        }

        word ptr = ReadWord(addr);

        Disassemble(stackLogEntry, sizeof(stackLogEntry), ptr);
        UIPrintln("[%04X] $%04X: %s", addr, ptr, stackLogEntry);

        addr += 2;
    }

    UISetCursor(startX + (UI_CHAR_WIDTH * 60), 
        startY + stackLogHeight + (UI_LINE_HEIGHT * 6));

    UIPrintln("INSTRUCTION LOG");

    int instLogWidth = (UI_CHAR_WIDTH * STACK_PREVIEW_ENTRY_SIZE);
    int instLogHeight = (UI_LINE_HEIGHT * INSTRUCTION_LOG_LENGTH);

    UIPanel(instLogWidth + (UI_PANEL_PADDING * 2),
               instLogHeight + (UI_PANEL_PADDING * 2));

    for (int i = 0; i < GetInstructionLogSize(); ++i) {
        UIPrintln(GetInstructionLogEntry(i));
    }
}
