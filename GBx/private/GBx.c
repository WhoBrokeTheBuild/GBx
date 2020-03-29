#include <GBx/GBx.h>

void Reset()
{
    ResetAPU();
    ResetCPU();
    ResetInterrupts();
    ResetJoypad();
    ResetLCD();
    ResetMBC();
    ResetMMU();
    ResetTimer();
}