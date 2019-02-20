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

    R.PC = 0x0000;

    for (int i = 0; i < sizeof(BOOTSTRAP); ++i) {
        cpuTick();
        printR();
    }

    if (!loadROM(argv[1])) {
        return 1;
    }

    const int LIMIT = 1000;

    for (int i = 0; i < LIMIT; ++i) {
        cpuTick();
        printR();
        //getchar();
    }

    freeROM();

    return 0;
}