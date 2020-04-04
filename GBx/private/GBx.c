#include <GBx/GBx.h>

uint8_t readByte(void * _, uint16_t address)
{
    return ReadByte(address);
}

void writeByte(void * _, uint16_t address, uint8_t data)
{
    WriteByte(address, data);
}

void tick(void * _, unsigned cycles)
{
    APUTick(cycles);
    TimerTick(cycles);
    LCDTick(cycles);
}

void Reset()
{
    SM83_Init(&CPU);
    CPU.Tick      = tick;
    CPU.ReadByte  = readByte;
    CPU.WriteByte = writeByte;

    ResetAPU();
    ResetJoypad();
    ResetLCD();
    ResetMBC();
    ResetMMU();
    ResetTimer();
}