#include <SM83/SM83.h>

uint8_t SM83_ReadByte(sm83_t * cpu, uint16_t address)
{
    SM83_Tick(cpu, 1);
    return cpu->ReadByte(cpu->UserData, address);
}

void SM83_WriteByte(sm83_t * cpu, uint16_t address, uint8_t data)
{
    SM83_Tick(cpu, 1);
    cpu->WriteByte(cpu->UserData, address, data);
}

uint16_t SM83_ReadWord(sm83_t * cpu, uint16_t address)
{
    return (SM83_ReadByte(cpu, address + 1) << 8) | SM83_ReadByte(cpu, address);
}

void SM83_WriteWord(sm83_t * cpu, uint16_t address, uint16_t data)
{
    SM83_WriteByte(cpu, address + 1, (uint8_t)(data >> 8));
    SM83_WriteByte(cpu, address, (uint8_t)(data & 0xFF));
}

uint8_t SM83_NextByte(sm83_t * cpu)
{
    uint8_t data = SM83_ReadByte(cpu, cpu->PC);
    ++cpu->PC;
    return data;
}

uint16_t SM83_NextWord(sm83_t * cpu)
{
    uint16_t data = SM83_ReadWord(cpu, cpu->PC);
    cpu->PC += 2;
    return data;
}

void SM83_PushWord(sm83_t * cpu, uint16_t data)
{
    cpu->SP -= 2;
    SM83_WriteWord(cpu, cpu->SP, data);
}

uint16_t SM83_PopWord(sm83_t * cpu)
{
    uint16_t data = SM83_ReadWord(cpu, cpu->SP);
    cpu->SP += 2;
    return data;
}