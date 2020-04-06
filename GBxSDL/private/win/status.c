#include "status.h"
#include "../debug.h"

#include <GBx/GBx.h>
#include <SM83/SM83.h>
#include <DUI/DUI.h>

void StatusTabRender()
{
    DUI_Style * style = DUI_GetStyle();
    int duiLineHeight = style->CharHeight + style->LinePadding;
    
    int startX, startY;
    DUI_GetCursor(&startX, &startY);

    int columnWidth = (style->CharWidth * 24);

    int column1X = startX;
    int column2X = column1X + columnWidth 
        + style->CharWidth + (style->PanelPadding * 2);
    int column3X = column2X + columnWidth
        + style->CharWidth + (style->PanelPadding * 2);

    DUI_PanelStart("Status", columnWidth, 0, false);

    DUI_Println("CPU: %s", 
        (CPU.Enabled ? "Enabled" : "Disabled"));

    DUI_Println("LCD: %s", 
        (LCDC.Enabled ? "Enabled" : "Disabled"));

    DUI_Println("APU: %s", 
        (APUC.Enabled ? "Enabled" : "Disabled"));

    DUI_Println("TAC: %s", 
        (TAC.Enabled ? "Enabled" : "Disabled"));

    DUI_PanelEnd();
    
    DUI_PanelStart("Registers", columnWidth, 0, false);

    DUI_Println("A: $%02X  F: $%02X", CPU.A, CPU.F);
    DUI_Println("B: $%02X  C: $%02X", CPU.B, CPU.C);
    DUI_Println("D: $%02X  E: $%02X", CPU.D, CPU.E);
    DUI_Println("H: $%02X  L: $%02X", CPU.H, CPU.L);
    DUI_Println("PC: $%04X", CPU.PC);
    DUI_Println("SP: $%04X", CPU.SP);
    DUI_Println("FLAGS: [%c%c%c%c]",
        (CPU.FZ ? 'Z' : '-'), 
        (CPU.FN ? 'N' : '-'), 
        (CPU.FH ? 'H' : '-'), 
        (CPU.FC ? 'C' : '-'));

    DUI_PanelEnd();

    DUI_PanelStart("Timing", columnWidth, 0, false);

    DUI_Println("DIV:  $%02X", DIV);
    DUI_Println("TMA:  $%02X", TMA);
    DUI_Println("TIMA: $%02X", TIMA);
    DUI_Println("TIMER: %d HZ", GetTimerSpeed());

    DUI_PanelEnd();

    DUI_PanelStart("Interrupts", columnWidth, 0, false);

    DUI_Println("VBlank: %s", 
        (CPU.IE.Int40 ? "Enabled" : "Disabled"));

    DUI_Println("STAT:   %s", 
        (CPU.IE.Int48 ? "Enabled" : "Disabled"));

    DUI_Println("Timer:  %s", 
        (CPU.IE.Int50 ? "Enabled" : "Disabled"));

    DUI_Println("Serial: %s", 
        (CPU.IE.Int58 ? "Enabled" : "Disabled"));

    DUI_Println("Joypad: %s", 
        (CPU.IE.Int60 ? "Enabled" : "Disabled"));

    DUI_PanelEnd();

    DUI_PanelStart("Input", columnWidth, 0, false);

    DUI_Println("JOYP: $%02X", JOYP.raw);
    DUI_Println(" DPad:    %s", (JOYP.SelectDPad ? "Selected" : "-"));
    DUI_Println(" Buttons: %s", (JOYP.SelectButtons ? "Selected" : "-"));
    DUI_Println(" A/Right:    %s", (JOYP.A ? "UP" : "DOWN"));
    DUI_Println(" B/Left:     %s", (JOYP.B ? "UP" : "DOWN"));
    DUI_Println(" Select/Up:  %s", (JOYP.Select ? "UP" : "DOWN"));
    DUI_Println(" Start/Down: %s", (JOYP.Start ? "UP" : "DOWN"));

    DUI_PanelEnd();

    DUI_MoveCursor(column2X, startY);

    DUI_PanelStart("Hardware", columnWidth, 0, false);

    DUI_Println("Device: %s", 
        (ColorEnabled ? "CGB" : (SuperEnabled ? "SGB" : "DMG")));

    DUI_Println("Clock: %d HZ", CPU.ClockSpeed);
    DUI_Println("Title: %.*s", 15, CartridgeHeader.Title);
    DUI_Println("Cartridge: %s", GetCartridgeTypeString());
    DUI_Println("ROM: %s", GetROMTypeString());
    DUI_Println("RAM: %s", GetRAMTypeString());

    DUI_PanelEnd();

    DUI_PanelStart("Video", columnWidth, 0, false);

    DUI_Println("LCD Mode: %s", GetLCDModeString(STAT.Mode));
    DUI_Println("Coincidence: %s", (STAT.Coincidence ? "True" : "False"));
    DUI_Println("LY:  $%02X (%d)", LY, LY);
    DUI_Println("LYC: $%02X (%d)", LYC, LYC);
    DUI_Println("SCX: $%02X (%d)", SCX, SCX);
    DUI_Println("SCY: $%02X (%d)", SCY, SCY);
    DUI_Println("WX:  $%02X (%d)", WX, WX);
    DUI_Println("WY:  $%02X (%d)", WY, WY);

    DUI_PanelEnd();

    DUI_MoveCursor(column3X, startY);

    DUI_PanelStart("Audio", columnWidth, 0, false);

    DUI_Println("Volume Left: %d Right: %d", 
        VolumeControl.LeftVolume, VolumeControl.RightVolume);

    DUI_Println("Tone1: %s", (APUC.Tone1Playing ? "Playing" : "Stopped"));
    DUI_Println(" Shift:        %d", Tone1.Shift);
    DUI_Println(" Negate:       %s", (Tone1.Negate ? "True" : "False"));
    DUI_Println(" Sweep:        %d", Tone1.SweepPeriod);
    DUI_Println(" Length Load:  %d", Tone1.LengthLoad);
    DUI_Println(" Duty:         %d", Tone1.Duty);
    DUI_Println(" Period:       %d", Tone1.Period);
    DUI_Println(" Env Add Mode: %s", (Tone1.EnvelopeAddMode ? "True" : "False"));
    DUI_Println(" Volume:       %d", Tone1.Volume);
    DUI_Println(" Frequency:    %d", Tone1.Frequency);
    DUI_Println(" Length:       %s", (Tone1.LengthEnabled ? "Enabled" : "Disabled"));
    DUI_Println(" Trigger:      %s", (Tone1.Trigger ? "Enabled" : "Disabled"));

    DUI_Println("Tone2: %s", (APUC.Tone2Playing ? "Playing" : "Stopped"));
    DUI_Println(" Length Load:  %d", Tone2.LengthLoad);
    DUI_Println(" Duty:         %d", Tone2.Duty);
    DUI_Println(" Period:       %d", Tone2.Period);
    DUI_Println(" Env Add Mode: %s", (Tone2.EnvelopeAddMode ? "True" : "False"));
    DUI_Println(" Volume:       %d", Tone2.Volume);
    DUI_Println(" Frequency:    %d", Tone2.Frequency);
    DUI_Println(" Length:       %s", (Tone2.LengthEnabled ? "Enabled" : "Disabled"));
    DUI_Println(" Trigger:      %s", (Tone2.Trigger ? "Enabled" : "Disabled"));

    DUI_Println("Wave: %s", (APUC.WavePlaying ? "Playing" : "Stopped"));
    DUI_Println(" DAC Power:    %d", Wave.DACPower);
    DUI_Println(" Length Load:  %d", Wave.LengthLoad);
    DUI_Println(" Volume CODE:  %d", Wave.VolumeCode);
    DUI_Println(" Frequency:    %d", Wave.Frequency);
    DUI_Println(" Length:       %s", (Wave.LengthEnabled ? "Enabled" : "Disabled"));
    DUI_Println(" Trigger:      %s", (Wave.Trigger ? "Enabled" : "Disabled"));

    DUI_Println("Noise: %s", (APUC.NoisePlaying ? "Playing" : "Stopped"));
    DUI_Println(" Length Load:  %d", Noise.LengthLoad);
    DUI_Println(" Period:       %d", Noise.Period);
    DUI_Println(" Env Add Mode: %s", (Noise.EnvelopeAddMode ? "True" : "False"));
    DUI_Println(" Volume:       %d", Noise.Volume);
    DUI_Println(" Divisor Code: %d", Noise.DivisorCode);
    DUI_Println(" LFSR W. Mode: %s", (Noise.LFSRWidthMode ? "Enabled" : "Disabled"));
    DUI_Println(" Clock Shift:  %d", Noise.ClockShift);
    DUI_Println(" Length:       %s", (Noise.LengthEnabled ? "Enabled" : "Disabled"));
    DUI_Println(" Trigger:      %s", (Noise.Trigger ? "Enabled" : "Disabled"));

    DUI_PanelEnd();
    
    return;

    int rightColumnX = startX + (style->CharWidth * 61);

    const int STACK_PREVIEW_Length = 16;
    const int STACK_PREVIEW_ENTRY_SIZE = 26;

    int stackLogWidth = (style->CharWidth * STACK_PREVIEW_ENTRY_SIZE);
    int stackLogHeight = (duiLineHeight * STACK_PREVIEW_Length);

    DUI_MoveCursor(startX + rightColumnX, startY);

    // DUI_Println("STACK [%04X]", CPU.StackBaseAddress);

    DUI_PanelStart("Stack", stackLogWidth + (style->PanelPadding * 2),
               stackLogHeight + (style->PanelPadding * 2), false);

    char stackLogEntry[32];

    uint16_t addr = CPU.SP;
    for (int i = 0; i < STACK_PREVIEW_Length; ++i) {
        if (addr == CPU.StackBaseAddress) {
            break;
        }

        uint16_t ptr = ReadWord(addr);

        SM83_Disassemble(&CPU, stackLogEntry, sizeof(stackLogEntry), ptr);
        DUI_Println("[%04X] $%04X: %s", addr, ptr, stackLogEntry);

        addr += 2;
    }

    DUI_PanelEnd();

    DUI_MoveCursor(startX + rightColumnX, 
        startY + stackLogHeight + (duiLineHeight * 6));

    DUI_Print("INSTRUCTION LOG");

    DUI_MoveCursorRelative(style->CharWidth * 3, -style->ButtonPadding);
    DUI_Checkbox("Enabled", &CPU.InstructionLoggingEnabled);

    DUI_MoveCursor(startX + rightColumnX, 
        startY + stackLogHeight + (duiLineHeight * 8));

    int instLogWidth = (style->CharWidth * STACK_PREVIEW_ENTRY_SIZE);
    int instLogHeight = (duiLineHeight * INSTRUCTION_LOG_LENGTH);

    DUI_PanelStart("Instruction Log", instLogWidth + (style->PanelPadding * 2),
               instLogHeight + (style->PanelPadding * 2), false);

    for (int i = 0; i < INSTRUCTION_LOG_LENGTH; ++i) {
        DUI_Println(SM83_GetInstructionLogEntry(&CPU, i));
    }

    DUI_PanelEnd();
}
