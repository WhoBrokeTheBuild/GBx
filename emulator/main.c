#if defined(WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <SDL.h>
#include <stdint.h>
#include <string.h>

#include "bootstrap.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"
#include "register.h"
#include "usage.h"

bool running = true;

void * cpu(void * args) 
{
    const char * filename = (const char *)args;
    LogInfo("bootstrap");

    R.PC = 0x0000;

    for (int i = 0; i < sizeof(BOOTSTRAP); ++i) {
        cpuTick();
        printR();
    }

    LogInfo("loading rom %s", filename);

    if (!loadROM(filename)) {
        LogFatal("failed to load rom");
    }

    const int LIMIT = 100;

    R.PC = 0x0100;

    for (int i = 0; i < LIMIT; ++i) {
        cpuTick();
        printR();
        //getchar();

        if (!running) {
            LogInfo("breaking");
            break;
        }
    }

    freeROM();
    return NULL;
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LogFatal("failed to initialize SDL2, %s", SDL_GetError());
    }

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    if (SDL_CreateWindowAndRenderer(160, 144, 0, &window, &renderer)) {
        LogFatal("failed to create SDL2 window/renderer, %s", SDL_GetError());
    }

    pthread_t cpuThread;
    if (pthread_create(&cpuThread, NULL, cpu, argv[1])) {
        LogFatal("failed to create CPU thread");
    }

    SDL_Event evt;
    while (running) {
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if (pthread_join(cpuThread, NULL)) {
        LogError("failed to join CPU thread");
    }

    SDL_Quit();

    return 0;
}