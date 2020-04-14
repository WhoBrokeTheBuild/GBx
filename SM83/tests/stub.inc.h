#include <SM83/SM83.h>

sm83_t * CPU = NULL;

uint8_t Memory[0x10000];

uint8_t readByte(uint16_t address)
{
    return Memory[address];
}

void writeByte(uint16_t address, uint8_t data)
{
    Memory[address] = data;
}

uint16_t readWord(uint16_t address)
{
    return (readByte(address + 1) << 8) | readByte(address);
}

void writeWord(uint16_t address, uint16_t data)
{
    writeByte(address + 1, (uint8_t)(data >> 8));
    writeByte(address, (uint8_t)(data & 0xFF));
}

void pushWord(uint16_t data)
{
    CPU->SP -= 2;
    writeWord(CPU->SP, data);
}

uint16_t popWord()
{
    CPU->SP += 2;
    return readWord(CPU->SP - 2);
}

void stub_tick(void * userData, unsigned cycles)
{
    (void)userData;
    (void)cycles;
}

uint8_t stub_readByte(void * userData, uint16_t address)
{
    (void)userData;
    return readByte(address);
}

void stub_writeByte(void * userData, uint16_t address, uint8_t data)
{
    (void)userData;
    writeByte(address, data);
}

void stub_init()
{
    CPU = SM83_Init();
    CPU->UserData = NULL;
    CPU->Tick = stub_tick;
    CPU->ReadByte = stub_readByte;
    CPU->WriteByte = stub_writeByte;
}
