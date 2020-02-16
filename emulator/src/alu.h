#ifndef ALU_H
#define ALU_H

#include <stdint.h>

uint16_t add16(uint16_t a, uint16_t b);

uint16_t add16s(uint16_t a, int16_t b);

uint8_t add8(uint8_t a, uint8_t b);

uint8_t add8s(uint8_t a, int8_t b);

uint8_t sub8(uint8_t a, uint8_t b);

uint8_t sub8s(uint8_t a, int8_t b);

uint8_t and8(uint8_t a, uint8_t b);

uint8_t or8(uint8_t a, uint8_t b);

uint8_t xor8(uint8_t a, uint8_t b);

#endif // ALU_H