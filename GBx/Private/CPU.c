#include <GBx/CPU.h>
#include <GBx/Context.h>
#include <GBx/Interrupt.h>
#include <GBx/MMU.h>
#include <GBx/Log.h>

#include <string.h>

void GBx_CPU_Reset(GBx * ctx)
{
    ctx->CPUEnabled = true;

    ctx->AF = 0x0080;
    ctx->BC = 0x0000;
    ctx->DE = 0x0000;
    ctx->HL = 0x0000;
    
    if (ctx->HaveBootstrap) {
        ctx->PC = 0x0000;
        ctx->SP = 0x0000;
    }
    else {
        ctx->PC = 0x0100;
        ctx->SP = 0xFFFE;

        ctx->L = 0x7C;

        switch (ctx->Model) {
        case GBX_MODEL_DMG:
            ctx->A = 0x01;
            
            // $FF = DMG-CPU
            // ctx->B = 0xFF;
            
            // $13 = DMG-CPU A/B/C
            ctx->C = 0x13;
            break;
        case GBX_MODEL_SGB:
            ctx->A = 0x01;
            break;
        case GBX_MODEL_MGB:
            ctx->A = 0xFF;
            break;
        case GBX_MODEL_CGB:
            ctx->A = 0x11;
            break;
        default:
            break;
        }
    }

    ctx->IME = true;
    ctx->RequestEnableIME = false;

    ctx->IF._raw = 0x00;
    ctx->IE._raw = 0x00;

    ctx->WRAMBank = 1;
    for (unsigned i = 0; i < GBX_WRAM_BANK_COUNT; ++i) {
        memset(ctx->WRAM[i], 0, GBX_WRAM_BANK_SIZE);
    }

    ctx->LastInstructionAddress = 0x00;
}

void GBx_CPU_Print(GBx * ctx)
{
    GBxLogInfo(
        "AF=$%04X BC=$%04X DE=$%04X HL=$%04X SP=$%04X PC=$%04X F=[%c%c%c%c]",
        ctx->AF,
        ctx->BC,
        ctx->DE,
        ctx->HL,
        ctx->SP,
        ctx->PC,
        (ctx->F.Z ? 'Z' : '-'),
        (ctx->F.N ? 'N' : '-'),
        (ctx->F.H ? 'H' : '-'),
        (ctx->F.C ? 'C' : '-'));
}

unsigned GBx_CPU_Step(GBx * ctx)
{
    if (!ctx->CPUEnabled) {
        return 4;
    }

    unsigned cycles = 0;

    ctx->LastInstructionAddress = ctx->PC;

    bool willEnableIME = ctx->RequestEnableIME;

    int8_t s8;
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;

    int bits;

    cycles += GBx_CheckInterrupts(ctx);

    uint8_t op = GBx_NextByte(ctx);
    cycles += 1;

    switch (op) {

    // NOP
    case 0x00:
        break;

    // STOP
    case 0x10:
        // Skip $00
        ++ctx->PC;
        // TODO: Improve
        break;

    // HALT
    case 0x76:
        ctx->CPUEnabled = false;
        // TODO: Improve
        break;

    // DI
    case 0xF3:
        ctx->IME = false;
        break;

    // EI
    case 0xFB:
        // Enabling IME is delayed by one instruction
        ctx->RequestEnableIME = true;
        break;

    // DAA
    case 0x27:
        if (ctx->F.N) {
            // Subtraction
            if (ctx->F.C) {
                ctx->A -= 0x60;
            }
            if (ctx->F.H) {
                ctx->A -= 0x06;
            }
        }
        else {
            // Addition
            if (ctx->F.C || ctx->A > 0x99) {
                ctx->A += 0x60;
                ctx->F.C = true;
            }
            if (ctx->F.H || (ctx->A & 0x0F) > 0x09) {
                ctx->A += 0x06;
            }
        }

        ctx->F.Z = (ctx->A == 0);
        ctx->F.H = false;
        break;

    // SFC
    case 0x37:
        ctx->F.N = false;
        ctx->F.H = false;
        ctx->F.C = true;
        break;

    // CCF
    case 0x3F:
        ctx->F.N = false;
        ctx->F.H = false;
        ctx->F.C = ~ctx->F.C;
        break;

    // CPL
    case 0x2F:
        ctx->A = ~ctx->A;
        ctx->F.N = true;
        ctx->F.H = true;
        break;
    
    #define _CALL(COND)                                                       \
        u16 = GBx_NextWord(ctx);                                              \
        cycles += 2;                                                          \
        if (COND) {                                                           \
            GBx_PushWord(ctx, ctx->PC);                                       \
            ctx->PC = u16;                                                    \
            cycles += 3;                                                      \
        }                                                                     \

    // CALL u16
    case 0xCD:
        _CALL(true);
        break;

    // CALL NZ,u16
    case 0xC4:
        _CALL(!ctx->F.Z);
        break;

    // CALL Z,u16
    case 0xCC:
        _CALL(ctx->F.Z);
        break;

    // CALL NC,u16
    case 0xD4:
        _CALL(!ctx->F.C);
        break;

    // CALL C,u16
    case 0xDC:
        _CALL(ctx->F.C);
        break;

    #define _RST(ADDR)                                                        \
        GBx_PushWord(ctx, ctx->PC);                                           \
        ctx->PC = (ADDR);                                                     \
        cycles += 3

    // RST $00
    case 0xC7:
        _RST(0x00);
        break;
    
    // RST $08
    case 0xCF:
        _RST(0x08);
        break;
    
    // RST $10
    case 0xD7:
        _RST(0x10);
        break;
    
    // RST $18
    case 0xDF:
        _RST(0x18);
        break;
    
    // RST $20
    case 0xE7:
        _RST(0x20);
        break;
    
    // RST $28
    case 0xEF:
        _RST(0x28);
        break;
    
    // RST $30
    case 0xF7:
        _RST(0x30);
        break;
    
    // RST $38
    case 0xFF:
        _RST(0x38);
        break;

    // RET
    case 0xC9:
        u16 = GBx_PopWord(ctx);
        ctx->PC = u16;
        cycles += 3;
        break;

    // RETI
    case 0xD9:
        u16 = GBx_PopWord(ctx);
        ctx->PC = u16;
        ctx->IME = true;
        cycles += 3;
        break;
    
    #define _RET(COND)                                                        \
        cycles += 1;                                                          \
        if (COND) {                                                           \
            u16 = GBx_PopWord(ctx);                                           \
            ctx->PC = u16;                                                    \
            cycles += 3;                                                      \
        }

    // RET NZ
    case 0xC0:
        _RET(!ctx->F.Z);
        break;
    
    // RET Z
    case 0xC8:
        _RET(ctx->F.Z);
        break;
    
    // RET NC
    case 0xD0:
        _RET(!ctx->F.C);
        break;
    
    // RET C
    case 0xD8:
        _RET(ctx->F.C);
        break;
    
    // JP (HL)
    case 0xE9:
        ctx->PC = ctx->HL;
        break;

    #define _JP(COND)                                                         \
        u16 = GBx_NextWord(ctx);                                              \
        cycles += 2;                                                          \
        if (COND) {                                                           \
            cycles += 1;                                                      \
            ctx->PC = u16;                                                    \
        }

    // JP u16
    case 0xC3:
        _JP(true);
        break;
    
    // JP NZ,u16
    case 0xC2:
        _JP(!ctx->F.Z);
        break;
    
    // JP Z,u16
    case 0xCA:
        _JP(ctx->F.Z);
        break;
    
    // JP NC,u16
    case 0xD2:
        _JP(!ctx->F.C);
        break;
    
    // JP C,u16
    case 0xDA:
        _JP(ctx->F.C);
        break;
    
    #define _JR(COND)                                                         \
        s8 = (int8_t)GBx_NextByte(ctx);                                       \
        cycles += 1;                                                          \
        if (COND) {                                                           \
            ctx->PC += s8;                                                    \
            cycles += 1;                                                      \
        }

    // JR s8
    case 0x18:
        _JR(true);
        break;
    
    // JR NZ,s8
    case 0x20:
        _JR(!ctx->F.Z);
        break;
    
    // JR Z,s8
    case 0x28:
        _JR(ctx->F.Z);
        break;
    
    // JR NC,s8
    case 0x30:
        _JR(!ctx->F.C);
        break;
    
    // JR C,s8
    case 0x38:
        _JR(ctx->F.C);
        break;

    #define _PUSH(SRC)                                                         \
        GBx_PushWord(ctx, (SRC));                                              \
        cycles += 3
    
    // PUSH AF
    case 0xF5:
        _PUSH(ctx->AF & 0xFFF0);
        break;
    
    // PUSH BC
    case 0xC5:
        _PUSH(ctx->BC);
        break;
    
    // PUSH DE
    case 0xD5:
        _PUSH(ctx->DE);
        break;
    
    // PUSH HL
    case 0xE5:
        _PUSH(ctx->HL);
        break;

    #define _POP(DST)                                                          \
        DST = GBx_PopWord(ctx);                                                \
        cycles += 2

    // POP AF
    case 0xF1:
        _POP(ctx->AF);
        break;

    // POP BC
    case 0xC1:
        _POP(ctx->BC);
        break;

    // POP DE
    case 0xD1:
        _POP(ctx->DE);
        break;

    // POP HL
    case 0xE1:
        _POP(ctx->HL);
        break;

    // LD A,B
    case 0x78:
        ctx->A = ctx->B;
        break;
        
    // LD A,C
    case 0x79:
        ctx->A = ctx->C;
        break;
        
    // LD A,D
    case 0x7A:
        ctx->A = ctx->D;
        break;
        
    // LD A,E
    case 0x7B:
        ctx->A = ctx->E;
        break;
        
    // LD A,H
    case 0x7C:
        ctx->A = ctx->H;
        break;
        
    // LD A,L
    case 0x7D:
        ctx->A = ctx->L;
        break;
        
    // LD A,(HL)
    case 0x7E:
        ctx->A = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        break;
        
    // LD A,A
    case 0x7F:
        ctx->A = ctx->A;
        break;
        
    // LD A,u8
    case 0x3E:
        ctx->A = GBx_NextByte(ctx);
        cycles += 1;
        break;
        
    // LD B,B
    case 0x40:
        ctx->B = ctx->B;
        break;
        
    // LD B,C
    case 0x41:
        ctx->B = ctx->C;
        break;
        
    // LD B,D
    case 0x42:
        ctx->B = ctx->D;
        break;
        
    // LD B,E
    case 0x43:
        ctx->B = ctx->E;
        break;
        
    // LD B,H
    case 0x44:
        ctx->B = ctx->H;
        break;
        
    // LD B,L
    case 0x45:
        ctx->B = ctx->L;
        break;
        
    // LD B,(HL)
    case 0x46:
        ctx->B = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        break;
        
    // LD B,A
    case 0x47:
        ctx->B = ctx->A;
        break;
        
    // LD B,u8
    case 0x06:
        ctx->B = GBx_NextByte(ctx);
        cycles += 1;
        break;
        
    // LD C,B
    case 0x48:
        ctx->C = ctx->B;
        break;
        
    // LD C,C
    case 0x49:
        ctx->C = ctx->C;
        break;
        
    // LD C,D
    case 0x4A:
        ctx->C = ctx->D;
        break;
        
    // LD C,E
    case 0x4B:
        ctx->C = ctx->E;
        break;
        
    // LD C,H
    case 0x4C:
        ctx->C = ctx->H;
        break;
        
    // LD C,L
    case 0x4D:
        ctx->C = ctx->L;
        break;
        
    // LD C,(HL)
    case 0x4E:
        ctx->C = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        break;
        
    // LD C,A
    case 0x4F:
        ctx->C = ctx->A;
        break;
        
    // LD C,u8
    case 0x0E:
        ctx->C = GBx_NextByte(ctx);
        cycles += 1;
        break;
        
    // LD D,B
    case 0x50:
        ctx->D = ctx->B;
        break;
        
    // LD D,C
    case 0x51:
        ctx->D = ctx->C;
        break;
        
    // LD D,D
    case 0x52:
        ctx->D = ctx->D;
        break;
        
    // LD D,E
    case 0x53:
        ctx->D = ctx->E;
        break;
        
    // LD D,H
    case 0x54:
        ctx->D = ctx->H;
        break;
        
    // LD D,L
    case 0x55:
        ctx->D = ctx->L;
        break;
        
    // LD D,(HL)
    case 0x56:
        ctx->D = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        break;
        
    // LD D,A
    case 0x57:
        ctx->D = ctx->A;
        break;
        
    // LD D,u8
    case 0x16:
        ctx->D = GBx_NextByte(ctx);
        cycles += 1;
        break;
        
    // LD E,B
    case 0x58:
        ctx->E = ctx->B;
        break;
        
    // LD E,C
    case 0x59:
        ctx->E = ctx->C;
        break;
        
    // LD E,D
    case 0x5A:
        ctx->E = ctx->D;
        break;
        
    // LD E,E
    case 0x5B:
        ctx->E = ctx->E;
        break;
        
    // LD E,H
    case 0x5C:
        ctx->E = ctx->H;
        break;
        
    // LD E,L
    case 0x5D:
        ctx->E = ctx->L;
        break;
        
    // LD E,(HL)
    case 0x5E:
        ctx->E = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        break;
        
    // LD E,A
    case 0x5F:
        ctx->E = ctx->A;
        break;
        
    // LD E,u8
    case 0x1E:
        ctx->E = GBx_NextByte(ctx);
        cycles += 1;
        break;
        
    // LD H,B
    case 0x60:
        ctx->H = ctx->B;
        break;
        
    // LD H,C
    case 0x61:
        ctx->H = ctx->C;
        break;
        
    // LD H,D
    case 0x62:
        ctx->H = ctx->D;
        break;
        
    // LD H,E
    case 0x63:
        ctx->H = ctx->E;
        break;
        
    // LD H,H
    case 0x64:
        ctx->H = ctx->H;
        break;
        
    // LD H,L
    case 0x65:
        ctx->H = ctx->L;
        break;
        
    // LD H,(HL)
    case 0x66:
        ctx->H = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        break;
        
    // LD H,A
    case 0x67:
        ctx->H = ctx->A;
        break;
        
    // LD H,u8
    case 0x26:
        ctx->H = GBx_NextByte(ctx);
        cycles += 1;
        break;
        
    // LD L,B
    case 0x68:
        ctx->L = ctx->B;
        break;
        
    // LD L,C
    case 0x69:
        ctx->L = ctx->C;
        break;
        
    // LD L,D
    case 0x6A:
        ctx->L = ctx->D;
        break;
        
    // LD L,E
    case 0x6B:
        ctx->L = ctx->E;
        break;
        
    // LD L,H
    case 0x6C:
        ctx->L = ctx->H;
        break;
        
    // LD L,L
    case 0x6D:
        ctx->L = ctx->L;
        break;
        
    // LD L,(HL)
    case 0x6E:
        ctx->L = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        break;
        
    // LD L,A
    case 0x6F:
        ctx->L = ctx->A;
        break;
        
    // LD L,u8
    case 0x2E:
        ctx->L = GBx_NextByte(ctx);
        cycles += 1;
        break;
    
    // LD (HL),B
    case 0x70:
        GBx_WriteByte(ctx, ctx->HL, ctx->B);
        cycles += 1;
        break;

    // LD (HL),C
    case 0x71:
        GBx_WriteByte(ctx, ctx->HL, ctx->C);
        cycles += 1;
        break;

    // LD (HL),D
    case 0x72:
        GBx_WriteByte(ctx, ctx->HL, ctx->D);
        cycles += 1;
        break;

    // LD (HL),E
    case 0x73:
        GBx_WriteByte(ctx, ctx->HL, ctx->E);
        cycles += 1;
        break;

    // LD (HL),H
    case 0x74:
        GBx_WriteByte(ctx, ctx->HL, ctx->H);
        cycles += 1;
        break;

    // LD (HL),L
    case 0x75:
        GBx_WriteByte(ctx, ctx->HL, ctx->L);
        cycles += 1;
        break;

    // LD (HL),u8
    case 0x36:
        GBx_WriteByte(ctx, ctx->HL, GBx_NextByte(ctx));
        cycles += 2;
        break;

    // LD (HL),A
    case 0x77:
        GBx_WriteByte(ctx, ctx->HL, ctx->A);
        cycles += 1;
        break;

    // LD (BC),A
    case 0x02:
        GBx_WriteByte(ctx, ctx->BC, ctx->A);
        cycles += 1;
        break;

    // LD (DE),A
    case 0x12:
        GBx_WriteByte(ctx, ctx->DE, ctx->A);
        cycles += 1;
        break;

    // LD A,(BC)
    case 0x0A:
        ctx->A = GBx_ReadByte(ctx, ctx->BC);
        cycles += 1;
        break;

    // LD A,(DE)
    case 0x1A:
        ctx->A = GBx_ReadByte(ctx, ctx->DE);
        cycles += 1;
        break;

    // LD BC,u16
    case 0x01:
        ctx->BC = GBx_NextWord(ctx);
        cycles += 2;
        break;

    // LD DE,u16
    case 0x11:
        ctx->DE = GBx_NextWord(ctx);
        cycles += 2;
        break;

    // LD HL,u16
    case 0x21:
        ctx->HL = GBx_NextWord(ctx);
        cycles += 2;
        break;

    // LD SP,u16
    case 0x31:
        ctx->SP = GBx_NextWord(ctx);
        cycles += 2;
        break;

    // LD SP,HL
    case 0xF9:
        ctx->SP = ctx->HL;
        break;

    // LD (u16),SP
    case 0x08:
        GBx_WriteWord(ctx, GBx_NextWord(ctx), ctx->SP);
        cycles += 4;
        break;

    // LD (u16),A
    case 0xEA:
        GBx_WriteByte(ctx, GBx_NextWord(ctx), ctx->A);
        cycles += 4;
        break;

    // LD A,(u16)
    case 0xFA:
        ctx->A = GBx_ReadByte(ctx, GBx_NextWord(ctx));
        cycles += 3;
        break;

    // LD (HL+),A
    // LD (HLI),A
    // LDI (HL),A
    case 0x22:
        GBx_WriteByte(ctx, ctx->HL, ctx->A);
        cycles += 1;
        ++ctx->HL;
        break;

    // LD (HL-),A
    // LD (HLD),A
    // LDD (HL),A
    case 0x32:
        GBx_WriteByte(ctx, ctx->HL, ctx->A);
        cycles += 1;
        --ctx->HL;
        break;

    // LD A,(HL+)
    // LD A,(HLI)
    // LDI A,(HL)
    case 0x2A:
        ctx->A = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        ++ctx->HL;
        break;

    // LD A,(HL-)
    // LD A,(HLD)
    // LDD A,(HL)
    case 0x3A:
        ctx->A = GBx_ReadByte(ctx, ctx->HL);
        cycles += 1;
        --ctx->HL;
        break;

    // LD ($FF00+u8),A
    // LDH (u8),A
    case 0xE0:
        GBx_WriteByte(ctx, 0xFF00 + GBx_NextByte(ctx), ctx->A);
        cycles += 2;
        break;

    // LD A,($FF00+u8)
    // LDH A,(u8)
    case 0xF0:
        ctx->A = GBx_ReadByte(ctx, 0xFF00 + GBx_NextByte(ctx));
        cycles += 2;
        break;

    // LD ($FF00+C),A
    // LDH (C),A
    case 0xE2:
        GBx_WriteByte(ctx, 0xFF00 + ctx->C, ctx->A);
        cycles += 1;
        break;

    // LD A,($FF00+C)
    // LDH A,(C)
    case 0xF2:
        ctx->A = GBx_ReadByte(ctx, 0xFF00 + ctx->C);
        cycles += 1;
        break;

    // LD HL,SP+s8
    case 0xF8:
        s8 = (int8_t)GBx_NextByte(ctx);
        u32 = ctx->SP + s8;
        bits = (ctx->SP ^ s8 ^ (u32 & 0xFFFF));
        ctx->F.N = false;
        ctx->F.Z = false;
        ctx->F.C = ((bits & 0x100) == 0x100);
        ctx->F.H = ((bits & 0x10) == 0x10);
        ctx->HL = (uint16_t)u32;
        cycles += 1;
        break;

    #define _ADD(X)                                                           \
        u8 = (X);                                                             \
        u16 = ctx->A + u8;                                                    \
        bits = ctx->A ^ u8 ^ u16;                                             \
        ctx->F.N = false;                                                     \
        ctx->F.Z = ((u16 & 0xFF) == 0);                                       \
        ctx->F.H = ((bits & 0x10) != 0);                                      \
        ctx->F.C = ((bits & 0x100) != 0);                                     \
        ctx->A = (uint8_t)u16

    // ADD B
    case 0x80:
        _ADD(ctx->B);
        break;

    // ADD C
    case 0x81:
        _ADD(ctx->C);
        break;

    // ADD D
    case 0x82:
        _ADD(ctx->D);
        break;

    // ADD E
    case 0x83:
        _ADD(ctx->E);
        break;

    // ADD H
    case 0x84:
        _ADD(ctx->H);
        break;

    // ADD L
    case 0x85:
        _ADD(ctx->L);
        break;

    // ADD (HL)
    case 0x86:
        _ADD(GBx_ReadByte(ctx, ctx->HL));
        cycles += 1;
        break;

    // ADD A
    case 0x87:
        _ADD(ctx->A);
        break;

    // ADD u8
    case 0xC6:
        _ADD(GBx_NextByte(ctx));
        cycles += 1;
        break;

    #define _ADC(X)                                                           \
        u8 = (X) + ctx->F.C;                                                  \
        u16 = ctx->A + u8;                                                    \
        ctx->F.N = false;                                                     \
        ctx->F.H = ((((ctx->A & 0x0F) + (u8 & 0x0F)) + ctx->F.C) > 0x0F);     \
        ctx->F.C = (u16 > 0xFF);                                              \
        ctx->A = (uint8_t)u16;                                                \
        ctx->F.Z = (ctx->A == 0)

    // ADC B
    case 0x88:
        _ADC(ctx->B);
        break;

    // ADC C
    case 0x89:
        _ADC(ctx->C);
        break;

    // ADC D
    case 0x8A:
        _ADC(ctx->D);
        break;

    // ADC E
    case 0x8B:
        _ADC(ctx->E);
        break;

    // ADC H
    case 0x8C:
        _ADC(ctx->H);
        break;

    // ADC L
    case 0x8D:
        _ADC(ctx->L);
        break;

    // ADC (HL)
    case 0x8E:
        _ADC(GBx_ReadByte(ctx, ctx->HL));
        cycles += 1;
        break;

    // ADC A
    case 0x8F:
        _ADC(ctx->A);
        break;

    // ADC u8
    case 0xCE:
        _ADC(GBx_NextByte(ctx));
        cycles += 1;
        break;

    #define _ADD_HL(X)                                                        \
        u16 = (X);                                                            \
        u32 = ctx->HL + u16;                                                  \
        ctx->F.N = false;                                                     \
        ctx->F.H = (((ctx->HL ^ u16 ^ (u32 & 0xFFFF)) & 0x1000) == 0x1000);   \
        ctx->F.C = ((u32 & 0xFFFF0000) > 0);                                  \
        ctx->HL = (uint16_t)u32

    // ADD HL,BC
    case 0x09:
        _ADD_HL(ctx->BC);
        break;

    // ADD HL,DE
    case 0x19:
        _ADD_HL(ctx->DE);
        break;

    // ADD HL,HL
    case 0x29:
        _ADD_HL(ctx->HL);
        break;

    // ADD HL,SP
    case 0x39:
        _ADD_HL(ctx->SP);
        break;

    // ADD SP,s8
    case 0xE8:
        s8 = (int8_t)GBx_NextByte(ctx);
        u32 = ctx->SP + s8;
        bits = (ctx->SP ^ s8 ^ (u32 & 0xFFFF));
        ctx->F.N = false;
        ctx->F.Z = false;
        ctx->F.C = ((bits & 0x100) == 0x100);
        ctx->F.H = ((bits & 0x10) == 0x10);
        ctx->SP = (uint16_t)u32;
        cycles += 3;
        break;

    #define _CP(X)                                                            \
        u8 = (X);                                                             \
        u16 = ctx->A - u8;                                                    \
        ctx->F.N = true;                                                      \
        ctx->F.H = ((u8 & 0x0F) > (ctx->A & 0x0F));                           \
        ctx->F.C = ((u16 & 0xFF00) > 0);                                      \
        ctx->F.Z = ((u16 & 0xFF) == 0)
    
    // CP B
    case 0xB8:
        _CP(ctx->B);
        break;

    // CP C
    case 0xB9:
        _CP(ctx->C);
        break;

    // CP D
    case 0xBA:
        _CP(ctx->D);
        break;

    // CP E
    case 0xBB:
        _CP(ctx->E);
        break;

    // CP H
    case 0xBC:
        _CP(ctx->H);
        break;

    // CP L
    case 0xBD:
        _CP(ctx->L);
        break;

    // CP (HL)
    case 0xBE:
        _CP(GBx_ReadByte(ctx, ctx->HL));
        cycles += 1;
        break;

    // CP A
    case 0xBF:
        _CP(ctx->A);
        break;

    // CP u8
    case 0xFE:
        _CP(GBx_NextByte(ctx));
        cycles += 1;
        break;

    #define _SUB(X)                                                           \
        _CP(X);                                                               \
        ctx->A = (uint8_t)u16

    // SUB B
    case 0x90:
        _SUB(ctx->B);
        break;

    // SUB C
    case 0x91:
        _SUB(ctx->C);
        break;

    // SUB D
    case 0x92:
        _SUB(ctx->D);
        break;

    // SUB E
    case 0x93:
        _SUB(ctx->E);
        break;

    // SUB H
    case 0x94:
        _SUB(ctx->H);
        break;

    // SUB L
    case 0x95:
        _SUB(ctx->L);
        break;

    // SUB (HL)
    case 0x96:
        _SUB(GBx_ReadByte(ctx, ctx->HL));
        cycles += 1;
        break;

    // SUB A
    case 0x97:
        _SUB(ctx->A);
        break;

    // SUB u8
    case 0xD6:
        _SUB(GBx_NextByte(ctx));
        cycles += 1;
        break;

    #define _SBC(X)                                                           \
        _SUB((X) + (uint8_t)ctx->F.C)

    // SBC B
    case 0x98:
        _SBC(ctx->B);
        break;

    // SBC C
    case 0x99:
        _SBC(ctx->C);
        break;

    // SBC D
    case 0x9A:
        _SBC(ctx->D);
        break;

    // SBC E
    case 0x9B:
        _SBC(ctx->E);
        break;

    // SBC H
    case 0x9C:
        _SBC(ctx->H);
        break;

    // SBC L
    case 0x9D:
        _SBC(ctx->L);
        break;

    // SBC (HL)
    case 0x9E:
        _SBC(GBx_ReadByte(ctx, ctx->HL));
        cycles += 1;
        break;

    // SBC A
    case 0x9F:
        _SBC(ctx->A);
        break;

    // SBC_u8
    case 0xDE:
        _SBC(GBx_NextByte(ctx));
        cycles += 1;
        break;

    #define _INC(VAR)                                                         \
        ctx->F.H = (((VAR) & 0x0F) == 0x0F);                                  \
        ++(VAR);                                                              \
        ctx->F.Z = ((VAR) == 0);                                              \
        ctx->F.N = false

    // INC B
    case 0x04:
        _INC(ctx->B);
        break;

    // INC C
    case 0x0C:
        _INC(ctx->C);
        break;

    // INC D
    case 0x14:
        _INC(ctx->D);
        break;

    // INC E
    case 0x1C:
        _INC(ctx->E);
        break;

    // INC H
    case 0x24:
        _INC(ctx->H);
        break;

    // INC L
    case 0x2C:
        _INC(ctx->L);
        break;

    // INC (HL)
    case 0x34:
        u8 = GBx_ReadByte(ctx, ctx->HL);
        _INC(u8);
        GBx_WriteByte(ctx, ctx->HL, u8);
        cycles += 2;
        break;

    // INC A
    case 0x3C:
        _INC(ctx->A);
        break;

    // INC BC
    case 0x03:
        cycles += 1;
        ++ctx->BC;
        break;

    // INC DE
    case 0x13:
        cycles += 1;
        ++ctx->DE;
        break;

    // INC HL
    case 0x23:
        cycles += 1;
        ++ctx->HL;
        break;

    // INC SP
    case 0x33:
        cycles += 1;
        ++ctx->SP;
        break;

    #define _DEC(VAR)                                                         \
        ctx->F.H = (((VAR) & 0x0F) == 0);                                     \
        --(VAR);                                                              \
        ctx->F.Z = ((VAR) == 0);                                              \
        ctx->F.N = true

    // DEC B
    case 0x05:
        _DEC(ctx->B);
        break;

    // DEC C
    case 0x0D:
        _DEC(ctx->C);
        break;

    // DEC D
    case 0x15:
        _DEC(ctx->D);
        break;

    // DEC E
    case 0x1D:
        _DEC(ctx->E);
        break;

    // DEC H
    case 0x25:
        _DEC(ctx->H);
        break;

    // DEC L
    case 0x2D:
        _DEC(ctx->L);
        break;

    // DEC (HL)
    case 0x35:
        u8 = GBx_ReadByte(ctx, ctx->HL);
        _DEC(u8);
        GBx_WriteByte(ctx, ctx->HL, u8);
        cycles += 2;
        break;

    // DEC A
    case 0x3D:
        _DEC(ctx->A);
        break;

    // DEC BC
    case 0x0B:
        --ctx->BC;
        break;

    // DEC DE
    case 0x1B:
        --ctx->DE;
        break;

    // DEC HL
    case 0x2B:
        --ctx->HL;
        break;

    // DEC SP
    case 0x3B:
        --ctx->SP;
        break;

    #define _AND(X)                                                           \
        ctx->A &= (X);                                                        \
        ctx->F.Z = (ctx->A == 0);                                             \
        ctx->F.N = false;                                                     \
        ctx->F.H = true;                                                      \
        ctx->F.C = false

    // AND B
    case 0xA0:
        _AND(ctx->B);
        break;

    // AND C
    case 0xA1:
        _AND(ctx->C);
        break;

    // AND D
    case 0xA2:
        _AND(ctx->D);
        break;

    // AND E
    case 0xA3:
        _AND(ctx->E);
        break;

    // AND H
    case 0xA4:
        _AND(ctx->H);
        break;

    // AND L
    case 0xA5:
        _AND(ctx->L);
        break;

    // AND (HL)
    case 0xA6:
        _AND(GBx_ReadByte(ctx, ctx->HL));
        cycles += 1;
        break;

    // AND A
    case 0xA7:
        _AND(ctx->A);
        break;

    // AND u8
    case 0xE6:
        _AND(GBx_NextByte(ctx));
        cycles += 1;
        break;

    #define _OR(X)                                                            \
        ctx->A |= (X);                                                        \
        ctx->F.Z = (ctx->A == 0);                                             \
        ctx->F.N = false;                                                     \
        ctx->F.H = false;                                                     \
        ctx->F.C = false

    // OR B
    case 0xB0:
        _OR(ctx->B);
        break;

    // OR C
    case 0xB1:
        _OR(ctx->C);
        break;

    // OR D
    case 0xB2:
        _OR(ctx->D);
        break;

    // OR E
    case 0xB3:
        _OR(ctx->E);
        break;

    // OR H
    case 0xB4:
        _OR(ctx->H);
        break;

    // OR L
    case 0xB5:
        _OR(ctx->L);
        break;

    // OR (HL)
    case 0xB6:
        _OR(GBx_ReadByte(ctx, ctx->HL));
        cycles += 1;
        break;

    // OR A
    case 0xB7:
        _OR(ctx->A);
        break;

    // OR u8
    case 0xF6:
        _OR(GBx_NextByte(ctx));
        cycles += 1;
        break;

    #define _XOR(X)                                                           \
        ctx->A ^= (X);                                                        \
        ctx->F.Z = (ctx->A == 0);                                             \
        ctx->F.N = false;                                                     \
        ctx->F.H = false;                                                     \
        ctx->F.C = false

    // XOR B
    case 0xA8:
        _XOR(ctx->B);
        break;

    // XOR C
    case 0xA9:
        _XOR(ctx->C);
        break;

    // XOR D
    case 0xAA:
        _XOR(ctx->D);
        break;

    // XOR E
    case 0xAB:
        _XOR(ctx->E);
        break;

    // XOR H
    case 0xAC:
        _XOR(ctx->H);
        break;

    // XOR L
    case 0xAD:
        _XOR(ctx->L);
        break;

    // XOR (HL)
    case 0xAE:
        _XOR(GBx_ReadByte(ctx, ctx->HL));
        cycles += 1;
        break;

    // XOR A
    case 0xAF:
        _XOR(ctx->A);
        break;

    // XOR u8
    case 0xEE:
        _XOR(GBx_NextByte(ctx));
        cycles += 1;
        break;

    #define _RLC(X)                                                           \
        ctx->F.C = (((X) & 0x80) == 0x80);                                    \
        (X) = ((X) << 1) | ctx->F.C;                                          \
        ctx->F.Z = ((X) == 0);                                                \
        ctx->F.N = false;                                                     \
        ctx->F.H = false

    // RLCA
    case 0x07:
        _RLC(ctx->A);
        break;

    #define _RRC(X)                                                           \
        ctx->F.C = (((X) & 0x01) == 0x01);                                    \
        (X) = ((X) >> 1) | (ctx->F.C << 7);                                   \
        ctx->F.Z = ((X) == 0);                                                \
        ctx->F.N = false;                                                     \
        ctx->F.H = false

    // RRCA
    case 0x0F:
        _RRC(ctx->A);
        break;

    #define _RL(X)                                                            \
        u8 = (((X) & 0x80) == 0x80);                                          \
        (X) = ((X) << 1) | (uint8_t)ctx->F.C;                                 \
        ctx->F.C = u8;                                                        \
        ctx->F.Z = ((X) == 0);                                                \
        ctx->F.N = false;                                                     \
        ctx->F.H = false

    // RLA
    case 0x17:
        _RL(ctx->A);
        break;

    #define _RR(X)                                                            \
        u8 = (((X) & 0x01) == 0x01);                                          \
        (X) = ((X) >> 1) | (ctx->F.C << 7);                                   \
        ctx->F.C = u8;                                                        \
        ctx->F.Z = ((X) == 0);                                                \
        ctx->F.N = false;                                                     \
        ctx->F.H = false

    // RRA
    case 0x1F:
        _RR(ctx->A);
        break;

    case 0xCB:

        op = GBx_NextByte(ctx);
        cycles += 1;

        switch (op) {

        // RLC B
        case 0x00:
            _RLC(ctx->B);
            break;

        // RLC C
        case 0x01:
            _RLC(ctx->C);
            break;

        // RLC D
        case 0x02:
            _RLC(ctx->D);
            break;

        // RLC E
        case 0x03:
            _RLC(ctx->E);
            break;

        // RLC H
        case 0x04:
            _RLC(ctx->H);
            break;

        // RLC L
        case 0x05:
            _RLC(ctx->L);
            break;

        // RLC (HL)
        case 0x06:
            u8 = GBx_ReadByte(ctx, ctx->HL);
            _RLC(u8);
            GBx_WriteByte(ctx, ctx->HL, u8);
            cycles += 2;
            break;

        // RLC A
        case 0x07:
            _RLC(ctx->A);
            break;

        // RRC B
        case 0x08:
            _RRC(ctx->B);
            break;

        // RRC C
        case 0x09:
            _RRC(ctx->C);
            break;

        // RRC D
        case 0x0A:
            _RRC(ctx->D);
            break;

        // RRC E
        case 0x0B:
            _RRC(ctx->E);
            break;

        // RRC H
        case 0x0C:
            _RRC(ctx->H);
            break;

        // RRC L
        case 0x0D:
            _RRC(ctx->L);
            break;

        // RRC (HL)
        case 0x0E:
            u8 = GBx_ReadByte(ctx, ctx->HL);
            _RRC(u8);
            GBx_WriteByte(ctx, ctx->HL, u8);
            cycles += 2;
            break;

        // RRC A
        case 0x0F:
            _RRC(ctx->A);
            break;

        // RL B
        case 0x10:
            _RL(ctx->B);
            break;

        // RL C
        case 0x11:
            _RL(ctx->C);
            break;

        // RL D
        case 0x12:
            _RL(ctx->D);
            break;

        // RL E
        case 0x13:
            _RL(ctx->E);
            break;

        // RL H
        case 0x14:
            _RL(ctx->H);
            break;

        // RL L
        case 0x15:
            _RL(ctx->L);
            break;

        // RL (HL)
        case 0x16:
            u8 = GBx_ReadByte(ctx, ctx->HL);
            _RL(u8);
            GBx_WriteByte(ctx, ctx->HL, u8);
            cycles += 2;
            break;

        // RL A
        case 0x17:
            _RL(ctx->A);
            break;

        // RR B
        case 0x18:
            _RR(ctx->B);
            break;

        // RR C
        case 0x19:
            _RR(ctx->C);
            break;

        // RR D
        case 0x1A:
            _RR(ctx->D);
            break;

        // RR E
        case 0x1B:
            _RR(ctx->E);
            break;

        // RR H
        case 0x1C:
            _RR(ctx->H);
            break;

        // RR L
        case 0x1D:
            _RR(ctx->L);
            break;

        // RR (HL)
        case 0x1E:
            u8 = GBx_ReadByte(ctx, ctx->HL);
            _RR(u8);
            GBx_WriteByte(ctx, ctx->HL, u8);
            cycles += 2;
            break;

        // RR A
        case 0x1F:
            _RR(ctx->A);
            break;

        #define _SLA(X)                                                       \
            ctx->F.C = ((X)&0x80) == 0x80;                                    \
            (X) <<= 1;                                                        \
            ctx->F.Z = ((X) == 0);                                            \
            ctx->F.N = false;                                                 \
            ctx->F.H = false

        // SLA B
        case 0x20:
            _SLA(ctx->B);
            break;

        // SLA C
        case 0x21:
            _SLA(ctx->C);
            break;

        // SLA D
        case 0x22:
            _SLA(ctx->D);
            break;

        // SLA E
        case 0x23:
            _SLA(ctx->E);
            break;

        // SLA H
        case 0x24:
            _SLA(ctx->H);
            break;

        // SLA L
        case 0x25:
            _SLA(ctx->L);
            break;

        // SLA (HL)
        case 0x26:
            u8 = GBx_ReadByte(ctx, ctx->HL);
            _SLA(u8);
            GBx_WriteByte(ctx, ctx->HL, u8);
            cycles += 2;
            break;

        // SLA A
        case 0x27:
            _SLA(ctx->A);
            break;

        #define _SRA(X)                                                       \
            ctx->F.C = ((X)&0x01) == 0x01;                                    \
            (X) = ((X) >> 1) | ((X)&0x80);                                    \
            ctx->F.Z = ((X) == 0);                                            \
            ctx->F.N = false;                                                 \
            ctx->F.H = false
        
        // SRA B
        case 0x28:
            _SRA(ctx->B);
            break;

        // SRA C
        case 0x29:
            _SRA(ctx->C);
            break;

        // SRA D
        case 0x2A:
            _SRA(ctx->D);
            break;

        // SRA E
        case 0x2B:
            _SRA(ctx->E);
            break;

        // SRA H
        case 0x2C:
            _SRA(ctx->H);
            break;

        // SRA L
        case 0x2D:
            _SRA(ctx->L);
            break;

        // SRA (HL)
        case 0x2E:
            u8 = GBx_ReadByte(ctx, ctx->HL);
            _SRA(u8);
            GBx_WriteByte(ctx, ctx->HL, u8);
            cycles += 2;
            break;

        // SRA A
        case 0x2F:
            _SRA(ctx->A);
            break;

        #define _SRL(X)                                                       \
            ctx->F.C = ((X)&0x01) == 0x01;                                    \
            (X) >>= 1;                                                        \
            ctx->F.Z = ((X) == 0);                                            \
            ctx->F.N = false;                                                 \
            ctx->F.H = false

        // SRL B
        case 0x38:
            _SRL(ctx->B);
            break;

        // SRL C
        case 0x39:
            _SRL(ctx->C);
            break;

        // SRL D
        case 0x3A:
            _SRL(ctx->D);
            break;

        // SRL E
        case 0x3B:
            _SRL(ctx->E);
            break;

        // SRL H
        case 0x3C:
            _SRL(ctx->H);
            break;

        // SRL L
        case 0x3D:
            _SRL(ctx->L);
            break;

        // SRL (HL)
        case 0x3E:
            u8 = GBx_ReadByte(ctx, ctx->HL);
            _SRL(u8);
            GBx_WriteByte(ctx, ctx->HL, u8);
            cycles += 2;
            break;

        // SRL A
        case 0x3F:
            _SRL(ctx->A);
            break;

        #define _SWAP(X)                                                      \
            (X) = (((X)&0x0F) << 4) | (((X)&0xF0) >> 4);                      \
            ctx->F.Z = ((X) == 0);                                            \
            ctx->F.N = false;                                                 \
            ctx->F.H = false;                                                 \
            ctx->F.C = false

        // SWAP B
        case 0x30:
            _SWAP(ctx->B);
            break;

        // SWAP C
        case 0x31:
            _SWAP(ctx->C);
            break;

        // SWAP D
        case 0x32:
            _SWAP(ctx->D);
            break;

        // SWAP E
        case 0x33:
            _SWAP(ctx->E);
            break;

        // SWAP H
        case 0x34:
            _SWAP(ctx->H);
            break;

        // SWAP L
        case 0x35:
            _SWAP(ctx->L);
            break;

        // SWAP (HL)
        case 0x36:
            u8 = GBx_ReadByte(ctx, ctx->HL);
            _SWAP(u8);
            GBx_WriteByte(ctx, ctx->HL, u8);
            cycles += 2;
            break;

        // SWAP A
        case 0x37:
            _SWAP(ctx->A);
            break;

        #define _BIT(NUM, X)                                                  \
            ctx->F.Z = (((X) & (1 << NUM)) == 0);                             \
            ctx->F.N = false;                                                 \
            ctx->F.H = true

        #define _RES(NUM, X)                                                  \
            X &= ~(1 << NUM)

        #define _BIT_GROUP(NUM)                                               \
            case (0x40 + ((NUM) * 8) + 0):                                    \
                _BIT((NUM), ctx->B);                                          \
                break;                                                        \
            case (0x40 + ((NUM) * 8) + 1):                                    \
                _BIT((NUM), ctx->C);                                          \
                break;                                                        \
            case (0x40 + ((NUM) * 8) + 2):                                    \
                _BIT((NUM), ctx->D);                                          \
                break;                                                        \
            case (0x40 + ((NUM) * 8) + 3):                                    \
                _BIT((NUM), ctx->E);                                          \
                break;                                                        \
            case (0x40 + ((NUM) * 8) + 4):                                    \
                _BIT((NUM), ctx->H);                                          \
                break;                                                        \
            case (0x40 + ((NUM) * 8) + 5):                                    \
                _BIT((NUM), ctx->L);                                          \
                break;                                                        \
            case (0x40 + ((NUM) * 8) + 6):                                    \
                _BIT((NUM), GBx_ReadByte(ctx, ctx->HL));                      \
                cycles += 1;                                                  \
                break;                                                        \
            case (0x40 + ((NUM) * 8) + 7):                                    \
                _BIT((NUM), ctx->A);                                          \
                break;

        _BIT_GROUP(0)
        _BIT_GROUP(1)
        _BIT_GROUP(2)
        _BIT_GROUP(3)
        _BIT_GROUP(4)
        _BIT_GROUP(5)
        _BIT_GROUP(6)
        _BIT_GROUP(7)

        #define _RES_GROUP(NUM)                                               \
            case (0x80 + ((NUM) * 8) + 0):                                    \
                _RES((NUM), ctx->B);                                          \
                break;                                                        \
            case (0x80 + ((NUM) * 8) + 1):                                    \
                _RES((NUM), ctx->C);                                          \
                break;                                                        \
            case (0x80 + ((NUM) * 8) + 2):                                    \
                _RES((NUM), ctx->D);                                          \
                break;                                                        \
            case (0x80 + ((NUM) * 8) + 3):                                    \
                _RES((NUM), ctx->E);                                          \
                break;                                                        \
            case (0x80 + ((NUM) * 8) + 4):                                    \
                _RES((NUM), ctx->H);                                          \
                break;                                                        \
            case (0x80 + ((NUM) * 8) + 5):                                    \
                _RES((NUM), ctx->L);                                          \
                break;                                                        \
            case (0x80 + ((NUM) * 8) + 6):                                    \
                u8 = GBx_ReadByte(ctx, ctx->HL);                              \
                _RES((NUM), u8);                                              \
                GBx_WriteByte(ctx, ctx->HL, u8);                              \
                cycles += 2;                                                  \
                break;                                                        \
            case (0x80 + ((NUM) * 8) + 7):                                    \
                _RES((NUM), ctx->A);                                          \
                break;

        _RES_GROUP(0)
        _RES_GROUP(1)
        _RES_GROUP(2)
        _RES_GROUP(3)
        _RES_GROUP(4)
        _RES_GROUP(5)
        _RES_GROUP(6)
        _RES_GROUP(7)

        #define _SET(N, X)                                                    \
            X |= (1 << N)

        #define _SET_GROUP(NUM)                                               \
            case (0xC0 + ((NUM) * 8) + 0):                                    \
                _SET((NUM), ctx->B);                                          \
                break;                                                        \
            case (0xC0 + ((NUM) * 8) + 1):                                    \
                _SET((NUM), ctx->C);                                          \
                break;                                                        \
            case (0xC0 + ((NUM) * 8) + 2):                                    \
                _SET((NUM), ctx->D);                                          \
                break;                                                        \
            case (0xC0 + ((NUM) * 8) + 3):                                    \
                _SET((NUM), ctx->E);                                          \
                break;                                                        \
            case (0xC0 + ((NUM) * 8) + 4):                                    \
                _SET((NUM), ctx->H);                                          \
                break;                                                        \
            case (0xC0 + ((NUM) * 8) + 5):                                    \
                _SET((NUM), ctx->L);                                          \
                break;                                                        \
            case (0xC0 + ((NUM) * 8) + 6):                                    \
                u8 = GBx_ReadByte(ctx, ctx->HL);                              \
                _SET((NUM), u8);                                              \
                GBx_WriteByte(ctx, ctx->HL, u8);                              \
                cycles += 2;                                                  \
                break;                                                        \
            case (0xC0 + ((NUM) * 8) + 7):                                    \
                _SET((NUM), ctx->A);                                          \
                break;

        _SET_GROUP(0)
        _SET_GROUP(1)
        _SET_GROUP(2)
        _SET_GROUP(3)
        _SET_GROUP(4)
        _SET_GROUP(5)
        _SET_GROUP(6)
        _SET_GROUP(7)

        }

        break;
    default:
        // Error
        break;
    }

    if (ctx->RequestEnableIME) {
        ctx->IME = true;
        ctx->RequestEnableIME = false;
    }

    return cycles;
}