#include "apu.h"
#include "bootstrap.h"
#include "cartridge.h"
#include "cpu.h"
#include "debug.h"
#include "lcd.h"
#include "log.h"
#include "register.h"

#include <SDL.h>
#include <signal.h>

static void usage() 
{
    printf(
        "usage: GBx FILENAME [-d]\n"
        "\n"
        "-b     Run with the original bootstrap code\n"
        "-d     Run with the built-in debugger\n"
    );
}

void handleSignal(int sig)
{
    LogInfo("Caught signal %d", sig);

    DebugMode = true;
    debugPrompt();
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    LogInfo("Loading ROM %s", argv[1]);

    if (!loadCartridge(argv[1])) {
        LogFatal("Failed to load ROM");
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        LogFatal("failed to initialize SDL2, %s", SDL_GetError());
    }

    lcdInit();
    apuInit();

    for (int i = 2; i < argc; ++i) {
        const char * arg = argv[i];
        if (strncmp(arg, "-d", 2) == 0
            || strncmp(arg, "--debug", 7) == 0) {
            DebugMode = true;
        }
        else if (strncmp(arg, "-b", 2) == 0
            || strncmp(arg, "bootstrap", 11) == 0) {
            BootstrapEnable = true;
        }
    }

    if (!BootstrapEnable) {
        bootstrap();
    }
    
    if (DebugMode) {
        signal(SIGINT, handleSignal);
        signal(SIGSEGV, handleSignal);
        
        setBreakpoint(BKCND_PC_EQ, R.PC);
    }

    for (;;) {
        if (atBreakpoint()) {
            debugPrompt();
        } else {
            nextInstruction();
        }
    }

    apuTerm();
    lcdTerm();

    freeCartridge();

    SDL_Quit();

    return 0;
}