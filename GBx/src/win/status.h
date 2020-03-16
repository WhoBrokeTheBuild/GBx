#ifndef WIN_STATUS_H
#define WIN_STATUS_H

#include "../debug.h"

static void StatusTabRender()
{
    int x = DEBUG_WINDOW_CONTENT_X;
    int y = DEBUG_WINDOW_CONTENT_Y;

    char buffer[1024];

    y += (DEBUG_LINE_HEIGHT * 2);

    snprintf(buffer, sizeof(buffer), "A: %02X F: %02X", R.A, R.F);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "B: %02X C: %02X", R.B, R.C);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "D: %02X E: %02X", R.D, R.E);
    RenderDebugString(x, y, buffer);
    y += DEBUG_LINE_HEIGHT;

    snprintf(buffer, sizeof(buffer), "H: %02X L: %02X", R.H, R.L);
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


}

#endif // WIN_STATUS_H