#include "status.h"
#include "../debug.h"

#include <GBx/GBx.h>
#include <SM83/SM83.h>
#include <DUI/DUI.h>

void StatusTabRender(gbx_t * ctx)
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
        (SM83_GetEnabled(ctx->CPU) ? "Enabled" : "Disabled"));

    DUI_Println("LCD: %s", 
        (ctx->LCDC.Enabled ? "Enabled" : "Disabled"));

    DUI_Println("APU: %s", 
        (ctx->APUC.Enabled ? "Enabled" : "Disabled"));

    DUI_Println("TAC: %s", 
        (ctx->TAC.Enabled ? "Enabled" : "Disabled"));

    DUI_PanelEnd();
    
    DUI_PanelStart("Registers", column1Width, 0, false);

    DUI_Println("A: $%02X  F: $%02X", ctx->CPU->A, ctx->CPU->F);
    DUI_Println("B: $%02X  C: $%02X", ctx->CPU->B, ctx->CPU->C);
    DUI_Println("D: $%02X  E: $%02X", ctx->CPU->D, ctx->CPU->E);
    DUI_Println("H: $%02X  L: $%02X", ctx->CPU->H, ctx->CPU->L);
    DUI_Println("PC: $%04X", ctx->CPU->PC);
    DUI_Println("SP: $%04X", ctx->CPU->SP);
    DUI_Println("FLAGS: [%c%c%c%c]",
        (ctx->CPU->FZ ? 'Z' : '-'), 
        (ctx->CPU->FN ? 'N' : '-'), 
        (ctx->CPU->FH ? 'H' : '-'), 
        (ctx->CPU->FC ? 'C' : '-'));

    DUI_PanelEnd();

    DUI_PanelStart("Timing", column1Width, 0, false);

    DUI_Println("DIV:  $%02X", ctx->DIV);
    DUI_Println("TMA:  $%02X", ctx->TMA);
    DUI_Println("TIMA: $%02X", ctx->TIMA);
    DUI_Println("TIMER: %d HZ", GBx_GetTimerSpeed(ctx));

    DUI_PanelEnd();

    DUI_PanelStart("Interrupts", column1Width, 0, false);

    DUI_Println("IME: %s",
        (ctx->CPU->IME ? "Enabled" : "Disabled"));

    DUI_Println("VBlank: %s", 
        (ctx->CPU->IE.Int40 ? "Enabled" : "Disabled"));

    DUI_Println("STAT:   %s", 
        (ctx->CPU->IE.Int48 ? "Enabled" : "Disabled"));

    DUI_Println("Timer:  %s", 
        (ctx->CPU->IE.Int50 ? "Enabled" : "Disabled"));

    DUI_Println("Serial: %s", 
        (ctx->CPU->IE.Int58 ? "Enabled" : "Disabled"));

    DUI_Println("Joypad: %s", 
        (ctx->CPU->IE.Int60 ? "Enabled" : "Disabled"));

    DUI_PanelEnd();

    DUI_PanelStart("Input", column1Width, 0, false);

    DUI_Println("JOYP: $%02X", ctx->JOYP.raw);
    DUI_Println(" DPad:    %s", (ctx->JOYP.SelectDPad ? "-" : "Selected"));
    DUI_Println(" Buttons: %s", (ctx->JOYP.SelectButtons ? "-" : "Selected"));
    DUI_Println(" A/Right:    %s", (ctx->JOYP.A ? "UP" : "DOWN"));
    DUI_Println(" B/Left:     %s", (ctx->JOYP.B ? "UP" : "DOWN"));
    DUI_Println(" Select/Up:  %s", (ctx->JOYP.Select ? "UP" : "DOWN"));
    DUI_Println(" Start/Down: %s", (ctx->JOYP.Start ? "UP" : "DOWN"));

    DUI_PanelEnd();

    DUI_MoveCursor(column2X, startY);

    DUI_PanelStart("Hardware", column2Width, 0, false);

    DUI_Println("Device: %s", 
        (ctx->ColorEnabled ? "CGB" : (ctx->SuperEnabled ? "SGB" : "DMG")));

    DUI_Println("Clock: %d HZ", SM83_GetClockSpeed(ctx->CPU));
    DUI_Println("Title: %.*s", 15, ctx->CartridgeHeader.Title);
    DUI_Println("Cartridge Type:");
    DUI_Println(" %s", GBx_GetCartridgeTypeString(ctx->CartridgeHeader.CartridgeType));
    DUI_Println("ROM: %s", GBx_GetROMTypeString(ctx->CartridgeHeader.ROMType));
    DUI_Println("RAM: %s", GBx_GetRAMTypeString(ctx->CartridgeHeader.RAMType));
    DUI_Println("ROM Bank:  %d", ctx->ROMBank);
    DUI_Println("SRAM Bank: %d", ctx->SRAMBank);
    DUI_Println("WRAM Bank: %d", ctx->WRAMBank);

    DUI_PanelEnd();

    DUI_PanelStart("Video", column2Width, 0, false);

    DUI_Println("LCD Mode: %s", GBx_GetPPUModeString(ctx->STAT.Mode));

    DUI_Println("Window: %s", 
        (ctx->LCDC.WindowDisplayEnabled ? "Enabled" : "Disabled"));

    DUI_Newline();

    DUI_Println("LY:  $%02X (%d)", ctx->LY, ctx->LY);
    DUI_Println("LYC: $%02X (%d)", ctx->LYC, ctx->LYC);
    DUI_Println("SCX: $%02X (%d)", ctx->SCX, ctx->SCX);
    DUI_Println("SCY: $%02X (%d)", ctx->SCY, ctx->SCY);
    DUI_Println("WX:  $%02X (%d)", ctx->WX, ctx->WX);
    DUI_Println("WY:  $%02X (%d)", ctx->WY, ctx->WY);

    DUI_Newline();

    DUI_Println("Coincidence: %s", 
        (ctx->STAT.Coincidence ? "LYC == LY" : "LYC != LY"));

    DUI_Newline();

    DUI_Println("STAT Interrupts");

    DUI_Println(" HBlank:       %s", 
        (ctx->STAT.IntHBlank ? "Enabled" : "Disabled"));

    DUI_Println(" VBlank:       %s", 
        (ctx->STAT.IntVBlank ? "Enabled" : "Disabled"));

    DUI_Println(" SearchSprite: %s", 
        (ctx->STAT.IntSearchSprite ? "Enabled" : "Disabled"));

    DUI_Println(" Coincidence:  %s", 
        (ctx->STAT.IntCoincidence ? "Enabled" : "Disabled"));

    DUI_Newline();

    DUI_Println("Sprite Size: %s", 
        GBx_GetSpriteSizeString(ctx->LCDC.SpriteSize));

    DUI_Println("Tile Data: %s",
        GBx_GetTileDataRangeString(ctx->LCDC.TileDataSelect));

    DUI_Println("BG Map:    %s", 
        GBx_GetTileMapRangeString(ctx->LCDC.BGTileMapSelect));

    DUI_Println("Win Map:   %s", 
        GBx_GetTileMapRangeString(ctx->LCDC.WindowTileMapSelect));

    DUI_PanelEnd();

    DUI_MoveCursor(column3X, startY);

    DUI_PanelStart("Audio", column3Width, 0, false);

    DUI_Println("Volume Left: %d Right: %d", 
        ctx->VolumeControl.LeftVolume, ctx->VolumeControl.RightVolume);

    DUI_Println("Tone1: %s", (ctx->APUC.Tone1Playing ? "Playing" : "Stopped"));
    DUI_Println(" Shift:        %d", ctx->Tone1.Shift);
    DUI_Println(" Negate:       %s", (ctx->Tone1.Negate ? "True" : "False"));
    DUI_Println(" Sweep:        %d", ctx->Tone1.SweepPeriod);
    DUI_Println(" Length Load:  %d", ctx->Tone1.LengthLoad);
    DUI_Println(" Duty:         %d", ctx->Tone1.Duty);
    DUI_Println(" Period:       %d", ctx->Tone1.Period);
    DUI_Println(" Env Add Mode: %s", (ctx->Tone1.EnvelopeAddMode ? "True" : "False"));
    DUI_Println(" Volume:       %d", ctx->Tone1.Volume);
    DUI_Println(" Frequency:    %d", ctx->Tone1.Frequency);
    DUI_Println(" Length:       %s", (ctx->Tone1.LengthEnabled ? "Enabled" : "Disabled"));
    DUI_Println(" Trigger:      %s", (ctx->Tone1.Trigger ? "Enabled" : "Disabled"));

    DUI_Println("Tone2: %s", (ctx->APUC.Tone2Playing ? "Playing" : "Stopped"));
    DUI_Println(" Length Load:  %d", ctx->Tone2.LengthLoad);
    DUI_Println(" Duty:         %d", ctx->Tone2.Duty);
    DUI_Println(" Period:       %d", ctx->Tone2.Period);
    DUI_Println(" Env Add Mode: %s", (ctx->Tone2.EnvelopeAddMode ? "True" : "False"));
    DUI_Println(" Volume:       %d", ctx->Tone2.Volume);
    DUI_Println(" Frequency:    %d", ctx->Tone2.Frequency);
    DUI_Println(" Length:       %s", (ctx->Tone2.LengthEnabled ? "Enabled" : "Disabled"));
    DUI_Println(" Trigger:      %s", (ctx->Tone2.Trigger ? "Enabled" : "Disabled"));

    DUI_Println("Wave: %s", (ctx->APUC.WavePlaying ? "Playing" : "Stopped"));
    DUI_Println(" DAC Power:    %d", ctx->Wave.DACPower);
    DUI_Println(" Length Load:  %d", ctx->Wave.LengthLoad);
    DUI_Println(" Volume CODE:  %d", ctx->Wave.VolumeCode);
    DUI_Println(" Frequency:    %d", ctx->Wave.Frequency);
    DUI_Println(" Length:       %s", (ctx->Wave.LengthEnabled ? "Enabled" : "Disabled"));
    DUI_Println(" Trigger:      %s", (ctx->Wave.Trigger ? "Enabled" : "Disabled"));

    DUI_Println("Noise: %s", (ctx->APUC.NoisePlaying ? "Playing" : "Stopped"));
    DUI_Println(" Length Load:  %d", ctx->Noise.LengthLoad);
    DUI_Println(" Period:       %d", ctx->Noise.Period);
    DUI_Println(" Env Add Mode: %s", (ctx->Noise.EnvelopeAddMode ? "True" : "False"));
    DUI_Println(" Volume:       %d", ctx->Noise.Volume);
    DUI_Println(" Divisor Code: %d", ctx->Noise.DivisorCode);
    DUI_Println(" LFSR W. Mode: %s", (ctx->Noise.LFSRWidthMode ? "Enabled" : "Disabled"));
    DUI_Println(" Clock Shift:  %d", ctx->Noise.ClockShift);
    DUI_Println(" Length:       %s", (ctx->Noise.LengthEnabled ? "Enabled" : "Disabled"));
    DUI_Println(" Trigger:      %s", (ctx->Noise.Trigger ? "Enabled" : "Disabled"));

    DUI_PanelEnd();

    const int STACK_PREVIEW_LENGTH = 8;
    const int STACK_PREVIEW_ENTRY_SIZE = 26;

    int stackLogWidth = column4Width;
    int stackLogHeight = (duiLineHeight * STACK_PREVIEW_LENGTH)
        + (style->PanelPadding * 2);

    DUI_MoveCursor(column4X, startY);

    DUI_Println("STACK [%04X]", SM83_GetStackBaseAddress(ctx->CPU));

    DUI_PanelStart(NULL, stackLogWidth, stackLogHeight, false);

    char stackLogEntry[STACK_PREVIEW_ENTRY_SIZE + 1];

    uint16_t addr = ctx->CPU->SP;
    for (int i = 0; i < STACK_PREVIEW_LENGTH; ++i) {
        if (addr == SM83_GetStackBaseAddress(ctx->CPU)) {
            break;
        }

        // uint16_t ptr = ReadWord(addr);

        // SM83_Disassemble(ctx->CPU, stackLogEntry, sizeof(stackLogEntry), ptr);
        // DUI_Println("[%04X] $%04X: %s", addr, ptr, stackLogEntry);

        addr += 2;
    }

    DUI_PanelEnd();

    DUI_MoveCursorRelative(0, style->LinePadding);

    int tmpY;
    DUI_GetCursor(NULL, &tmpY);

    DUI_Print("Instruction Log");

    DUI_MoveCursor(bounds->x + bounds->w 
        - (style->CharWidth * 9) - (style->ButtonPadding * 2), tmpY - style->ButtonPadding);

    // bool instLogEnabled = SM83_GetInstructionLoggingEnabled(CPU);
    // DUI_Checkbox("Enabled", &instLogEnabled);

    DUI_MoveCursor(column4X, 
        tmpY + style->CharHeight + (style->LinePadding * 2));

    const int INSTRUCTION_LOG_VIEW_LENGTH = 28;

    int instLogWidth = column4Width;
    int instLogHeight = (duiLineHeight * INSTRUCTION_LOG_VIEW_LENGTH);

    DUI_PanelStart(NULL, instLogWidth, instLogHeight, false);

    for (int i = 0; i < INSTRUCTION_LOG_VIEW_LENGTH; ++i) {
        DUI_Println(SM83_GetInstructionLogEntry(ctx->CPU, i));
    }

    DUI_PanelEnd();
}
