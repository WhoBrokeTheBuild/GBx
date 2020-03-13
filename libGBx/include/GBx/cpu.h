#ifndef CPU_H
#define CPU_H

#include "types.h"

typedef struct
{
    union {
        struct {
            union {
                struct {
                    unsigned :4;
                    bool     FC:1; // Carry Flag
                    bool     FH:1; // Half Carry Flag
                    bool     FN:1; // Subtract Flag
                    bool     FZ:1; // Zero Flag
                };
                byte F;
            };
            byte A;
        };
        word AF;
    };
    union {
        struct {
            byte C;
            byte B;
        };
        word BC;
    };
    union {
        struct {
            byte E;
            byte D;
        };
        word DE;
    };
    union {
        struct {
            byte L;
            byte H;
        };
        word HL;
    };
    word SP;
    word PC;

} registers;

extern registers R;

extern bool CPUEnabled;

extern word LastInstructionAddress;

byte Fetch();

void Execute(byte op);

void NextInstruction();

void PrintR();

#endif // CPU_H