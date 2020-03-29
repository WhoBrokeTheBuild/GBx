#ifndef CPU_H
#define CPU_H

#include <GBx/Types.h>

#define DMG_CLOCK_SPEED (4194304) // Hz
#define SGB_CLOCK_SPEED (4295454) // Hz
#define CGB_CLOCK_SPEED (8388608) // Hz

typedef struct
{
    union {
        struct {
            union {
                struct {
                    uint :4;
                    bool FC:1; // Carry Flag
                    bool FH:1; // Half Carry Flag
                    bool FN:1; // Subtract Flag
                    bool FZ:1; // Zero Flag
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

extern uint64_t TotalTicks;

extern unsigned ClockSpeed;

extern word LastInstructionAddress;

extern word StackBaseAddress;

void ResetCPU();

void Tick(unsigned ticks);

byte Fetch();

void Execute(byte opcode);

void NextInstruction();

void PrintRegisters();

#endif // CPU_H