#ifndef ALU_H
#define ALU_H

#include "types.h"

word add16(word a, word b);

word add16s(word a, sbyte b);

byte add8(byte a, byte b);

byte sub8(byte a, byte b);

byte and8(byte a, byte b);

byte or8(byte a, byte b);

byte xor8(byte a, byte b);

#endif // ALU_H