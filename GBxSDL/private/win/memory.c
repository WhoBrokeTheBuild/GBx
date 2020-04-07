#include "status.h"
#include "../debug.h"

#include <GBx/GBx.h>
#include <DUI/DUI.h>
#include <SDL.h>

SDL_Texture * sdlMemoryTexture = NULL;

const int MEMORY_TRACKER_AGE_TIMEOUT = 4;
int memoryTrackerAgeDelay = 0;

#define MEMORY_TEXTURE_WIDTH  (256)
#define MEMORY_TEXTURE_HEIGHT (256)
#define MEMORY_TEXTURE_COMP   (3)

void InitMemoryTab()
{
    sdlMemoryTexture = SDL_CreateTexture(GetDebugWindowRenderer(),
        SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        MEMORY_TEXTURE_WIDTH, MEMORY_TEXTURE_HEIGHT);
}

void TermMemoryTab()
{
    SDL_DestroyTexture(sdlMemoryTexture);
}

void MemoryTabRefresh()
{
    uint8_t * pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(sdlMemoryTexture, NULL, (void **)&pixels, &pitch);

    if (MemoryTrackingEnabled) {
        for (unsigned i = 0; i < sizeof(MemoryTracker); ++i) {
            int offset = (i * MEMORY_TEXTURE_COMP);
            pixels[offset + 0] = (MemoryTracker[i].Write << 4);
            pixels[offset + 1] = (MemoryTracker[i].Read << 4);
            pixels[offset + 2] = 0x00;
        }
    }
    else {
        memset(pixels, 0, 
            MEMORY_TEXTURE_WIDTH * MEMORY_TEXTURE_HEIGHT * MEMORY_TEXTURE_COMP);
    }

    SDL_UnlockTexture(sdlMemoryTexture);

    ++memoryTrackerAgeDelay;
    if (memoryTrackerAgeDelay >= MEMORY_TRACKER_AGE_TIMEOUT) {
        memoryTrackerAgeDelay = 0;
        AgeMemoryTracker();
    }
}

void MemoryTabRender()
{
    DUI_Style * style = DUI_GetStyle();

    int startX, startY;
    DUI_GetCursor(&startX, &startY);

    DUI_Println("MEMORY TRACKING");

    DUI_Checkbox("ENABLED", &MemoryTrackingEnabled);
    DUI_Newline();

    SDL_Rect dst = {
        .x = startX + (style->CharWidth * 14),
        .y = startY + (style->CharHeight * 10),
        .w = (MEMORY_TEXTURE_WIDTH * 2),
        .h = (MEMORY_TEXTURE_HEIGHT * 2),
    };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlMemoryTexture, NULL, &dst);

    SDL_Rect read = {
        .x = startX + (style->CharWidth * 4),
        .y = startY + (style->CharHeight * 6),
        .w = (style->CharWidth * (strlen("READ") + 1)),
        .h = (style->CharHeight * 2),
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &read);

    DUI_PrintAt(
        read.x + (style->CharWidth / 2),
        read.y + (style->CharHeight / 2),
        "READ");

    int readOffsetX = startX + (style->CharWidth * 13);
    int readOffsetY = startY + (style->CharHeight * 10);

    DUI_PrintAt(
        readOffsetX - (style->CharWidth * strlen("ROM[0]")),
        readOffsetY,
        "ROM[0]");

    readOffsetY += (8 * 16);

    DUI_PrintAt(
        readOffsetX - (style->CharWidth * strlen("ROM[N]")),
        readOffsetY,
        "ROM[N]");

    readOffsetY += (8 * 16);

    DUI_PrintAt(
        readOffsetX - (style->CharWidth * strlen("VRAM[N]")),
        readOffsetY,
        "VRAM[N]");

    readOffsetY += (8 * 8);

    DUI_PrintAt(
        readOffsetX - (style->CharWidth * strlen("SRAM[N]")),
        readOffsetY,
        "SRAM[N]");

    readOffsetY += (8 * 8);

    DUI_PrintAt(
        readOffsetX - (style->CharWidth * strlen("WRAM[0]")),
        readOffsetY,
        "WRAM[0]");

    readOffsetY += (8 * 4);

    DUI_PrintAt(
        readOffsetX - (style->CharWidth * strlen("WRAM[N]")),
        readOffsetY,
        "WRAM[N]");

    readOffsetY += (8 * 4);

    DUI_PrintAt(
        readOffsetX - (style->CharWidth * strlen("ECHO")),
        readOffsetY,
        "ECHO");

    readOffsetY += (8 * 4);

    DUI_PrintAt(
        readOffsetX - (style->CharWidth * strlen("OAM/HW/HRAM")),
        readOffsetY,
        "OAM/HW/HRAM");

    SDL_Rect write = {
        .x = startX + (style->CharWidth * 18) + (MEMORY_TEXTURE_WIDTH * 2),
        .y = startY + (style->CharHeight * 6),
        .w = (style->CharWidth * (strlen("WRITE") + 1)),
        .h = (style->CharHeight * 2),
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &write);

    DUI_PrintAt(
        write.x + (style->CharWidth / 2),
        write.y + (style->CharHeight / 2),
        "WRITE");

    int writeOffsetX = startX + (style->CharWidth * 15) + (MEMORY_TEXTURE_WIDTH * 2);
    int writeOffsetY = startY + (style->CharHeight * 10);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "MBC");

    writeOffsetY += (8 * 32);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "VRAM[N]");

    writeOffsetY += (8 * 8);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "SRAM[N]");

    writeOffsetY += (8 * 8);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "WRAM[0]");

    writeOffsetY += (8 * 4);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "WRAM[N]");

    writeOffsetY += (8 * 4);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "ECHO");

    writeOffsetY += (8 * 4);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "OAM/HW/HRAM");

    struct hwaddr
    {
        uint16_t address;
        const char * text;

    };
    
    struct hwaddr hwaddrs[] = {
        { 0xFF00, "FF00 - JOYP" },
        { 0xFF01, "FF01 - SB" },
        { 0xFF02, "FF02 - SC" },
        { 0xFF04, "FF04 - DIV" },
        { 0xFF05, "FF05 - TIMA" },
        { 0xFF06, "FF06 - TMA" },
        { 0xFF07, "FF07 - TAC" },
        { 0xFF0F, "FF0F - IF" },
        { 0xFF10, "FF10 - Tone1" },
        { 0xFF11, "FF11 - " },
        { 0xFF12, "FF12 - " },
        { 0xFF13, "FF13 - " },
        { 0xFF14, "FF14 - " },
        { 0xFF16, "FF16 - Tone2" },
        { 0xFF17, "FF17 - " },
        { 0xFF18, "FF18 - " },
        { 0xFF19, "FF19 - " },
        { 0xFF20, "FF20 - Noise" },
        { 0xFF21, "FF21 - " },
        { 0xFF22, "FF22 - " },
        { 0xFF23, "FF23 - " },
        { 0xFF24, "FF24 - VolumeControl" },
        { 0xFF25, "FF25 - SoundTerminal" },
        { 0xFF26, "FF26 - APUC" },
        { 0xFF1A, "FF1A - Wave" },
        { 0xFF1B, "FF1B - " },
        { 0xFF1C, "FF1C - " },
        { 0xFF1D, "FF1D - " },
        { 0xFF1E, "FF1E - " },
        { 0xFF40, "FF40 - LCDC" },
        { 0xFF41, "FF41 - STAT" },
        { 0xFF42, "FF42 - SCY" },
        { 0xFF43, "FF43 - SCX" },
        { 0xFF44, "FF44 - LY" },
        { 0xFF45, "FF45 - LYC" },
        { 0xFF46, "FF46 - DMA" },
        { 0xFF47, "FF47 - BGP" },
        { 0xFF48, "FF48 - OBP0" },
        { 0xFF49, "FF49 - OBP1" },
        { 0xFF4A, "FF4A - WX" },
        { 0xFF4B, "FF4B - WY" },
        { 0xFFFF, "FFFF - IE" },
    };

    DUI_MoveCursor(startX + (style->CharWidth * 84), startY);

    SDL_Rect box = {
        .w = style->CharHeight,
        .h = style->CharHeight,
    };

    DUI_GetCursor(&box.x, &box.y);

    box.x -= style->CharWidth + box.w;

    for (int i = 0; i < sizeof(hwaddrs) / sizeof(struct hwaddr); ++i) {
        SDL_SetRenderDrawColor(GetDebugWindowRenderer(),
            (MemoryTracker[i].Write << 4),
            (MemoryTracker[i].Read << 4),
            0x00,
            0xFF
        );

        SDL_RenderFillRect(GetDebugWindowRenderer(), &box);
        box.y += style->CharHeight + style->LinePadding;

        DUI_Println(hwaddrs[i].text);
    }
}
