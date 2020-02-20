#include "timer.h"

#include "clock.h"
#include "interrupt.h"

uint8_t TIMA;
uint8_t TMA;
TAC_t TAC;

uint8_t DIV;

int timerCounter = 0;
int divCounter = 0;

int getTimerSpeed()
{
    switch (TAC.Type) {
    case 0b00:
        return 4096;
    case 0b01:
        return 262144;
    case 0b10:
        return 65536;
    case 0b11:
        return 16384;
    default:
        return 1;
    }
}

void timerTick(unsigned cycles)
{
    divCounter += cycles;
    if (divCounter >= 255) {
        divCounter -= 255;
        ++DIV;
    }

    if (!TAC.Enable) {
        return;
    }

    unsigned speed = ClockSpeed / TAC.Type;

    timerCounter += cycles;
    if (timerCounter > speed) {
        timerCounter -= speed;
        
        if (TIMA == 0xFF) {
            TIMA = TMA;
            IF.Timer = true;
        }
        else {
            ++TIMA;
        }
    }
}