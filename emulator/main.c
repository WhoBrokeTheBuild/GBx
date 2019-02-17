#include <stdint.h>
#include <string.h>

#include "log.h"
#include "usage.h"
#include "memory.h"
#include "register.h"

#include "arithmetic.h"
#include "loads.h"
#include "jumps.h"
#include "calls.h"

typedef void(*opfun_t)();

void opNOP() 
{
    LogVerbose("%s", __FUNCTION__);
}

void opHALT()
{
    LogVerbose("%s", __FUNCTION__);
}

opfun_t opfunMap[0xFFFF] = {
    [0x00] = &opNOP,
    [0x01] = &opLD_BC,
    [0x02] = &opLD_BC_A,
    [0x09] = &opADD_BC,
    [0x11] = &opLD_DE,
    [0x12] = &opLD_DE_A,
    [0x19] = &opADD_DE,
    [0x21] = &opLD_HL,
    [0x29] = &opADD_HL,
    [0x31] = &opLD_SP,
    [0x39] = &opADD_SP,
    [0x47] = &opLD_B_A,
    [0x4F] = &opLD_C_A,
    [0x57] = &opLD_D_A,
    [0x5F] = &opLD_E_A,
    [0x67] = &opLD_H_A,
    [0x6F] = &opLD_L_A,
    [0x76] = &opHALT,
    [0x77] = &opLD_HL_A,
    [0x7F] = &opLD_A_A,
    [0xC3] = &opJP,
    [0xC2] = &opJP_NZ,
    [0xC9] = &opRET,
    [0xCA] = &opJP_Z,
    [0xD2] = &opJP_NC,
    [0xDA] = &opJP_C,
    [0xEA] = &opLD_nn_A,
};

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    if (!loadROM(argv[1])) {
        return 1;
    }

    R.AF = 0x0001;
    R.F  = 0x00B0;
    R.PC = 0x0100;
    R.BC = 0x0013;
    R.DE = 0x00D8;
    R.HL = 0x014D;
    R.SP = 0xFFFE;

    const int LIMIT = 10;

    uint8_t op;
    opfun_t opfun = NULL;
    for (int i = 0; i < LIMIT; ++i) {
        op = readByte(R.PC++);

        opfun = opfunMap[op];
        if (opfun) {
            opfun();
        } else {
            LogVerbose("op: %02X", op);
        }

        printR();
    }

    freeROM();

    return 0;
}