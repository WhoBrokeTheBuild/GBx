#include "status.h"
#include "../debug.h"

#include <GBx/GBx.h>
#include <SM83/SM83.h>
#include <DUI/DUI.h>

void StatusTabRender()
{
    DUI_Style * style = DUI_GetStyle();
    int duiLineHeight = style->CharHeight + style->LinePadding;
    
    SDL_Rect * bounds = DUI_GetPanelBounds();

    int startX, startY;
    DUI_GetCursor(&startX, &startY);

    int column1Width = (style->CharWidth * 18);
    int column2Width = (style->CharWidth * 26);
    int column3Width = (style->CharWidth * 26);
    int column4Width = (style->CharWidth * 26);

    int column1X = startX;

    int column2X = column1X + column1Width 
        + style->CharWidth + (style->PanelPadding * 2);

    int column3X = column2X + column2Width
        + style->CharWidth + (style->PanelPadding * 2);

    int column4X = bounds->x + bounds->w 
        - column4Width - (style->PanelPadding * 2);

    DUI_PanelStart("Status", column1Width, 0, false);

    DUI_Println("CPU: %s", 
        (CPU.Enabled ? "Enabled" : "Disabled"));

    DUI_Println("LCD: %s", 
        (LCDC.Enabled ? "Enabled" : "Disabled"));

    DUI_Println("APU: %s", 
        (APUC.Enabled ? "Enabled" : "Disabled"));

    DUI_Println("TAC: %s", 
        (TAC.Enabled ? "Enabled" : "Disabled"));

    DUI_PanelEnd();
    
    DUI_PanelStart("Registers", column1Width, 0, false);

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

    DUI_PanelStart("Timing", column1Width, 0, false);

    DUI_Println("DIV:  $%02X", DIV);
    DUI_Println("TMA:  $%02X", TMA);
    DUI_Println("TIMA: $%02X", TIMA);
    DUI_Println("TIMER: %d HZ", GetTimerSpeed());

    DUI_PanelEnd();

    DUI_PanelStart("Interrupts", column1Width, 0, false);

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

    DUI_PanelStart("Input", column1Width, 0, false);

    DUI_Println("JOYP: $%02X", JOYP.raw);
    DUI_Println(" DPad:    %s", (JOYP.SelectDPad ? "Selected" : "-"));
    DUI_Println(" Buttons: %s", (JOYP.SelectButtons ? "Selected" : "-"));
    DUI_Println(" A/Right:    %s", (JOYP.A ? "UP" : "DOWN"));
    DUI_Println(" B/Left:     %s", (JOYP.B ? "UP" : "DOWN"));
    DUI_Println(" Select/Up:  %s", (JOYP.Select ? "UP" : "DOWN"));
    DUI_Println(" Start/Down: %s", (JOYP.Start ? "UP" : "DOWN"));

    DUI_PanelEnd();

    DUI_MoveCursor(column2X, startY);

    DUI_PanelStart("Hardware", column2Width, 0, false);

    DUI_Println("Device: %s", 
        (ColorEnabled ? "CGB" : (SuperEnabled ? "SGB" : "DMG")));

    DUI_Println("Clock: %d HZ", CPU.ClockSpeed);
    DUI_Println("Title: %.*s", 15, CartridgeHeader.Title);
    DUI_Println("Cartridge: %s", GetCartridgeTypeString());
    DUI_Println("ROM: %s", GetROMTypeString());
    DUI_Println("RAM: %s", GetRAMTypeString());

    DUI_PanelEnd();

    DUI_PanelStart("Video", column2Width, 0, false);

    DUI_Println("LCD Mode: %s", GetLCDModeString(STAT.Mode));

    DUI_Println("Window: %s", 
        (LCDC.WindowDisplayEnabled ? "Enabled" : "Disabled"));

    DUI_Newline();

    DUI_Println("LY:  $%02X (%d)", LY, LY);
    DUI_Println("LYC: $%02X (%d)", LYC, LYC);
    DUI_Println("SCX: $%02X (%d)", SCX, SCX);
    DUI_Println("SCY: $%02X (%d)", SCY, SCY);
    DUI_Println("WX:  $%02X (%d)", WX, WX);
    DUI_Println("WY:  $%02X (%d)", WY, WY);

    DUI_Newline();

    DUI_Println("Coincidence: %s", 
        (STAT.Coincidence ? "LYC == LY" : "LYC != LY"));

    DUI_Newline();

    DUI_Println("STAT Interrupts");

    DUI_Println(" HBlank:       %s", 
        (STAT.IntHBlank ? "Enabled" : "Disabled"));

    DUI_Println(" VBlank:       %s", 
        (STAT.IntVBlank ? "Enabled" : "Disabled"));

    DUI_Println(" SearchSprite: %s", 
        (STAT.IntSearchSprite ? "Enabled" : "Disabled"));

    DUI_Println(" Coincidence:  %s", 
        (STAT.IntCoincidence ? "Enabled" : "Disabled"));

    DUI_Newline();

    DUI_Println("Sprite Size: %s", 
        GetSpriteSizeString(LCDC.SpriteSize));

    DUI_Println("Tile Data: %s",
        GetTileDataRangeString(LCDC.TileDataSelect));

    DUI_Println("BG Map:    %s", 
        GetTileMapRangeString(LCDC.BGTileMapSelect));

    DUI_Println("Win Map:   %s", 
        GetTileMapRangeString(LCDC.WindowTileMapSelect));

    DUI_PanelEnd();

    DUI_MoveCursor(column3X, startY);

    DUI_PanelStart("Audio", column3Width, 0, false);

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

    const int STACK_PREVIEW_LENGTH = 8;
    const int STACK_PREVIEW_ENTRY_SIZE = 26;

    int stackLogWidth = column4Width;
    int stackLogHeight = (duiLineHeight * STACK_PREVIEW_LENGTH)
        + (style->PanelPadding * 2);

    DUI_MoveCursor(column4X, startY);

    DUI_Println("STACK [%04X]", CPU.StackBaseAddress);

    DUI_PanelStart(NULL, stackLogWidth, stackLogHeight, false);

    char stackLogEntry[STACK_PREVIEW_ENTRY_SIZE + 1];

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

    DUI_PanelEnd();

    DUI_MoveCursorRelative(0, style->LinePadding);

    int tmpY;
    DUI_GetCursor(NULL, &tmpY);

    DUI_Print("Instruction Log");

    DUI_MoveCursor(bounds->x + bounds->w 
        - (style->CharWidth * 9) - (style->ButtonPadding * 2), tmpY - style->ButtonPadding);

    DUI_Checkbox("Enabled", &CPU.InstructionLoggingEnabled);

    DUI_MoveCursor(column4X, 
        tmpY + style->CharHeight + (style->LinePadding * 2));

    const int INSTRUCTION_LOG_VIEW_LENGTH = 28;

    int instLogWidth = column4Width;
    int instLogHeight = (duiLineHeight * INSTRUCTION_LOG_VIEW_LENGTH);

    DUI_PanelStart(NULL, instLogWidth, instLogHeight, false);

    for (int i = 0; i < INSTRUCTION_LOG_VIEW_LENGTH; ++i) {
        DUI_Println(SM83_GetInstructionLogEntry(&CPU, i));
    }

    DUI_PanelEnd();
}
