#ifndef IO_H
#define IO_H

#include <stdint.h>

typedef union {
    struct {
        uint8_t P10:1;
        uint8_t P11:1;
        uint8_t P12:1;
        uint8_t P13:1;
        uint8_t P14:1;
        uint8_t P15:1;
        uint8_t _:2;
    };
    uint8_t data;
} P1_t;

extern P1_t P1;

extern uint8_t SB;

typedef union {
    struct {
        uint8_t ShiftClock:1;
        uint8_t _:6;
        uint8_t TransferStart:1;
    };
    uint8_t data;
} SC_t;

extern SC_t SC;

#endif // IO_H