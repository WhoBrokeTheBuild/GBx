#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

#include "log.h"

#define CLOCK_TYPE_1 (0b00) // 4096 Hz
#define CLOCK_TYPE_2 (0b01) // 262144 Hz
#define CLOCK_TYPE_3 (0b10) // 65536 Hz
#define CLOCK_TYPE_4 (0b11) // 16384 Hz

typedef union {
    struct {
        unsigned Type:2;
        bool     Enable:1;
        unsigned :5;
    };
    uint8_t raw;
} TAC_t;

extern uint8_t TIMA; // Timer Counter
extern uint8_t TMA;  // Timer Modulo
extern TAC_t TAC;    // Timer Control

extern uint8_t DIV;

int getTimerSpeed();

void timerTick(unsigned cycles);

static void printTimer()
{
    LogInfo("Enable=%s Type=%d (%d Hz) TIMA=%d TMA=%d DIV=%d",
        (TAC.Enable ? "True" : "False"), TAC.Type, getTimerSpeed(), TIMA, TMA, DIV);
}

#endif // TIMER_H