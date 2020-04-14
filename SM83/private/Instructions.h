#ifndef SM83_INSTRUCTIONS_H
#define SM83_INSTRUCTIONS_H

#include <SM83/SM83.h>

#include "Internal.h"

#define DEFINE_INSTRUCTION(NAME) static void SM83_INST_##NAME(sm83_t * cpu)

#define DEFINE_BIT_INSTRUCTION(NAME)                                           \
    static void SM83_INST_##NAME(sm83_t * cpu, int bit)

#define DEFINE_SIMPLE_BIT_INSTRUCTION(NAME, EXPR)                              \
    static void SM83_INST_##NAME(sm83_t * cpu, int bit)                        \
    {                                                                          \
        EXPR;                                                                  \
    }

#define DEFINE_SIMPLE_INSTRUCTION(NAME, EXPR)                                  \
    static void SM83_INST_##NAME(sm83_t * cpu)                                 \
    {                                                                          \
        EXPR;                                                                  \
    }

// Miscellenious

DEFINE_INSTRUCTION(NOP)
{
    (void)(cpu);
}

DEFINE_INSTRUCTION(HALT)
{
    cpu->Enabled = false;
}

DEFINE_INSTRUCTION(STOP)
{
    // STOP is technically 0x1000, so skip the second half
    ++cpu->PC;
    // TODO: Fix
    // LCDC.Enabled = false;
}

DEFINE_INSTRUCTION(DI)
{
    cpu->IME = false;
}

DEFINE_INSTRUCTION(EI)
{
    // Enabling IME is delayed by one instruction
    cpu->RequestEnableIME = true;
}

DEFINE_INSTRUCTION(DAA)
{
    if (cpu->FN) {
        // Subtraction
        if (cpu->FC) {
            cpu->A -= 0x60;
        }
        if (cpu->FH) {
            cpu->A -= 0x06;
        }
    }
    else {
        // Addition
        if (cpu->FC || cpu->A > 0x99) {
            cpu->A += 0x60;
            cpu->FC = true;
        }
        if (cpu->FH || (cpu->A & 0x0F) > 0x09) {
            cpu->A += 0x06;
        }
    }

    cpu->FZ = (cpu->A == 0);
    cpu->FH = false;
}

DEFINE_INSTRUCTION(SCF)
{
    cpu->FN = false;
    cpu->FH = false;
    cpu->FC = true;
}

DEFINE_INSTRUCTION(CCF)
{
    cpu->FN = false;
    cpu->FH = false;
    cpu->FC = !cpu->FC;
}

DEFINE_INSTRUCTION(CPL)
{
    cpu->A = ~cpu->A;
    cpu->FN = true;
    cpu->FH = true;
}

// Call

#define DEFINE_INSTRUCTION_CALL_COND(NAME, TEXT, COND)                         \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        uint16_t u16 = SM83_NextWord(cpu);                                     \
        SM83_Tick(cpu, 1);                                                     \
        if (COND) {                                                            \
            SM83_PushWord(cpu, cpu->PC);                                       \
            SM83_PushStackLogEntry(cpu, TEXT " $%04X", u16);                   \
            cpu->PC = u16;                                                     \
        }                                                                      \
    }

DEFINE_INSTRUCTION_CALL_COND(CALL_u16,    "CALL",    true)
DEFINE_INSTRUCTION_CALL_COND(CALL_Z_u16,  "CALL Z",  cpu->FZ)
DEFINE_INSTRUCTION_CALL_COND(CALL_C_u16,  "CALL C",  cpu->FC)
DEFINE_INSTRUCTION_CALL_COND(CALL_NZ_u16, "CALL NZ", !cpu->FZ)
DEFINE_INSTRUCTION_CALL_COND(CALL_NC_u16, "CALL NC", !cpu->FC)

// Restart

#define DEFINE_INSTRUCTION_RST(NAME, TEXT, ADDR)                               \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        SM83_PushWord(cpu, cpu->PC);                                           \
        SM83_PushStackLogEntry(cpu, TEXT);                                     \
        SM83_Tick(cpu, 1);                                                     \
        cpu->PC = (ADDR);                                                      \
    }

DEFINE_INSTRUCTION_RST(RST_00, "RST $00", 0x00)
DEFINE_INSTRUCTION_RST(RST_08, "RST $08", 0x08)
DEFINE_INSTRUCTION_RST(RST_10, "RST $10", 0x10)
DEFINE_INSTRUCTION_RST(RST_18, "RST $18", 0x18)
DEFINE_INSTRUCTION_RST(RST_20, "RST $20", 0x20)
DEFINE_INSTRUCTION_RST(RST_28, "RST $28", 0x28)
DEFINE_INSTRUCTION_RST(RST_30, "RST $30", 0x30)
DEFINE_INSTRUCTION_RST(RST_38, "RST $38", 0x38)

// Return

DEFINE_INSTRUCTION(RET)
{
    uint16_t pc = SM83_PopWord(cpu);
    SM83_PopStackLogEntry(cpu);
    SM83_Tick(cpu, 1);
    cpu->PC = pc;
}

DEFINE_INSTRUCTION(RETI)
{
    uint16_t pc = SM83_PopWord(cpu);
    SM83_PopStackLogEntry(cpu);
    SM83_Tick(cpu, 1);
    cpu->PC = pc;
    cpu->IME = true;
}

#define DEFINE_INSTRUCTION_RET_COND(NAME, COND)                                \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        SM83_Tick(cpu, 2);                                                     \
        if (COND) {                                                            \
            uint16_t pc = SM83_PopWord(cpu);                                   \
            SM83_PopStackLogEntry(cpu);                                        \
            SM83_Tick(cpu, 1);                                                 \
            cpu->PC = pc;                                                      \
        }                                                                      \
    }

DEFINE_INSTRUCTION_RET_COND(RET_Z, cpu->FZ)
DEFINE_INSTRUCTION_RET_COND(RET_C, cpu->FC)
DEFINE_INSTRUCTION_RET_COND(RET_NZ, !cpu->FZ)
DEFINE_INSTRUCTION_RET_COND(RET_NC, !cpu->FC)

// Jump

DEFINE_INSTRUCTION(JP_HL)
{
    cpu->PC = cpu->HL;
}

#define DEFINE_INSTRUCTION_JP_COND(NAME, COND)                                 \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        uint16_t u16 = SM83_NextWord(cpu);                                     \
        if (COND) {                                                            \
            SM83_Tick(cpu, 1);                                                 \
            cpu->PC = u16;                                                     \
        }                                                                      \
    }

DEFINE_INSTRUCTION_JP_COND(JP_u16, true)
DEFINE_INSTRUCTION_JP_COND(JP_Z_u16, cpu->FZ)
DEFINE_INSTRUCTION_JP_COND(JP_C_u16, cpu->FC)
DEFINE_INSTRUCTION_JP_COND(JP_NZ_u16, !cpu->FZ)
DEFINE_INSTRUCTION_JP_COND(JP_NC_u16, !cpu->FC)

#define DEFINE_INSTRUCTION_JR_COND(NAME, COND)                                 \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        int8_t i8 = (int8_t)SM83_NextByte(cpu);                                \
        if (COND) {                                                            \
            SM83_Tick(cpu, 1);                                                 \
            cpu->PC += i8;                                                     \
        }                                                                      \
    }

DEFINE_INSTRUCTION_JR_COND(JR_s8, true)
DEFINE_INSTRUCTION_JR_COND(JR_Z_s8, cpu->FZ)
DEFINE_INSTRUCTION_JR_COND(JR_C_s8, cpu->FC)
DEFINE_INSTRUCTION_JR_COND(JR_NZ_s8, !cpu->FZ)
DEFINE_INSTRUCTION_JR_COND(JR_NC_s8, !cpu->FC)

// Push

#define DEFINE_INSTRUCTION_PUSH(NAME, TEXT, SRC)                               \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        SM83_Tick(cpu, 1);                                                     \
        SM83_PushWord(cpu, (SRC));                                             \
        SM83_PushStackLogEntry(cpu, TEXT);                                     \
    }

DEFINE_INSTRUCTION_PUSH(PUSH_AF, "PUSH AF", cpu->AF)
DEFINE_INSTRUCTION_PUSH(PUSH_BC, "PUSH BC", cpu->BC)
DEFINE_INSTRUCTION_PUSH(PUSH_DE, "PUSH DE", cpu->DE)
DEFINE_INSTRUCTION_PUSH(PUSH_HL, "PUSH HL", cpu->HL)

// Pop

#define DEFINE_INSTRUCTION_POP(NAME, DST)                                      \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        DST = SM83_PopWord(cpu);                                               \
        SM83_PopStackLogEntry(cpu);                                            \
    }

DEFINE_INSTRUCTION_POP(POP_AF, cpu->AF)
DEFINE_INSTRUCTION_POP(POP_BC, cpu->BC)
DEFINE_INSTRUCTION_POP(POP_DE, cpu->DE)
DEFINE_INSTRUCTION_POP(POP_HL, cpu->HL)

// Load

#define DEFINE_INSTRUCTION_LD(NAME, DST, SRC)                                  \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        DST = SRC;                                                             \
    }

#define DEFINE_INSTRUCTION_LD_GROUP(PREFIX, DST)                               \
    DEFINE_INSTRUCTION_LD(PREFIX##_A, DST, cpu->A)                             \
    DEFINE_INSTRUCTION_LD(PREFIX##_B, DST, cpu->B)                             \
    DEFINE_INSTRUCTION_LD(PREFIX##_C, DST, cpu->C)                             \
    DEFINE_INSTRUCTION_LD(PREFIX##_D, DST, cpu->D)                             \
    DEFINE_INSTRUCTION_LD(PREFIX##_E, DST, cpu->E)                             \
    DEFINE_INSTRUCTION_LD(PREFIX##_H, DST, cpu->H)                             \
    DEFINE_INSTRUCTION_LD(PREFIX##_L, DST, cpu->L)                             \
    DEFINE_INSTRUCTION_LD(PREFIX##_pHL, DST, SM83_ReadByte(cpu, cpu->HL))      \
    DEFINE_INSTRUCTION_LD(PREFIX##_u8, DST, SM83_NextByte(cpu))

DEFINE_INSTRUCTION_LD_GROUP(LD_A, cpu->A)
DEFINE_INSTRUCTION_LD_GROUP(LD_B, cpu->B)
DEFINE_INSTRUCTION_LD_GROUP(LD_C, cpu->C)
DEFINE_INSTRUCTION_LD_GROUP(LD_D, cpu->D)
DEFINE_INSTRUCTION_LD_GROUP(LD_E, cpu->E)
DEFINE_INSTRUCTION_LD_GROUP(LD_H, cpu->H)
DEFINE_INSTRUCTION_LD_GROUP(LD_L, cpu->L)

#define DEFINE_INSTRUCTION_LD_WRITE_BYTE(NAME, ADDR, SRC)                      \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        SM83_WriteByte(cpu, ADDR, SRC);                                        \
    }

DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pHL_A, cpu->HL, cpu->A)
DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pHL_B, cpu->HL, cpu->B)
DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pHL_C, cpu->HL, cpu->C)
DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pHL_D, cpu->HL, cpu->D)
DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pHL_E, cpu->HL, cpu->E)
DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pHL_H, cpu->HL, cpu->H)
DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pHL_L, cpu->HL, cpu->L)
DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pHL_u8, cpu->HL, SM83_NextByte(cpu))

DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pBC_A, cpu->BC, cpu->A)
DEFINE_INSTRUCTION_LD_WRITE_BYTE(LD_pDE_A, cpu->DE, cpu->A)
DEFINE_INSTRUCTION_LD(LD_A_pBC, cpu->A, SM83_ReadByte(cpu, cpu->BC))
DEFINE_INSTRUCTION_LD(LD_A_pDE, cpu->A, SM83_ReadByte(cpu, cpu->DE))

DEFINE_INSTRUCTION_LD(LD_BC_u16, cpu->BC, SM83_NextWord(cpu))
DEFINE_INSTRUCTION_LD(LD_DE_u16, cpu->DE, SM83_NextWord(cpu))
DEFINE_INSTRUCTION_LD(LD_HL_u16, cpu->HL, SM83_NextWord(cpu))

DEFINE_INSTRUCTION(LD_SP_u16)
{
    cpu->SP = SM83_NextWord(cpu);
}

DEFINE_INSTRUCTION(LD_SP_HL)
{
    cpu->SP = cpu->HL;
}

DEFINE_INSTRUCTION(LD_pu16_SP)
{
    SM83_WriteWord(cpu, SM83_NextWord(cpu), cpu->SP);
}

DEFINE_INSTRUCTION(LD_pu16_A)
{
    SM83_WriteWord(cpu, SM83_NextWord(cpu), cpu->A);
}

DEFINE_INSTRUCTION(LD_A_pu16)
{
    cpu->A = SM83_ReadByte(cpu, SM83_NextWord(cpu));
}

DEFINE_INSTRUCTION(LDI_pHL_A)
{
    SM83_WriteByte(cpu, cpu->HL, cpu->A);
    ++cpu->HL;
}

DEFINE_INSTRUCTION(LDD_pHL_A)
{
    SM83_WriteByte(cpu, cpu->HL, cpu->A);
    --cpu->HL;
}

DEFINE_INSTRUCTION(LDI_A_pHL)
{
    cpu->A = SM83_ReadByte(cpu, cpu->HL);
    ++cpu->HL;
}

DEFINE_INSTRUCTION(LDD_A_pHL)
{
    cpu->A = SM83_ReadByte(cpu, cpu->HL);
    --cpu->HL;
}

DEFINE_INSTRUCTION(LDH_pu8_A)
{
    SM83_WriteByte(cpu, 0xFF00 + SM83_NextByte(cpu), cpu->A);
}

DEFINE_INSTRUCTION(LDH_A_pu8)
{
    cpu->A = SM83_ReadByte(cpu, 0xFF00 + SM83_NextByte(cpu));
}

DEFINE_INSTRUCTION(LDH_pC_A)
{
    SM83_WriteByte(cpu, 0xFF00 + cpu->C, cpu->A);
}

DEFINE_INSTRUCTION(LDH_A_pC)
{
    cpu->A = SM83_ReadByte(cpu, 0xFF00 + cpu->C);
}

DEFINE_INSTRUCTION(LD_HL_SP_s8)
{
    int8_t s8 = (int8_t)SM83_NextByte(cpu);

    uint32_t result = cpu->SP + s8;
    cpu->FN = false;
    cpu->FH = (((cpu->SP & 0x7FF) + s8) & 0x800) == 0x800;
    cpu->FC = ((result & 0xFFFF0000) > 0);
    cpu->HL = (uint16_t)result;

    SM83_Tick(cpu, 1);
}

// Add

#define _ADD(X)                                                                \
    uint8_t tmp = (X);                                                         \
    uint16_t result = cpu->A + tmp;                                            \
    cpu->FZ = ((result & 0xFF) == 0);                                          \
    cpu->FN = false;                                                           \
    cpu->FH = ((((cpu->A & 0xF) + (tmp & 0xF)) & 0x10) == 0x10);               \
    cpu->FC = ((result & 0xFF00) > 0);                                         \
    cpu->A = (uint8_t)result

DEFINE_SIMPLE_INSTRUCTION(ADD_A, _ADD(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(ADD_B, _ADD(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(ADD_C, _ADD(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(ADD_D, _ADD(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(ADD_E, _ADD(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(ADD_H, _ADD(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(ADD_L, _ADD(cpu->L))
DEFINE_SIMPLE_INSTRUCTION(ADD_pHL, _ADD(SM83_ReadByte(cpu, cpu->HL)))
DEFINE_SIMPLE_INSTRUCTION(ADD_u8, _ADD(SM83_NextByte(cpu)))

#define DEFINE_INSTRUCTION_ADC(NAME, VALUE)                                    \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        _ADD((VALUE) + (uint8_t)cpu->FC);                                      \
    }

DEFINE_INSTRUCTION_ADC(ADC_A, cpu->A)
DEFINE_INSTRUCTION_ADC(ADC_B, cpu->B)
DEFINE_INSTRUCTION_ADC(ADC_C, cpu->C)
DEFINE_INSTRUCTION_ADC(ADC_D, cpu->D)
DEFINE_INSTRUCTION_ADC(ADC_E, cpu->E)
DEFINE_INSTRUCTION_ADC(ADC_H, cpu->H)
DEFINE_INSTRUCTION_ADC(ADC_L, cpu->L)
DEFINE_INSTRUCTION_ADC(ADC_pHL, SM83_ReadByte(cpu, cpu->HL))
DEFINE_INSTRUCTION_ADC(ADC_u8, SM83_NextByte(cpu))

#define _ADD_HL(X)                                                             \
    uint16_t tmp = (X);                                                        \
    uint32_t result = cpu->HL + tmp;                                           \
    cpu->FN = false;                                                           \
    cpu->FH = ((((cpu->HL & 0x7FF) + (tmp & 0x7FF)) & 0x800) == 0x800);        \
    cpu->FC = ((result & 0xFFFF0000) > 0);                                     \
    cpu->HL = (uint16_t)result

#define DEFINE_INSTRUCTION_ADD_HL(NAME, VALUE)                                 \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        SM83_Tick(cpu, 1);                                                     \
        _ADD_HL(VALUE);                                                        \
    }

DEFINE_INSTRUCTION_ADD_HL(ADD_HL_BC, cpu->BC)
DEFINE_INSTRUCTION_ADD_HL(ADD_HL_DE, cpu->DE)
DEFINE_INSTRUCTION_ADD_HL(ADD_HL_HL, cpu->HL)
DEFINE_INSTRUCTION_ADD_HL(ADD_HL_SP, cpu->SP)

DEFINE_INSTRUCTION(ADD_SP_s8)
{
    int8_t s8 = (int8_t)SM83_NextByte(cpu);

    uint32_t result = cpu->SP + s8;
    cpu->FN = false;
    cpu->FH = ((((cpu->SP & 0x7FF) + s8) & 0x800) == 0x800);
    cpu->FC = ((result & 0xFFFF0000) > 0);
    cpu->SP = (uint16_t)result;

    SM83_Tick(cpu, 1);
}

// Subtract

#define _CP(X)                                                                 \
    uint8_t tmp = (X);                                                         \
    uint16_t result = cpu->A - tmp;                                            \
    cpu->FZ = ((result & 0xFF) == 0);                                          \
    cpu->FN = true;                                                            \
    cpu->FH = ((tmp & 0xF) > (cpu->A & 0xF));                                  \
    cpu->FC = (result & 0xFF00) > 0

#define _SUB(X)                                                                \
    _CP(X);                                                                    \
    cpu->A = (uint8_t)result

DEFINE_SIMPLE_INSTRUCTION(SUB_A, _SUB(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(SUB_B, _SUB(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(SUB_C, _SUB(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(SUB_D, _SUB(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(SUB_E, _SUB(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(SUB_H, _SUB(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(SUB_L, _SUB(cpu->L))
DEFINE_SIMPLE_INSTRUCTION(SUB_pHL, _SUB(SM83_ReadByte(cpu, cpu->HL)))
DEFINE_SIMPLE_INSTRUCTION(SUB_u8, _SUB(SM83_NextByte(cpu)))

#define DEFINE_INSTRUCTION_SBC(NAME, VALUE)                                    \
    DEFINE_INSTRUCTION(NAME)                                                   \
    {                                                                          \
        _SUB((VALUE) + (uint8_t)cpu->FC);                                      \
    }

DEFINE_INSTRUCTION_SBC(SBC_A, cpu->A)
DEFINE_INSTRUCTION_SBC(SBC_B, cpu->B)
DEFINE_INSTRUCTION_SBC(SBC_C, cpu->C)
DEFINE_INSTRUCTION_SBC(SBC_D, cpu->D)
DEFINE_INSTRUCTION_SBC(SBC_E, cpu->E)
DEFINE_INSTRUCTION_SBC(SBC_H, cpu->H)
DEFINE_INSTRUCTION_SBC(SBC_L, cpu->L)
DEFINE_INSTRUCTION_SBC(SBC_pHL, SM83_ReadByte(cpu, cpu->HL))
DEFINE_INSTRUCTION_SBC(SBC_u8, SM83_NextByte(cpu))

// Increment

#define _INC(X)                                                                \
    cpu->FH = (((X)&0xF) == 0xF);                                              \
    ++(X);                                                                     \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false

DEFINE_SIMPLE_INSTRUCTION(INC_A, _INC(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(INC_B, _INC(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(INC_C, _INC(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(INC_D, _INC(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(INC_E, _INC(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(INC_H, _INC(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(INC_L, _INC(cpu->L))

DEFINE_INSTRUCTION(INC_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _INC(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

DEFINE_INSTRUCTION(INC_BC)
{
    SM83_Tick(cpu, 1);
    ++cpu->BC;
}

DEFINE_INSTRUCTION(INC_DE)
{
    SM83_Tick(cpu, 1);
    ++cpu->DE;
}

DEFINE_INSTRUCTION(INC_HL)
{
    SM83_Tick(cpu, 1);
    ++cpu->HL;
}

DEFINE_INSTRUCTION(INC_SP)
{
    SM83_Tick(cpu, 1);
    ++cpu->SP;
}

// Decrement

#define _DEC(X)                                                                \
    cpu->FH = (((X)&0xF) == 0);                                                \
    --(X);                                                                     \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = true

DEFINE_SIMPLE_INSTRUCTION(DEC_A, _DEC(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(DEC_B, _DEC(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(DEC_C, _DEC(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(DEC_D, _DEC(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(DEC_E, _DEC(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(DEC_H, _DEC(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(DEC_L, _DEC(cpu->L))

DEFINE_INSTRUCTION(DEC_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _DEC(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

DEFINE_INSTRUCTION(DEC_BC)
{
    SM83_Tick(cpu, 1);
    --cpu->BC;
}

DEFINE_INSTRUCTION(DEC_DE)
{
    SM83_Tick(cpu, 1);
    --cpu->DE;
}

DEFINE_INSTRUCTION(DEC_HL)
{
    SM83_Tick(cpu, 1);
    --cpu->HL;
}

DEFINE_INSTRUCTION(DEC_SP)
{
    SM83_Tick(cpu, 1);
    --cpu->SP;
}

// Compare

// See _SUB for _CP

DEFINE_SIMPLE_INSTRUCTION(CP_A, _CP(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(CP_B, _CP(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(CP_C, _CP(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(CP_D, _CP(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(CP_E, _CP(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(CP_H, _CP(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(CP_L, _CP(cpu->L))
DEFINE_SIMPLE_INSTRUCTION(CP_pHL, _CP(SM83_ReadByte(cpu, cpu->HL)))
DEFINE_SIMPLE_INSTRUCTION(CP_u8, _CP(SM83_NextByte(cpu)))

// Bitwise AND

#define _AND(X)                                                                \
    cpu->A &= (X);                                                             \
    cpu->FZ = (cpu->A == 0);                                                   \
    cpu->FN = false;                                                           \
    cpu->FH = true;                                                            \
    cpu->FC = false;

DEFINE_SIMPLE_INSTRUCTION(AND_A, _AND(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(AND_B, _AND(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(AND_C, _AND(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(AND_D, _AND(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(AND_E, _AND(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(AND_H, _AND(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(AND_L, _AND(cpu->L))
DEFINE_SIMPLE_INSTRUCTION(AND_pHL, _AND(SM83_ReadByte(cpu, cpu->HL)))
DEFINE_SIMPLE_INSTRUCTION(AND_u8, _AND(SM83_NextByte(cpu)))

// Bitwise OR

#define _OR(X)                                                                 \
    cpu->A |= (X);                                                             \
    cpu->FZ = (cpu->A == 0);                                                   \
    cpu->FN = false;                                                           \
    cpu->FH = false;                                                           \
    cpu->FC = false;

DEFINE_SIMPLE_INSTRUCTION(OR_A, _OR(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(OR_B, _OR(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(OR_C, _OR(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(OR_D, _OR(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(OR_E, _OR(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(OR_H, _OR(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(OR_L, _OR(cpu->L))
DEFINE_SIMPLE_INSTRUCTION(OR_pHL, _OR(SM83_ReadByte(cpu, cpu->HL)))
DEFINE_SIMPLE_INSTRUCTION(OR_u8, _OR(SM83_NextByte(cpu)))

// Bitwise XOR

#define _XOR(X)                                                                \
    cpu->A ^= (X);                                                             \
    cpu->FZ = (cpu->A == 0);                                                   \
    cpu->FN = false;                                                           \
    cpu->FH = false;                                                           \
    cpu->FC = false;

DEFINE_SIMPLE_INSTRUCTION(XOR_A, _XOR(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(XOR_B, _XOR(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(XOR_C, _XOR(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(XOR_D, _XOR(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(XOR_E, _XOR(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(XOR_H, _XOR(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(XOR_L, _XOR(cpu->L))
DEFINE_SIMPLE_INSTRUCTION(XOR_pHL, _XOR(SM83_ReadByte(cpu, cpu->HL)))
DEFINE_SIMPLE_INSTRUCTION(XOR_u8, _XOR(SM83_NextByte(cpu)))

// Rotate

#define _RLC(X)                                                                \
    cpu->FC = (((X)&0x80) == 0x80);                                            \
    (X) = ((X) << 1) | cpu->FC;                                                \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false;                                                           \
    cpu->FH = false;

DEFINE_SIMPLE_INSTRUCTION(RLC_A, _RLC(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(RLC_B, _RLC(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(RLC_C, _RLC(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(RLC_D, _RLC(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(RLC_E, _RLC(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(RLC_H, _RLC(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(RLC_L, _RLC(cpu->L))

DEFINE_INSTRUCTION(RLC_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _RLC(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

#define _RRC(X)                                                                \
    cpu->FC = (((X)&0x01) == 0x01);                                            \
    (X) = ((X) >> 1) | (cpu->FC << 7);                                         \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false;                                                           \
    cpu->FH = false;

DEFINE_SIMPLE_INSTRUCTION(RRC_A, _RRC(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(RRC_B, _RRC(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(RRC_C, _RRC(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(RRC_D, _RRC(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(RRC_E, _RRC(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(RRC_H, _RRC(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(RRC_L, _RRC(cpu->L))

DEFINE_INSTRUCTION(RRC_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _RRC(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

#define _RL(X)                                                                 \
    bool fc = (((X)&0x80) == 0x80);                                            \
    (X) = ((X) << 1) | (uint8_t)cpu->FC;                                       \
    cpu->FC = fc;                                                              \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false;                                                           \
    cpu->FH = false;

DEFINE_SIMPLE_INSTRUCTION(RL_A, _RL(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(RL_B, _RL(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(RL_C, _RL(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(RL_D, _RL(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(RL_E, _RL(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(RL_H, _RL(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(RL_L, _RL(cpu->L))

DEFINE_INSTRUCTION(RL_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _RL(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

#define _RR(X)                                                                 \
    bool fc = (((X)&0x01) == 0x01);                                            \
    (X) = ((X) >> 1) | (cpu->FC << 7);                                         \
    cpu->FC = fc;                                                              \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false;                                                           \
    cpu->FH = false;

DEFINE_SIMPLE_INSTRUCTION(RR_A, _RR(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(RR_B, _RR(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(RR_C, _RR(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(RR_D, _RR(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(RR_E, _RR(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(RR_H, _RR(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(RR_L, _RR(cpu->L))

DEFINE_INSTRUCTION(RR_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _RR(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

// Shift

#define _SLA(X)                                                                \
    cpu->FC = ((X)&0x80) == 0x80;                                              \
    (X) <<= 1;                                                                 \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false;                                                           \
    cpu->FH = false;

DEFINE_SIMPLE_INSTRUCTION(SLA_A, _SLA(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(SLA_B, _SLA(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(SLA_C, _SLA(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(SLA_D, _SLA(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(SLA_E, _SLA(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(SLA_H, _SLA(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(SLA_L, _SLA(cpu->L))

DEFINE_INSTRUCTION(SLA_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _SLA(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

#define _SRA(X)                                                                \
    cpu->FC = ((X)&0x01) == 0x01;                                              \
    (X) = ((X) >> 1) | ((X)&0x80);                                             \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false;                                                           \
    cpu->FH = false;

DEFINE_SIMPLE_INSTRUCTION(SRA_A, _SRA(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(SRA_B, _SRA(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(SRA_C, _SRA(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(SRA_D, _SRA(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(SRA_E, _SRA(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(SRA_H, _SRA(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(SRA_L, _SRA(cpu->L))

DEFINE_INSTRUCTION(SRA_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _SRA(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

#define _SRL(X)                                                                \
    cpu->FC = ((X)&0x01) == 0x01;                                              \
    (X) >>= 1;                                                                 \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false;                                                           \
    cpu->FH = false;

DEFINE_SIMPLE_INSTRUCTION(SRL_A, _SRL(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(SRL_B, _SRL(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(SRL_C, _SRL(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(SRL_D, _SRL(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(SRL_E, _SRL(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(SRL_H, _SRL(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(SRL_L, _SRL(cpu->L))

DEFINE_INSTRUCTION(SRL_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _SRL(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

// Swap

#define _SWAP(X)                                                               \
    (X) = (((X)&0x0F) << 4) | (((X)&0xF0) >> 4);                               \
    cpu->FZ = ((X) == 0);                                                      \
    cpu->FN = false;                                                           \
    cpu->FH = false;                                                           \
    cpu->FC = false;

DEFINE_SIMPLE_INSTRUCTION(SWAP_A, _SWAP(cpu->A))
DEFINE_SIMPLE_INSTRUCTION(SWAP_B, _SWAP(cpu->B))
DEFINE_SIMPLE_INSTRUCTION(SWAP_C, _SWAP(cpu->C))
DEFINE_SIMPLE_INSTRUCTION(SWAP_D, _SWAP(cpu->D))
DEFINE_SIMPLE_INSTRUCTION(SWAP_E, _SWAP(cpu->E))
DEFINE_SIMPLE_INSTRUCTION(SWAP_H, _SWAP(cpu->H))
DEFINE_SIMPLE_INSTRUCTION(SWAP_L, _SWAP(cpu->L))

DEFINE_INSTRUCTION(SWAP_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _SWAP(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

// Test Bit

#define _BIT(X)                                                                \
    cpu->FZ = (((X) & (1 << bit)) == 0);                                       \
    cpu->FN = false;                                                           \
    cpu->FH = true;

DEFINE_SIMPLE_BIT_INSTRUCTION(BIT_b_A, _BIT(cpu->A))
DEFINE_SIMPLE_BIT_INSTRUCTION(BIT_b_B, _BIT(cpu->B))
DEFINE_SIMPLE_BIT_INSTRUCTION(BIT_b_C, _BIT(cpu->C))
DEFINE_SIMPLE_BIT_INSTRUCTION(BIT_b_D, _BIT(cpu->D))
DEFINE_SIMPLE_BIT_INSTRUCTION(BIT_b_E, _BIT(cpu->E))
DEFINE_SIMPLE_BIT_INSTRUCTION(BIT_b_H, _BIT(cpu->H))
DEFINE_SIMPLE_BIT_INSTRUCTION(BIT_b_L, _BIT(cpu->L))
DEFINE_SIMPLE_BIT_INSTRUCTION(BIT_b_pHL, _BIT(SM83_ReadByte(cpu, cpu->HL)))

// Reset Bit

#define _RES(X) (X) &= ~(1 << bit);

DEFINE_SIMPLE_BIT_INSTRUCTION(RES_b_A, _BIT(cpu->A))
DEFINE_SIMPLE_BIT_INSTRUCTION(RES_b_B, _BIT(cpu->B))
DEFINE_SIMPLE_BIT_INSTRUCTION(RES_b_C, _BIT(cpu->C))
DEFINE_SIMPLE_BIT_INSTRUCTION(RES_b_D, _BIT(cpu->D))
DEFINE_SIMPLE_BIT_INSTRUCTION(RES_b_E, _BIT(cpu->E))
DEFINE_SIMPLE_BIT_INSTRUCTION(RES_b_H, _BIT(cpu->H))
DEFINE_SIMPLE_BIT_INSTRUCTION(RES_b_L, _BIT(cpu->L))

DEFINE_BIT_INSTRUCTION(RES_b_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _RES(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

// Set Bit

#define _SET(X) (X) |= (1 << bit);

DEFINE_SIMPLE_BIT_INSTRUCTION(SET_b_A, _SET(cpu->A))
DEFINE_SIMPLE_BIT_INSTRUCTION(SET_b_B, _SET(cpu->B))
DEFINE_SIMPLE_BIT_INSTRUCTION(SET_b_C, _SET(cpu->C))
DEFINE_SIMPLE_BIT_INSTRUCTION(SET_b_D, _SET(cpu->D))
DEFINE_SIMPLE_BIT_INSTRUCTION(SET_b_E, _SET(cpu->E))
DEFINE_SIMPLE_BIT_INSTRUCTION(SET_b_H, _SET(cpu->H))
DEFINE_SIMPLE_BIT_INSTRUCTION(SET_b_L, _SET(cpu->L))

DEFINE_BIT_INSTRUCTION(SET_b_pHL)
{
    uint8_t u8 = SM83_ReadByte(cpu, cpu->HL);

    _SET(u8);

    SM83_WriteByte(cpu, cpu->HL, u8);
}

// 0xCB Prefix

DEFINE_INSTRUCTION(CB)
{
    static const sm83_instruction_t instructions[] = {
        // Rotate
        [0x00] = SM83_INST_RLC_B,
        [0x01] = SM83_INST_RLC_C,
        [0x02] = SM83_INST_RLC_D,
        [0x03] = SM83_INST_RLC_E,
        [0x04] = SM83_INST_RLC_H,
        [0x05] = SM83_INST_RLC_L,
        [0x06] = SM83_INST_RLC_pHL,
        [0x07] = SM83_INST_RLC_A,
        [0x08] = SM83_INST_RRC_B,
        [0x09] = SM83_INST_RRC_C,
        [0x0A] = SM83_INST_RRC_D,
        [0x0B] = SM83_INST_RRC_E,
        [0x0C] = SM83_INST_RRC_H,
        [0x0D] = SM83_INST_RRC_L,
        [0x0E] = SM83_INST_RRC_pHL,
        [0x0F] = SM83_INST_RRC_A,
        [0x10] = SM83_INST_RL_B,
        [0x11] = SM83_INST_RL_C,
        [0x12] = SM83_INST_RL_D,
        [0x13] = SM83_INST_RL_E,
        [0x14] = SM83_INST_RL_H,
        [0x15] = SM83_INST_RL_L,
        [0x16] = SM83_INST_RL_pHL,
        [0x17] = SM83_INST_RL_A,
        [0x18] = SM83_INST_RR_B,
        [0x19] = SM83_INST_RR_C,
        [0x1A] = SM83_INST_RR_D,
        [0x1B] = SM83_INST_RR_E,
        [0x1C] = SM83_INST_RR_H,
        [0x1D] = SM83_INST_RR_L,
        [0x1E] = SM83_INST_RR_pHL,
        [0x1F] = SM83_INST_RR_A,
        // Shift
        [0x20] = SM83_INST_SLA_B,
        [0x21] = SM83_INST_SLA_C,
        [0x22] = SM83_INST_SLA_D,
        [0x23] = SM83_INST_SLA_E,
        [0x24] = SM83_INST_SLA_H,
        [0x25] = SM83_INST_SLA_L,
        [0x26] = SM83_INST_SLA_pHL,
        [0x27] = SM83_INST_SLA_A,
        [0x28] = SM83_INST_SRA_B,
        [0x29] = SM83_INST_SRA_C,
        [0x2A] = SM83_INST_SRA_D,
        [0x2B] = SM83_INST_SRA_E,
        [0x2C] = SM83_INST_SRA_H,
        [0x2D] = SM83_INST_SRA_L,
        [0x2E] = SM83_INST_SRA_pHL,
        [0x2F] = SM83_INST_SRA_A,
        [0x38] = SM83_INST_SRL_B,
        [0x39] = SM83_INST_SRL_C,
        [0x3A] = SM83_INST_SRL_D,
        [0x3B] = SM83_INST_SRL_E,
        [0x3C] = SM83_INST_SRL_H,
        [0x3D] = SM83_INST_SRL_L,
        [0x3E] = SM83_INST_SRL_pHL,
        [0x3F] = SM83_INST_SRL_A,
        // Swap
        [0x30] = SM83_INST_SWAP_B,
        [0x31] = SM83_INST_SWAP_C,
        [0x32] = SM83_INST_SWAP_D,
        [0x33] = SM83_INST_SWAP_E,
        [0x34] = SM83_INST_SWAP_H,
        [0x35] = SM83_INST_SWAP_L,
        [0x36] = SM83_INST_SWAP_pHL,
        [0x37] = SM83_INST_SWAP_A,
    };

    static const sm83_bit_instruction_t bitInstructions[] = {
        // Test Bit
        SM83_INST_BIT_b_B,
        SM83_INST_BIT_b_C,
        SM83_INST_BIT_b_D,
        SM83_INST_BIT_b_E,
        SM83_INST_BIT_b_H,
        SM83_INST_BIT_b_L,
        SM83_INST_BIT_b_pHL,
        SM83_INST_BIT_b_A,
        // Reset Bit
        SM83_INST_RES_b_B,
        SM83_INST_RES_b_C,
        SM83_INST_RES_b_D,
        SM83_INST_RES_b_E,
        SM83_INST_RES_b_H,
        SM83_INST_RES_b_L,
        SM83_INST_RES_b_pHL,
        SM83_INST_RES_b_A,
        // Set Bit
        SM83_INST_SET_b_B,
        SM83_INST_SET_b_C,
        SM83_INST_SET_b_D,
        SM83_INST_SET_b_E,
        SM83_INST_SET_b_H,
        SM83_INST_SET_b_L,
        SM83_INST_SET_b_pHL,
        SM83_INST_SET_b_A,
    };

    uint8_t opcode = SM83_NextByte(cpu);
    if (opcode < 0x40) {
        instructions[opcode](cpu);
    }
    else {
        uint8_t tmp = opcode - 0x40;
        // 0-7
        int bit = (tmp % 8) + (tmp / 16);
        // 0-2
        int inst = tmp / 64;
        // 0-7
        int variant = tmp % 8;
        bitInstructions[(inst * 8) + variant](cpu, bit);
    }
}

#endif // SM83_INSTRUCTIONS_H