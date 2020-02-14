#ifndef ALU_H
#define ALU_H

#include <stdint.h>

uint16_t add16(uint16_t a, uint16_t b);

int8_t add8(uint8_t a, uint8_t b);

int8_t sub8(uint8_t a, uint8_t b);

int8_t and8(uint8_t a, uint8_t b);

int8_t or8(uint8_t a, uint8_t b);

int8_t xor8(uint8_t a, uint8_t b);

#endif // ALU_H