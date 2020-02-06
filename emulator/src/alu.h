#ifndef ALU_H
#define ALU_H

#include <stdint.h>

extern uint8_t DIV;

int8_t add8(int8_t a, int8_t b);
int8_t sub8(int8_t a, int8_t b);

uint16_t add16(uint16_t a, uint16_t b);
uint16_t sub16(uint16_t a, uint16_t b);

uint8_t and8(uint8_t a, uint8_t b);
uint8_t or8(uint8_t a, uint8_t b);
uint8_t xor8(uint8_t a, uint8_t b);

uint8_t inc8(uint8_t a);
uint8_t dec8(uint8_t a);

#endif // ALU_H