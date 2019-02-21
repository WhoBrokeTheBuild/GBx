#include <SDL.h>
#include <stdint.h>
#include <string.h>

#include "bootstrap.h"
#include "cpu.h"
#include "log.h"
#include "memory.h"
#include "register.h"
#include "usage.h"

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

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    LogInfo("loading rom %s", argv[1]);

    if (!loadROM(argv[1])) {
        LogFatal("failed to load rom");
    }

    R.PC = 0x0000;

    SDL_Event evt;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        cpuTick();
        printR();

        lcdTick();

        //SDL_RenderClear(renderer);
        //SDL_RenderPresent(renderer);
    }

    freeROM();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}