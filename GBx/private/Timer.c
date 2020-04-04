#include <GBx/Timer.h>

#include <GBx/CPU.h>
#include <GBx/Log.h>
#include <GBx/Util.h>

uint8_t DIV;
uint8_t TIMA;
uint8_t TMA;

timer_control TAC;

int timerCounter = 0;
int divCounter = 0;

int GetTimerSpeed()
{
    switch (TAC.Type) {
    case 0b00:
        return CPU.ClockSpeed / 1024;
    case 0b01:
        return CPU.ClockSpeed / 16;
    case 0b10:
        return CPU.ClockSpeed / 64;
    case 0b11:
        return CPU.ClockSpeed / 256;
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
    if (divCounter >= 0xFF) {
        divCounter -= 0xFF;
        ++DIV;
    }

    if (!TAC.Enabled) {
        return;
    }

    int speed = GetTimerSpeed();

    timerCounter += cycles;
    if (timerCounter > speed) {
        timerCounter -= speed;
        
        if (TIMA == 0xFF) {
            LogVerbose(2, "Timer Rollover");
            TIMA = TMA;
            CPU.IF.Int50 = true;
        }
        else {
            ++TIMA;
        }
    }
}

void PrintTimer()
{
    LogInfo("Timer %s Type=%d (%d Hz) TIMA=%d TMA=%d DIV=%d",
        GetEnabledString(TAC.Enabled),
        TAC.Type,
        GetTimerSpeed(),
        TIMA,
        TMA,
        DIV);
}
