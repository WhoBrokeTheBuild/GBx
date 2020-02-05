#include <SDL.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <signal.h>

#include "bootstrap.h"
#include "cpu.h"
#include "debug.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"
#include "register.h"
#include "sound.h"
#include "timer.h"
#include "usage.h"
#include "video.h"

void handleSignal(int sig)
{
    LogInfo("signal");
    DebugMode = true;
}

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

    if (DebugMode) {
        signal(SIGINT, handleSignal);
    }

    for (;;) {
        if (atBreakpoint() || DebugMode) {
            debugPrompt();
        } else {
            nextInstruction();
        }
    }

    freeROM();

    lcdTerm();

    return 0;
}