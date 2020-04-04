#ifndef TIMER_H
#define TIMER_H

#include <GBx/Types.h>

#define CLOCK_TYPE_1 (0b00) // 4096 Hz
#define CLOCK_TYPE_2 (0b01) // 262144 Hz
#define CLOCK_TYPE_3 (0b10) // 65536 Hz
#define CLOCK_TYPE_4 (0b11) // 16384 Hz

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t Type:2;
        bool Enabled:1;
        uint8_t :5;
    });

    // clang-format on

    uint8_t raw;

} timer_control;

// FF04 - Divider

extern uint8_t DIV;

// FF05 Timer Counter

extern uint8_t TIMA;

// FF06 - Timer Modulo

extern uint8_t TMA;

// FF07 - Timer Control

extern timer_control TAC;

int GetTimerSpeed();

void ResetTimer();

void TimerTick(unsigned cycles);

void PrintTimer();

#endif // TIMER_H