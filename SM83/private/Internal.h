#ifndef SM83_INTERNAL_H
#define SM83_INTERNAL_H

#include <SM83/Context.h>
#include <SM83/Types.h>

#define INSTRUCTION_LOG_LENGTH     (64)
#define INSTRUCTION_LOG_ENTRY_SIZE (24)

#define STACK_LOG_LENGTH     (64)
#define STACK_LOG_ENTRY_SIZE (24)

typedef struct sm83_internal
{
    unsigned ClockSpeed;

    sm83_mode_t Mode;

    uintmax_t TotalTicks;

    // Debug

    uint16_t LastInstructionAddress;

    bool InstructionLoggingEnabled;

    char InstructionLog[INSTRUCTION_LOG_LENGTH][INSTRUCTION_LOG_ENTRY_SIZE];

    int InstructionLogIndex;

    bool StackLoggingEnabled;

    char StackLog[STACK_LOG_LENGTH][STACK_LOG_ENTRY_SIZE];

    int StackLogIndex;

} sm83_internal_t;

static SM83_INLINE void SM83_Tick(sm83_t * ctx, unsigned mcycles)
{
    ctx->internal->TotalTicks += mcycles;
    ctx->Tick(ctx->UserData, mcycles);
}

static SM83_INLINE uint8_t SM83_ReadByte(sm83_t * cpu, uint16_t address)
{
    SM83_Tick(cpu, 1);
    return cpu->ReadByte(cpu->UserData, address);
}

static SM83_INLINE void SM83_WriteByte(sm83_t * cpu, uint16_t address, uint8_t data)
{
    SM83_Tick(cpu, 1);
    cpu->WriteByte(cpu->UserData, address, data);
}

static SM83_INLINE uint16_t SM83_ReadWord(sm83_t * cpu, uint16_t address)
{
    return (SM83_ReadByte(cpu, address + 1) << 8) | SM83_ReadByte(cpu, address);
}

static SM83_INLINE void SM83_WriteWord(sm83_t * cpu, uint16_t address, uint16_t data)
{
    SM83_WriteByte(cpu, address + 1, (uint8_t)(data >> 8));
    SM83_WriteByte(cpu, address, (uint8_t)(data & 0xFF));
}

static SM83_INLINE uint8_t SM83_NextByte(sm83_t * cpu)
{
    uint8_t data = SM83_ReadByte(cpu, cpu->PC);
    ++cpu->PC;
    return data;
}

static SM83_INLINE uint16_t SM83_NextWord(sm83_t * cpu)
{
    cpu->PC += 2;
    return SM83_ReadWord(cpu, cpu->PC - 2);
}

static SM83_INLINE void SM83_PushWord(sm83_t * cpu, uint16_t data)
{
    cpu->SP -= 2;
    SM83_WriteWord(cpu, cpu->SP, data);
}

static SM83_INLINE uint16_t SM83_PopWord(sm83_t * cpu)
{
    cpu->SP += 2;
    return SM83_ReadWord(cpu, cpu->SP - 2);
}

#endif // SM83_INTERNAL_H