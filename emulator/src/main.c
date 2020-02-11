#include <SDL.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

#include "bootstrap.h"
#include "cpu.h"
#include "cartridge.h"
#include "debug.h"
#include "log.h"
#include "memory.h"
#include "video.h"
#include "interrupt.h"

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
    LogInfo("Caught Signal %d", sig);

    DebugMode = true;
    debugPrompt();
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    LogInfo("loading rom %s", argv[1]);

    if (!loadCartridge(argv[1])) {
        LogFatal("failed to load rom");
    }

    lcdInit();

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
        setBreakpoint(BKCND_PC_EQ, 0x0150);
    }
    else {
        setBreakpoint(BKCND_PC_EQ, 0x0000);
    }

    if (DebugMode) {
        signal(SIGINT, handleSignal);
        signal(SIGSEGV, handleSignal);
    }

    for (;;) {
        if (atBreakpoint()) {
            debugPrompt();
        } else {
            nextInstruction();
        }
    }

    freeCartridge();

    lcdTerm();

    return 0;
}