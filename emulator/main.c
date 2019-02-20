#include <stdint.h>
#include <string.h>

#include "log.h"
#include "usage.h"
#include "cpu.h"
#include "memory.h"
#include "register.h"

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    if (!loadROM(argv[1])) {
        return 1;
    }

    R.A  = 0x01;
    R.F  = 0b10110000;
    R.PC = 0x0100;
    R.BC = 0x0013;
    R.DE = 0x00D8;
    R.HL = 0x014D;
    R.SP = 0xFFFE;
    printR();

    const int LIMIT = 15;

    for (int i = 0; i < LIMIT; ++i) {
        CPUTick();
        printR();
        //getchar();
    }

    freeROM();

    return 0;
}