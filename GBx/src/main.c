#include "video.h"
#include "audio.h"
#include "debug.h"
#include "breakpoint.h"

#include <GBx/bootstrap.h>
#include <GBx/cartridge.h>
#include <GBx/cpu.h>
#include <GBx/instruction.h>
#include <GBx/log.h>
#include <GBx/memory.h>

#include <GBx/lcd.h>

#include <SDL.h>
#include <cflags.h>

// TODO Win32 support
#include <pthread.h>

#ifdef HAVE_READLINE
    #include <readline/readline.h>
#endif

void * runThread(void * ptr)
{
    if (DebugEnabled) {
        DebugInit();
    }

    for (;;) {
        if (AtBreakpoint()) {
            DebugPrompt();
        }
        
        NextInstruction();
    }

    if (DebugEnabled) {
        DebugTerm();
    }
}

int main(int argc, char ** argv)
{
    cflags_t * flags = cflags_init();

    cflags_add_bool(flags, 'd', "debug", &DebugEnabled, "Enable Debug Mode");

    int scale = 1;
    cflags_add_int(flags, 's', "scale", &scale, "Window scale, default is 1");

    cflags_flag_t * verbose = cflags_add_bool(flags, 'v', "verbose", NULL, "Enables verbose output, repeat up to 4 times for more verbosity");

    const char * bootROMFilename = NULL;
    cflags_add_string(flags, 'b', "bootstrap", &bootROMFilename, "Load a Bootstrap ROM");

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

    if (!LoadCartridgeROM(flags->argv[0])) {
        return 1;
    }

    if (bootROMFilename) {
        LoadBootstrapROM(bootROMFilename);
    }

    Reset();

    VideoInit(scale);
    AudioInit();

    if (DebugEnabled) {
        SetBreakpoint("PC", R.PC);
    }
    
    pthread_t thread;
    pthread_create(&thread, NULL, runThread, NULL);
    
    for (;;) {
        PollEvents();
        
        Render();
    }
    
    pthread_join(thread, NULL);

    VideoTerm();
    AudioTerm();

    SDL_Quit();

    cflags_free(flags);

    return 0;
}
