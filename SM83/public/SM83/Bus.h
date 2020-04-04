#ifndef SM83_BUS_H
#define SM83_BUS_H

#include <SM83/Context.h>
#include <SM83/Types.h>

uint8_t SM83_ReadByte(sm83_t * cpu, uint16_t address);

void SM83_WriteByte(sm83_t * cpu, uint16_t address, uint8_t data);

uint16_t SM83_ReadWord(sm83_t * cpu, uint16_t address);

void SM83_WriteWord(sm83_t * cpu, uint16_t address, uint16_t data);

uint8_t SM83_NextByte(sm83_t * cpu);

uint16_t SM83_NextWord(sm83_t * cpu);

void SM83_PushWord(sm83_t * cpu, uint16_t data);

uint16_t SM83_PopWord(sm83_t * cpu);

#endif // SM83_BUS_H