#ifndef REGISTER_H
#define REGISTER_H

#include "log.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    union {
        struct {
            union {
                struct {
                    uint8_t _:4;
                    bool FC:1; // Carry Flag
                    bool FH:1; // Half Carry Flag
                    bool FN:1; // Subtract Flag
                    bool FZ:1; // Zero Flag
                };
                uint8_t F;
            };
            uint8_t A;
        };
        uint16_t AF;
    };
    union {
        struct {
            uint8_t C;
            uint8_t B;
        };
        uint16_t BC;
    };
    union {
        struct {
            uint8_t E;
            uint8_t D;
        };
        uint16_t DE;
    };
    union {
        struct {
            uint8_t L;
            uint8_t H;
        };
        uint16_t HL;
    };
    uint16_t SP;
    uint16_t PC;
} reg_t;

extern reg_t R;

static void printR()
{
    LogInfo("AF=%04X BC=%04X DE=%04X HL=%04X SP=%04X PC=%04X FZ=%d FN=%d FH=%d FC=%d",
        R.AF, R.BC, R.DE, R.HL, R.SP, R.PC, R.FZ, R.FN, R.FH, R.FC);
}

#endif // REGISTER_H