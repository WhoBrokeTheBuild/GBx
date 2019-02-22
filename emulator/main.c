#include <SDL.h>
#include <stdint.h>
#include <string.h>

#include "bootstrap.h"
#include "cpu.h"
#include "debug.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"
#include "register.h"
#include "usage.h"
#include "video.h"

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    lcdInit();

    LogInfo("loading rom %s", argv[1]);

    if (!loadROM(argv[1])) {
        LogFatal("failed to load rom");
    }

    if (argc > 2 && strcmp(argv[2], "-d") == 0) {
        DebugMode = true;
        B.PC = 0x0000;
    }

    R.PC = 0x0000;

    for (;;) {
        if (R.PC == B.PC) {
            debugPrompt();
        } else {
            nextInstruction();
        }
    }


    freeROM();

    lcdTerm();

    SDL_Quit();

    return 0;
}