#include "timer.h"

#include "clock.h"
#include "interrupt.h"
#include "log.h"

byte DIV;
byte TIMA;
byte TMA;

timer_control TAC;

int timerCounter = 0;
int divCounter = 0;

int getTimerSpeed()
{
    switch (TAC.Type) {
    case 0b00:
        return ClockSpeed / 1024;
    case 0b01:
        return ClockSpeed / 16;
    case 0b10:
        return ClockSpeed / 64;
    case 0b11:
        return ClockSpeed / 256;
    default:
        break;
    }

    return 0;
}

void ResetTimer()
{
    TIMA = 0x00;
    TMA = 0x00;
    TAC.raw = 0x00;
}

void TimerTick(unsigned cycles)
{
    divCounter += cycles;
    if (divCounter >= 255) {
        divCounter -= 255;
        ++DIV;
    }

    if (!TAC.Enable) {
        return;
    }

    int speed = getTimerSpeed();

    timerCounter += cycles;
    if (timerCounter > speed) {
        timerCounter -= speed;
        
        if (TIMA == 0xFF) {
            LogVerbose(2, "Timer Rollover");
            TIMA = TMA;
            IF.Timer = true;
        }
        else {
            ++TIMA;
        }
    }
}

void PrintTimer()
{
    LogInfo("Enable=%s Type=%d (%d Hz) TIMA=%d TMA=%d DIV=%d",
        (TAC.Enable ? "True" : "False"), TAC.Type, getTimerSpeed(), TIMA, TMA, DIV);
}
