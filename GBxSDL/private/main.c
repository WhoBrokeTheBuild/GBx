#include "video.h"
#include "audio.h"
#include "debug.h"
#include "breakpoint.h"

#include <GBx/GBx.h>

#include <SDL.h>
#include <cflags.h>

#if defined(WIN32)

    #include <Windows.h>

#else 

    #include <pthread.h>

#endif // defined(WIN32)

#ifdef HAVE_READLINE
    #include <readline/readline.h>
#endif

void * runThread(void * arg)
{
    gbx_t * ctx = (gbx_t *)arg;

    if (DebugEnabled) {
        DebugPromptInit(ctx);
    }

    for (;;) {
        if (AtBreakpoint(ctx)) {
            DebugPrompt(ctx);
        }
        
        SM83_Step(ctx->CPU);
    }

    if (DebugEnabled) {
        DebugPromptTerm(ctx);
    }
}

int main(int argc, char ** argv)
{
    gbx_t * GBx = GBx_Init();

    cflags_t * flags = cflags_init();

    cflags_add_bool(flags, 'd', "debug", &DebugEnabled, "Enable Debug Mode");

    int scale = 1;
    cflags_add_int(flags, 's', "scale", &scale, "Window scale, default is 1");

    cflags_flag_t * verbose = cflags_add_bool(flags, 'v', "verbose", NULL, "Enables verbose output, repeat up to 4 times for more verbosity");

    const char * bootROMFilename = NULL;
    cflags_add_string(flags, 'b', "bootstrap", &bootROMFilename, "Load a Bootstrap ROM");

    cflags_add_bool(flags, '\0', "orig-colors", &GBx->UseOriginalColors, "Use original green-tinted LCD colors");

    cflags_parse(flags, argc, argv);
    GBx->VerboseLevel = verbose->count;
    GBx->CPU->VerboseLevel = verbose->count;

    if (flags->argc == 0) {
        cflags_print_usage(flags,
            "[OPTION]... ROM_FILENAME",
            "A Toy GameBoy Emulator",
            "Additional information about this program can be found by contacting:\n"
            "  sdl.slane@gmail.com");
        return 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        fprintf(stderr, "Failed to initialize SDL2, %s", SDL_GetError());
    }

    SDL_version sdlVer;
    SDL_GetVersion(&sdlVer);

    if (GBx->VerboseLevel >= 1) {
        printf("SDL Version: %d.%d.%d\n",
            sdlVer.major, sdlVer.minor, sdlVer.patch);
    }

    #if defined(HAVE_READLINE)
    if (GBx->VerboseLevel >= 1) {
        printf("Readline Version: %s\n", rl_library_version);
    }
    #endif


    if (!GBx_LoadCartridge(GBx, flags->argv[0])) {
        return 1;
    }

    SM83_PrintRegisters(GBx->CPU);

    if (bootROMFilename) {
        GBx_LoadBootstrap(GBx, bootROMFilename);
    }

    GBx_Reset(GBx);

    VideoInit(GBx, scale);
    AudioInit(GBx);

    if (DebugEnabled) {
        SetBreakpoint("PC", GBx->CPU->PC);

        DebugWindowInit(GBx);
    }

#if defined(WIN32)

    DWORD threadID;
    HANDLE thread;
    thread = CreateThread(NULL, 0, runThread, NULL, 0, &threadID);

#else

    pthread_t thread;
    pthread_create(&thread, NULL, runThread, GBx);

#endif // defined(WIN32)
    
    for (;;) {
        PollEvents(GBx);
        
        Render(GBx);
        DebugWindowRender(GBx);
    }
   
#if defined(WIN32)

    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);

#else

    pthread_join(thread, NULL);

#endif // defined(WIN32)

    if (DebugEnabled) {
        DebugWindowTerm(GBx);
    }

    VideoTerm(GBx);
    AudioTerm(GBx);

    GBx_Term(GBx);

    SDL_Quit();

    cflags_free(flags);

    return 0;
}
