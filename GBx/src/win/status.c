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
#include <DUI/DUI.h>

void StatusTabRender()
{
    DUI_Style * style = DUI_GetStyle();
    
    DUI_MoveCursor(DEBUG_CONTENT_X, DEBUG_CONTENT_Y);
    DUI_Panel(DEBUG_CONTENT_WIDTH, DEBUG_CONTENT_HEIGHT);

    int startX, startY;
    DUI_GetCursor(&startX, &startY);

    DUI_Println("--STATUS--------");
    DUI_Println("CPU: %s", 
        (CPUEnabled ? "ENABLED" : "DISABLED"));

    DUI_Println("LCD: %s", 
        (LCDC.Enabled ? "ENABLED" : "DISABLED"));

    DUI_Println("APU: %s", 
        (APUC.Enabled ? "ENABLED" : "DISABLED"));

    DUI_Println("TAC: %s", 
        (TAC.Enabled ? "ENABLED" : "DISABLED"));
    
    DUI_Newline();

    DUI_Println("--REGISTERS-----");
    DUI_Println("A: $%02X  F: $%02X", R.A, R.F);
    DUI_Println("B: $%02X  C: $%02X", R.B, R.C);
    DUI_Println("D: $%02X  E: $%02X", R.D, R.E);
    DUI_Println("H: $%02X  L: $%02X", R.H, R.L);
    DUI_Println("PC: $%04X", R.PC);
    DUI_Println("SP: $%04X", R.SP);
    DUI_Println("FLAGS: [%c%c%c%c]",
        (R.FZ ? 'Z' : '-'), 
        (R.FN ? 'N' : '-'), 
        (R.FH ? 'H' : '-'), 
        (R.FC ? 'C' : '-'));

    DUI_Newline();

    DUI_Println("--TIMING--------");

    DUI_Println("DIV:  $%02X", DIV);
    DUI_Println("TMA:  $%02X", TMA);
    DUI_Println("TIMA: $%02X", TIMA);
    DUI_Println("TIMER: %d HZ", GetTimerSpeed());

    DUI_Newline();

    DUI_Println("--INTERRUPTS----");

    DUI_Println("VBLANK: %s", 
        (IE.VBlank ? "ENABLED" : "DISABLED"));

    DUI_Println("STAT:   %s", 
        (IE.STAT ? "ENABLED" : "DISABLED"));

    DUI_Println("TIMER:  %s", 
        (IE.Timer ? "ENABLED" : "DISABLED"));

    DUI_Println("SERIAL: %s", 
        (IE.Serial ? "ENABLED" : "DISABLED"));

    DUI_Println("JOYPAD: %s", 
        (IE.Joypad ? "ENABLED" : "DISABLED"));

    DUI_Newline();

    DUI_Println("--VIDEO---------");

    DUI_Println("LCD MODE: %s", GetLCDModeString(STAT.Mode));
    DUI_Println("LY:  $%02X (%d)", LY, LY);
    DUI_Println("LYC: $%02X (%d)", LYC, LYC);
    DUI_Println("SCX: $%02X (%d)", SCX, SCX);
    DUI_Println("SCY: $%02X (%d)", SCY, SCY);
    DUI_Println("WX:  $%02X (%d)", WX, WX);
    DUI_Println("WY:  $%02X (%d)", WY, WY);

    DUI_Newline();

    DUI_Println("--HARDWARE------");

    DUI_Println("DEVICE: %s", 
        (ColorEnabled ? "CGB" : (SuperEnabled ? "SGB" : "DMG")));

    DUI_Println("CLOCK: %d HZ", ClockSpeed);
    DUI_Println("TITLE: %.*s", 15, CartridgeHeader.Title);
    DUI_Println("CARTRIDGE: %s", GetCartridgeTypeString());
    DUI_Println("ROM: %s", GetROMTypeString());
    DUI_Println("RAM: %s", GetRAMTypeString());

    DUI_MoveCursor(startX + (style->CharSize * 20), startY);

    DUI_Println("--AUDIO---------");

    DUI_Println("VOLUME L: %d R: %d", 
        VolumeControl.LeftVolume, VolumeControl.RightVolume);

    DUI_Println("TONE1: %s", (APUC.Tone1Playing ? "PLAYING" : "STOPPED"));
    DUI_Println(" SHIFT:   %d", Tone1.Shift);
    DUI_Println(" NEGATE:  %s", (Tone1.Negate ? "TRUE" : "FALSE"));
    DUI_Println(" SWEEP:   %d", Tone1.SweepPeriod);
    DUI_Println(" LENLOAD: %d", Tone1.LengthLoad);
    DUI_Println(" DUTY:    %d", Tone1.Duty);
    DUI_Println(" PERIOD:  %d", Tone1.Period);
    DUI_Println(" ENVADDM: %s", (Tone1.EnvelopeAddMode ? "TRUE" : "FALSE"));
    DUI_Println(" VOLUME:  %d", Tone1.Volume);
    DUI_Println(" FREQ:    %d", Tone1.Frequency);
    DUI_Println(" LENGTH:  %s", (Tone1.LengthEnabled ? "ENABLED" : "DISABLED"));
    DUI_Println(" TRIG:    %s", (Tone1.Trigger ? "ENABLED" : "DISABLED"));

    DUI_Newline();

    DUI_Println("TONE2: %s", (APUC.Tone2Playing ? "PLAYING" : "STOPPED"));
    DUI_Println(" LENLOAD: %d", Tone2.LengthLoad);
    DUI_Println(" DUTY:    %d", Tone2.Duty);
    DUI_Println(" PERIOD:  %d", Tone2.Period);
    DUI_Println(" ENVADDM: %s", (Tone2.EnvelopeAddMode ? "TRUE" : "FALSE"));
    DUI_Println(" VOLUME:  %d", Tone2.Volume);
    DUI_Println(" FREQ:    %d", Tone2.Frequency);
    DUI_Println(" LENGTH:  %s", (Tone2.LengthEnabled ? "ENABLED" : "DISABLED"));
    DUI_Println(" TRIG:    %s", (Tone2.Trigger ? "ENABLED" : "DISABLED"));

    DUI_Newline();

    DUI_Println("WAVE: %s", (APUC.WavePlaying ? "PLAYING" : "STOPPED"));
    DUI_Println(" DACPWR:  %d", Wave.DACPower);
    DUI_Println(" LENLOAD: %d", Wave.LengthLoad);
    DUI_Println(" VOLCODE: %d", Wave.VolumeCode);
    DUI_Println(" FREQ:    %d", Wave.Frequency);
    DUI_Println(" LENGTH:  %s", (Wave.LengthEnabled ? "ENABLED" : "DISABLED"));
    DUI_Println(" TRIG:    %s", (Wave.Trigger ? "ENABLED" : "DISABLED"));

    DUI_Newline();

    DUI_Println("NOISE: %s", (APUC.NoisePlaying ? "PLAYING" : "STOPPED"));
    DUI_Println(" LENLOAD: %d", Noise.LengthLoad);
    DUI_Println(" PERIOD:  %d", Noise.Period);
    DUI_Println(" ENVADDM: %s", (Noise.EnvelopeAddMode ? "TRUE" : "FALSE"));
    DUI_Println(" VOLUME:  %d", Noise.Volume);
    DUI_Println(" DIVCODE: %d", Noise.DivisorCode);
    DUI_Println(" LFSRWID: %s", (Noise.LFSRWidthMode ? "ENABLED" : "DISABLED"));
    DUI_Println(" CLSHIFT: %d", Noise.ClockShift);
    DUI_Println(" LENGTH:  %s", (Noise.LengthEnabled ? "ENABLED" : "DISABLED"));
    DUI_Println(" TRIG:    %s", (Noise.Trigger ? "ENABLED" : "DISABLED"));


    const int STACK_PREVIEW_LENGTH = 16;
    const int STACK_PREVIEW_ENTRY_SIZE = 26;

    int stackLogWidth = (style->CharSize * STACK_PREVIEW_ENTRY_SIZE);
    int stackLogHeight = (style->LineHeight * STACK_PREVIEW_LENGTH);

    DUI_MoveCursor(startX + (style->CharSize * 60), startY);

    DUI_Println("STACK [%04X]", StackBaseAddress);

    DUI_Panel(stackLogWidth + (style->PanelPadding * 2),
               stackLogHeight + (style->PanelPadding * 2));

    char stackLogEntry[32];

    word addr = R.SP;
    for (int i = 0; i < STACK_PREVIEW_LENGTH; ++i) {
        if (addr == StackBaseAddress) {
            break;
        }

        word ptr = ReadWord(addr);

        Disassemble(stackLogEntry, sizeof(stackLogEntry), ptr);
        DUI_Println("[%04X] $%04X: %s", addr, ptr, stackLogEntry);

        addr += 2;
    }

    DUI_MoveCursor(startX + (style->CharSize * 60), 
        startY + stackLogHeight + (style->LineHeight * 6));

    DUI_Println("INSTRUCTION LOG");

    int instLogWidth = (style->CharSize * STACK_PREVIEW_ENTRY_SIZE);
    int instLogHeight = (style->LineHeight * INSTRUCTION_LOG_LENGTH);

    DUI_Panel(instLogWidth + (style->PanelPadding * 2),
               instLogHeight + (style->PanelPadding * 2));

    for (int i = 0; i < GetInstructionLogSize(); ++i) {
        DUI_Println(GetInstructionLogEntry(i));
    }
}
