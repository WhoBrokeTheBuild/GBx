#include "apu.h"
#include "bootstrap.h"
#include "cartridge.h"
#include "cpu.h"
#include "debug.h"
#include "gbx.h"
#include "lcd.h"
#include "log.h"

#include <SDL.h>
#include <cflags.h>

#ifdef HAVE_READLINE

#include <readline/readline.h>

#endif

int main(int argc, char ** argv)
{
    cflags_t * flags = cflags_init();

    cflags_add_bool(flags, 'd', "debug", &DebugEnable, "Enable Debug Mode");
    cflags_flag_t * verbose = cflags_add_bool(flags, 'v', "verbose", NULL, "Enables verbose output, repeat up to 4 times for more verbosity");

    const char * bootstrapFilename = NULL;
    cflags_add_string(flags, 'b', "bootstrap", &bootstrapFilename, "Load a Bootstrap ROM");

    bool breakAtStart = false;
    cflags_add_bool(flags, '\0', "break-start", &breakAtStart, "Breakpoint when application starts, implies Debug Mode");

    cflags_parse(flags, argc, argv);
    VerboseLevel = verbose->count;

    if (flags->argc == 0) {
        cflags_print_usage(flags,
            "[OPTION]... ROM_FILENAME",
            "A Toy GameBoy Emulator",
            "Additional information about this program can be found by contacting:\n"
            "  sdl.slane@gmail.com");
        return 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        LogFatal("Failed to initialize SDL2, %s", SDL_GetError());
    }

    SDL_version sdlVer;
    SDL_GetVersion(&sdlVer);
    LogVerbose(1, "SDL Version: %d.%d.%d",
        sdlVer.major, sdlVer.minor, sdlVer.patch);

    #if defined(HAVE_READLINE)
        LogVerbose(1, "Readline Version: %s", rl_library_version);
    #endif

    if (!loadCartridge(flags->argv[0])) {
        return 1;
    }

    lcdInit();
    apuInit();

    if (bootstrapFilename) {
        loadBootstrap(bootstrapFilename);
    }
    else {
        bootstrap();
    }

    if (breakAtStart) {
        DebugEnable = true;
    }
    
    if (DebugEnable) {
        debugInit();
        
        if (breakAtStart) {
            setBreakpoint(R.PC);
        }
    }

    run();

    if (DebugEnable) {
        debugTerm();
    }

    apuTerm();
    lcdTerm();

    freeCartridge();

    SDL_Quit();

    cflags_free(flags);

    return 0;
}
