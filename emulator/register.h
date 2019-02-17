#ifndef REGISTER_H
#define REGISTER_H

#include "log.h"
#include <stdint.h>

typedef struct
{
    union {
        struct {
            uint8_t A;
            uint8_t F;
        };
        uint16_t AF;
    };
    union {
        struct {
            uint8_t B;
            uint8_t C;
        };
        uint16_t BC;
    };
    union {
        struct {
            uint8_t D;
            uint8_t E;
        };
        uint16_t DE;
    };
    union {
        struct {
            uint8_t H;
            uint8_t L;
        };
        uint16_t HL;
    };
    uint16_t SP;
    uint16_t PC;
    union {
        struct {
            uint8_t Flag_:4;
            uint8_t FlagC:1;
            uint8_t FlagN:1;
            uint8_t FlagH:1;
            uint8_t FlagZ:1;
        };
        uint8_t Flag;
    };
} GB_register_t;

extern GB_register_t R;

static void printR()
{
    LogVerbose("AF=%08X DE=%08X HL=%08X SP=%08X PC=%08X F=%08X",
        R.AF, R.DE, R.HL, R.SP, R.PC, R.Flag);
}

#endif // REGISTER_H