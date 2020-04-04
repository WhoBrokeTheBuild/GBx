#include "status.h"
#include "../debug.h"

#include <GBx/GBx.h>
#include <SM83/SM83.h>
#include <DUI/DUI.h>

void StatusTabRender()
{
    DUI_Style * style = DUI_GetStyle();
    
    DUI_MoveCursor(DEBUG_CONTENT_X, DEBUG_CONTENT_Y);
    DUI_Panel(DEBUG_CONTENT_WIDTH, DEBUG_CONTENT_HEIGHT);

    int startX, startY;
    DUI_GetCursor(&startX, &startY);

    int columnWidth = (style->CharSize * 25);

    DUI_Println("--------STATUS---------");
    DUI_Println("CPU: %s", 
        (CPU.Enabled ? "ENABLED" : "DISABLED"));

    DUI_Println("LCD: %s", 
        (LCDC.Enabled ? "ENABLED" : "DISABLED"));

    DUI_Println("APU: %s", 
        (APUC.Enabled ? "ENABLED" : "DISABLED"));

    DUI_Println("TAC: %s", 
        (TAC.Enabled ? "ENABLED" : "DISABLED"));
    
    DUI_Newline();

    DUI_Println("-------REGISTERS-------");
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

    DUI_Newline();

    DUI_Println("--------TIMING---------");
    DUI_Println("DIV:  $%02X", DIV);
    DUI_Println("TMA:  $%02X", TMA);
    DUI_Println("TIMA: $%02X", TIMA);
    DUI_Println("TIMER: %d HZ", GetTimerSpeed());

    DUI_Newline();

    DUI_Println("------INTERRUPTS-------");

    DUI_Println("VBLANK: %s", 
        (CPU.IE.Int40 ? "ENABLED" : "DISABLED"));

    DUI_Println("STAT:   %s", 
        (CPU.IE.Int48 ? "ENABLED" : "DISABLED"));

    DUI_Println("TIMER:  %s", 
        (CPU.IE.Int50 ? "ENABLED" : "DISABLED"));

    DUI_Println("SERIAL: %s", 
        (CPU.IE.Int58 ? "ENABLED" : "DISABLED"));

    DUI_Println("JOYPAD: %s", 
        (CPU.IE.Int60 ? "ENABLED" : "DISABLED"));

    DUI_Newline();

    DUI_Println("---------VIDEO---------");

    DUI_Println("LCD MODE: %s", GetLCDModeString(STAT.Mode));
    DUI_Println("LY:  $%02X (%d)", LY, LY);
    DUI_Println("LYC: $%02X (%d)", LYC, LYC);
    DUI_Println("SCX: $%02X (%d)", SCX, SCX);
    DUI_Println("SCY: $%02X (%d)", SCY, SCY);
    DUI_Println("WX:  $%02X (%d)", WX, WX);
    DUI_Println("WY:  $%02X (%d)", WY, WY);

    DUI_Newline();

    DUI_Println("---------INPUT---------");

    DUI_Println("JOYP: $%02X", JOYP.raw);
    DUI_Println(" DPAD:    %s", (JOYP.SelectDPad ? "SELECTED" : "-"));
    DUI_Println(" Buttons: %s", (JOYP.SelectButtons ? "SELECTED" : "-"));
    DUI_Println(" A/RIGHT:    %s", (JOYP.A ? "UP" : "DOWN"));
    DUI_Println(" B/LEFT:     %s", (JOYP.B ? "UP" : "DOWN"));
    DUI_Println(" SELECT/UP:  %s", (JOYP.Select ? "UP" : "DOWN"));
    DUI_Println(" START/DOWN: %s", (JOYP.Start ? "UP" : "DOWN"));

    DUI_Newline();

    DUI_Println("-------HARDWARE--------");

    DUI_Println("DEVICE: %s", 
        (ColorEnabled ? "CGB" : (SuperEnabled ? "SGB" : "DMG")));

    DUI_Println("CLOCK: %d HZ", CPU.ClockSpeed);
    DUI_Println("TITLE: %.*s", 15, CartridgeHeader.Title);
    DUI_Println("CARTRIDGE: %s", GetCartridgeTypeString());
    DUI_Println("ROM: %s", GetROMTypeString());
    DUI_Println("RAM: %s", GetRAMTypeString());

    DUI_MoveCursor(startX + columnWidth, startY);

    DUI_Println("---------AUDIO---------");

    DUI_Println("VOLUME LEFT: %d RIGHT: %d", 
        VolumeControl.LeftVolume, VolumeControl.RightVolume);

    DUI_Println("TONE1: %s", (APUC.Tone1Playing ? "PLAYING" : "STOPPED"));
    DUI_Println(" SHIFT:        %d", Tone1.Shift);
    DUI_Println(" NEGATE:       %s", (Tone1.Negate ? "TRUE" : "FALSE"));
    DUI_Println(" SWEEP:        %d", Tone1.SweepPeriod);
    DUI_Println(" LENGTH LOAD:  %d", Tone1.LengthLoad);
    DUI_Println(" DUTY:         %d", Tone1.Duty);
    DUI_Println(" PERIOD:       %d", Tone1.Period);
    DUI_Println(" ENV ADD MODE: %s", (Tone1.EnvelopeAddMode ? "TRUE" : "FALSE"));
    DUI_Println(" VOLUME:       %d", Tone1.Volume);
    DUI_Println(" FREQUENCY:    %d", Tone1.Frequency);
    DUI_Println(" LENGTH:       %s", (Tone1.LengthEnabled ? "ENABLED" : "DISABLED"));
    DUI_Println(" TRIGGER:      %s", (Tone1.Trigger ? "ENABLED" : "DISABLED"));

    DUI_Newline();

    DUI_Println("TONE2: %s", (APUC.Tone2Playing ? "PLAYING" : "STOPPED"));
    DUI_Println(" LENGTH LOAD:  %d", Tone2.LengthLoad);
    DUI_Println(" DUTY:         %d", Tone2.Duty);
    DUI_Println(" PERIOD:       %d", Tone2.Period);
    DUI_Println(" ENV ADD MODE: %s", (Tone2.EnvelopeAddMode ? "TRUE" : "FALSE"));
    DUI_Println(" VOLUME:       %d", Tone2.Volume);
    DUI_Println(" FREQUENCY:    %d", Tone2.Frequency);
    DUI_Println(" LENGTH:       %s", (Tone2.LengthEnabled ? "ENABLED" : "DISABLED"));
    DUI_Println(" TRIGGER:      %s", (Tone2.Trigger ? "ENABLED" : "DISABLED"));

    DUI_Newline();

    DUI_Println("WAVE: %s", (APUC.WavePlaying ? "PLAYING" : "STOPPED"));
    DUI_Println(" DAC POWER:    %d", Wave.DACPower);
    DUI_Println(" LENGTH LOAD:  %d", Wave.LengthLoad);
    DUI_Println(" VOLUME CODE:  %d", Wave.VolumeCode);
    DUI_Println(" FREQUENCY:    %d", Wave.Frequency);
    DUI_Println(" LENGTH:       %s", (Wave.LengthEnabled ? "ENABLED" : "DISABLED"));
    DUI_Println(" TRIGGER:      %s", (Wave.Trigger ? "ENABLED" : "DISABLED"));

    DUI_Newline();

    DUI_Println("NOISE: %s", (APUC.NoisePlaying ? "PLAYING" : "STOPPED"));
    DUI_Println(" LENGTH LOAD:  %d", Noise.LengthLoad);
    DUI_Println(" PERIOD:       %d", Noise.Period);
    DUI_Println(" ENV ADD MODE: %s", (Noise.EnvelopeAddMode ? "TRUE" : "FALSE"));
    DUI_Println(" VOLUME:       %d", Noise.Volume);
    DUI_Println(" DIVISOR CODE: %d", Noise.DivisorCode);
    DUI_Println(" LFSR W. MODE: %s", (Noise.LFSRWidthMode ? "ENABLED" : "DISABLED"));
    DUI_Println(" CLOCK SHIFT:  %d", Noise.ClockShift);
    DUI_Println(" LENGTH:       %s", (Noise.LengthEnabled ? "ENABLED" : "DISABLED"));
    DUI_Println(" TRIGGER:      %s", (Noise.Trigger ? "ENABLED" : "DISABLED"));

    int rightColumnX = startX + (style->CharSize * 61);

    const int STACK_PREVIEW_LENGTH = 16;
    const int STACK_PREVIEW_ENTRY_SIZE = 26;

    int stackLogWidth = (style->CharSize * STACK_PREVIEW_ENTRY_SIZE);
    int stackLogHeight = (style->LineHeight * STACK_PREVIEW_LENGTH);

    DUI_MoveCursor(startX + rightColumnX, startY);

    DUI_Println("STACK [%04X]", CPU.StackBaseAddress);

    DUI_Panel(stackLogWidth + (style->PanelPadding * 2),
               stackLogHeight + (style->PanelPadding * 2));

    char stackLogEntry[32];

    uint16_t addr = CPU.SP;
    for (int i = 0; i < STACK_PREVIEW_LENGTH; ++i) {
        if (addr == CPU.StackBaseAddress) {
            break;
        }

        uint16_t ptr = ReadWord(addr);

        SM83_Disassemble(&CPU, stackLogEntry, sizeof(stackLogEntry), ptr);
        DUI_Println("[%04X] $%04X: %s", addr, ptr, stackLogEntry);

        addr += 2;
    }

    DUI_MoveCursor(startX + rightColumnX, 
        startY + stackLogHeight + (style->LineHeight * 6));

    DUI_Print("INSTRUCTION LOG");

    DUI_MoveCursorRelative(style->CharSize * 3, -style->ButtonPadding);
    DUI_Checkbox("ENABLED", &CPU.InstructionLoggingEnabled);

    DUI_MoveCursor(startX + rightColumnX, 
        startY + stackLogHeight + (style->LineHeight * 8));

    int instLogWidth = (style->CharSize * STACK_PREVIEW_ENTRY_SIZE);
    int instLogHeight = (style->LineHeight * INSTRUCTION_LOG_LENGTH);

    DUI_Panel(instLogWidth + (style->PanelPadding * 2),
               instLogHeight + (style->PanelPadding * 2));

    for (int i = 0; i < INSTRUCTION_LOG_LENGTH; ++i) {
        DUI_Println(SM83_GetInstructionLogEntry(&CPU, i));
    }
}
