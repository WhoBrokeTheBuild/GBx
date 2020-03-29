#include "status.h"
#include "../debug.h"

#include <GBx/GBx.h>
#include <DUI/DUI.h>
#include <SDL.h>

SDL_Texture * sdlMemoryTexture = NULL;

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
            pixels[offset + 1] = 0x00;
            pixels[offset + 2] = (MemoryTracker[i].Read << 4);
        }
    }
    else {
        memset(pixels, 0, 
            MEMORY_TEXTURE_WIDTH * MEMORY_TEXTURE_HEIGHT * MEMORY_TEXTURE_COMP);
    }

    SDL_UnlockTexture(sdlMemoryTexture);

    AgeMemoryTracker();
}

void MemoryTabRender()
{
    DUI_Style * style = DUI_GetStyle();
    
    DUI_MoveCursor(DEBUG_CONTENT_X, DEBUG_CONTENT_Y);
    DUI_Panel(DEBUG_CONTENT_WIDTH, DEBUG_CONTENT_HEIGHT);

    int startX, startY;
    DUI_GetCursor(&startX, &startY);

    DUI_Println("MEMORY TRACKING");

    DUI_CheckboxAt(
        startX + (style->CharSize * 82),
        startY,
        "ENABLED", &MemoryTrackingEnabled);

    SDL_Rect dst = {
        .x = startX + (style->CharSize * 14),
        .y = startY + (style->CharSize * 10),
        .w = (MEMORY_TEXTURE_WIDTH * 2),
        .h = (MEMORY_TEXTURE_HEIGHT * 2),
    };

    SDL_RenderCopy(GetDebugWindowRenderer(), sdlMemoryTexture, NULL, &dst);

    SDL_Rect read = {
        .x = startX + (style->CharSize * 3),
        .y = startY + (style->CharSize * 4),
        .w = (style->CharSize * (strlen("READ") + 1)),
        .h = (style->CharSize * 2),
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &read);

    DUI_PrintAt(
        read.x + (style->CharSize / 2),
        read.y + (style->CharSize / 2),
        "READ");

    int readOffsetX = startX + (style->CharSize * 13);
    int readOffsetY = startY + (style->CharSize * 10);

    DUI_PrintAt(
        readOffsetX - (style->CharSize * strlen("ROM[0]")),
        readOffsetY,
        "ROM[0]");

    readOffsetY += (style->CharSize * 16);

    DUI_PrintAt(
        readOffsetX - (style->CharSize * strlen("ROM[N]")),
        readOffsetY,
        "ROM[N]");

    readOffsetY += (style->CharSize * 16);

    DUI_PrintAt(
        readOffsetX - (style->CharSize * strlen("VRAM[N]")),
        readOffsetY,
        "VRAM[N]");

    readOffsetY += (style->CharSize * 8);

    DUI_PrintAt(
        readOffsetX - (style->CharSize * strlen("SRAM[N]")),
        readOffsetY,
        "SRAM[N]");

    readOffsetY += (style->CharSize * 8);

    DUI_PrintAt(
        readOffsetX - (style->CharSize * strlen("WRAM[0]")),
        readOffsetY,
        "WRAM[0]");

    readOffsetY += (style->CharSize * 4);

    DUI_PrintAt(
        readOffsetX - (style->CharSize * strlen("WRAM[N]")),
        readOffsetY,
        "WRAM[N]");

    readOffsetY += (style->CharSize * 4);

    DUI_PrintAt(
        readOffsetX - (style->CharSize * strlen("ECHO")),
        readOffsetY,
        "ECHO");

    readOffsetY += (style->CharSize * 4);

    DUI_PrintAt(
        readOffsetX - (style->CharSize * strlen("OAM/HW/HRAM")),
        readOffsetY,
        "OAM/HW/HRAM");

    SDL_Rect write = {
        .x = startX + (style->CharSize * 82),
        .y = startY + (style->CharSize * 4),
        .w = (style->CharSize * (strlen("WRITE") + 1)),
        .h = (style->CharSize * 2),
    };

    SDL_SetRenderDrawColor(GetDebugWindowRenderer(), 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(GetDebugWindowRenderer(), &write);

    DUI_PrintAt(
        write.x + (style->CharSize / 2),
        write.y + (style->CharSize / 2),
        "WRITE");

    int writeOffsetX = startX + (style->CharSize * 79);
    int writeOffsetY = startY + (style->CharSize * 10);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "MBC");

    writeOffsetY += (style->CharSize * 32);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "VRAM[N]");

    writeOffsetY += (style->CharSize * 8);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "SRAM[N]");

    writeOffsetY += (style->CharSize * 8);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "WRAM[0]");

    writeOffsetY += (style->CharSize * 4);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "WRAM[N]");

    writeOffsetY += (style->CharSize * 4);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "ECHO");

    writeOffsetY += (style->CharSize * 4);

    DUI_PrintAt(
        writeOffsetX,
        writeOffsetY,
        "OAM/HW/HRAM");
}
