#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

enum {
    CLOCK_TYPE_1    = 0b00, // 4.096 Khz
    CLOCK_TYPE_2    = 0b01, // 262.144 Khz
    CLOCK_TYPE_3    = 0b10, // 65.536 Khz
    CLOCK_TYPE_4    = 0b11, // 16.384 Khz
};

typedef union 
{
    struct {
        uint8_t Type:2;
        bool Enable:1;
        uint8_t _:5;
    };
    uint8_t data;
} TAC_t;

extern uint8_t TIMA; // Timer Counter
extern uint8_t TMA;  // Timer Modulo
extern TAC_t TAC;    // Timer Control

#endif // TIMER_H