#include <SM83/CPU.h>
#include <SM83/Context.h>
#include <SM83/Debug.h>
#include <SM83/Disassembler.h>
#include <SM83/Interrupts.h>

#include "Instructions.h"
#include "Internal.h"
#include "Log.h"

uint8_t SM83_Fetch(sm83_t * ctx)
{
    ctx->internal->LastInstructionAddress = ctx->PC;

    uint8_t op = SM83_NextByte(ctx);
    return op;
}

sm83_instruction_t SM83_Decode(sm83_t * ctx, uint8_t opcode)
{
    static const sm83_instruction_t instructions[] = {
        // Miscellenious
        [0x00] = SM83_INST_NOP,
        [0x76] = SM83_INST_HALT,
        [0x10] = SM83_INST_STOP,
        [0xF3] = SM83_INST_DI,
        [0xFB] = SM83_INST_EI,
        [0x27] = SM83_INST_DAA,
        [0x37] = SM83_INST_SCF,
        [0x2F] = SM83_INST_CPL,
        [0x3F] = SM83_INST_CCF,
        // Call
        [0xCD] = SM83_INST_CALL_u16,
        [0xC4] = SM83_INST_CALL_NZ_u16,
        [0xCC] = SM83_INST_CALL_Z_u16,
        [0xD4] = SM83_INST_CALL_NC_u16,
        [0xDC] = SM83_INST_CALL_C_u16,
        // Restart
        [0xC7] = SM83_INST_RST_00,
        [0xCF] = SM83_INST_RST_08,
        [0xD7] = SM83_INST_RST_10,
        [0xDF] = SM83_INST_RST_18,
        [0xE7] = SM83_INST_RST_20,
        [0xEF] = SM83_INST_RST_28,
        [0xF7] = SM83_INST_RST_30,
        [0xFF] = SM83_INST_RST_38,
        // Return
        [0xC9] = SM83_INST_RET,
        [0xC0] = SM83_INST_RET_NZ,
        [0xC8] = SM83_INST_RET_Z,
        [0xD0] = SM83_INST_RET_NC,
        [0xD8] = SM83_INST_RET_C,
        [0xD9] = SM83_INST_RETI,
        // Rotate
        [0x07] = SM83_INST_RLC_A,
        [0x0F] = SM83_INST_RRC_A,
        [0x17] = SM83_INST_RL_A,
        [0x1F] = SM83_INST_RR_A,
        // Jumps
        [0xE9] = SM83_INST_JP_HL,
        [0xC3] = SM83_INST_JP_u16,
        [0xC2] = SM83_INST_JP_NZ_u16,
        [0xCA] = SM83_INST_JP_Z_u16,
        [0xD2] = SM83_INST_JP_NC_u16,
        [0xDA] = SM83_INST_JP_C_u16,
        [0x18] = SM83_INST_JR_s8,
        [0x20] = SM83_INST_JR_NZ_s8,
        [0x28] = SM83_INST_JR_Z_s8,
        [0x30] = SM83_INST_JR_NC_s8,
        [0x38] = SM83_INST_JR_C_s8,
        // Push
        [0xF5] = SM83_INST_PUSH_AF,
        [0xC5] = SM83_INST_PUSH_BC,
        [0xD5] = SM83_INST_PUSH_DE,
        [0xE5] = SM83_INST_PUSH_HL,
        // Pop
        [0xF1] = SM83_INST_POP_AF,
        [0xC1] = SM83_INST_POP_BC,
        [0xD1] = SM83_INST_POP_DE,
        [0xE1] = SM83_INST_POP_HL,
        // Load
        [0x78] = SM83_INST_LD_A_B,
        [0x79] = SM83_INST_LD_A_C,
        [0x7A] = SM83_INST_LD_A_D,
        [0x7B] = SM83_INST_LD_A_E,
        [0x7C] = SM83_INST_LD_A_H,
        [0x7D] = SM83_INST_LD_A_L,
        [0x7E] = SM83_INST_LD_A_pHL,
        [0x7F] = SM83_INST_LD_A_A,
        [0x3E] = SM83_INST_LD_A_u8,
        [0x40] = SM83_INST_LD_B_B,
        [0x41] = SM83_INST_LD_B_C,
        [0x42] = SM83_INST_LD_B_D,
        [0x43] = SM83_INST_LD_B_E,
        [0x44] = SM83_INST_LD_B_H,
        [0x45] = SM83_INST_LD_B_L,
        [0x46] = SM83_INST_LD_B_pHL,
        [0x47] = SM83_INST_LD_B_A,
        [0x06] = SM83_INST_LD_B_u8,
        [0x48] = SM83_INST_LD_C_B,
        [0x49] = SM83_INST_LD_C_C,
        [0x4A] = SM83_INST_LD_C_D,
        [0x4B] = SM83_INST_LD_C_E,
        [0x4C] = SM83_INST_LD_C_H,
        [0x4D] = SM83_INST_LD_C_L,
        [0x4E] = SM83_INST_LD_C_pHL,
        [0x4F] = SM83_INST_LD_C_A,
        [0x0E] = SM83_INST_LD_C_u8,
        [0x50] = SM83_INST_LD_D_B,
        [0x51] = SM83_INST_LD_D_C,
        [0x52] = SM83_INST_LD_D_D,
        [0x53] = SM83_INST_LD_D_E,
        [0x54] = SM83_INST_LD_D_H,
        [0x55] = SM83_INST_LD_D_L,
        [0x56] = SM83_INST_LD_D_pHL,
        [0x57] = SM83_INST_LD_D_A,
        [0x16] = SM83_INST_LD_D_u8,
        [0x58] = SM83_INST_LD_E_B,
        [0x59] = SM83_INST_LD_E_C,
        [0x5A] = SM83_INST_LD_E_D,
        [0x5B] = SM83_INST_LD_E_E,
        [0x5C] = SM83_INST_LD_E_H,
        [0x5D] = SM83_INST_LD_E_L,
        [0x5E] = SM83_INST_LD_E_pHL,
        [0x5F] = SM83_INST_LD_E_A,
        [0x1E] = SM83_INST_LD_E_u8,
        [0x60] = SM83_INST_LD_H_B,
        [0x61] = SM83_INST_LD_H_C,
        [0x62] = SM83_INST_LD_H_D,
        [0x63] = SM83_INST_LD_H_E,
        [0x64] = SM83_INST_LD_H_H,
        [0x65] = SM83_INST_LD_H_L,
        [0x66] = SM83_INST_LD_H_pHL,
        [0x67] = SM83_INST_LD_H_A,
        [0x26] = SM83_INST_LD_H_u8,
        [0x68] = SM83_INST_LD_L_B,
        [0x69] = SM83_INST_LD_L_C,
        [0x6A] = SM83_INST_LD_L_D,
        [0x6B] = SM83_INST_LD_L_E,
        [0x6C] = SM83_INST_LD_L_H,
        [0x6D] = SM83_INST_LD_L_L,
        [0x6E] = SM83_INST_LD_L_pHL,
        [0x6F] = SM83_INST_LD_L_A,
        [0x2E] = SM83_INST_LD_L_u8,
        [0x70] = SM83_INST_LD_pHL_B,
        [0x71] = SM83_INST_LD_pHL_C,
        [0x72] = SM83_INST_LD_pHL_D,
        [0x73] = SM83_INST_LD_pHL_E,
        [0x74] = SM83_INST_LD_pHL_H,
        [0x75] = SM83_INST_LD_pHL_L,
        [0x36] = SM83_INST_LD_pHL_u8,
        [0x77] = SM83_INST_LD_pHL_A,
        [0x02] = SM83_INST_LD_pBC_A,
        [0x12] = SM83_INST_LD_pDE_A,
        [0x0A] = SM83_INST_LD_A_pBC,
        [0x1A] = SM83_INST_LD_A_pDE,
        [0x01] = SM83_INST_LD_BC_u16,
        [0x11] = SM83_INST_LD_DE_u16,
        [0x21] = SM83_INST_LD_HL_u16,
        [0x31] = SM83_INST_LD_SP_u16,
        [0xF9] = SM83_INST_LD_SP_HL,
        [0x08] = SM83_INST_LD_pu16_SP,
        [0xEA] = SM83_INST_LD_pu16_A,
        [0xFA] = SM83_INST_LD_A_pu16,
        [0x22] = SM83_INST_LDI_pHL_A,
        [0x32] = SM83_INST_LDD_pHL_A,
        [0x2A] = SM83_INST_LDI_A_pHL,
        [0x3A] = SM83_INST_LDD_A_pHL,
        [0xE0] = SM83_INST_LDH_pu8_A,
        [0xF0] = SM83_INST_LDH_A_pu8,
        [0xE2] = SM83_INST_LDH_pC_A,
        [0xF2] = SM83_INST_LDH_A_pC,
        [0xF8] = SM83_INST_LD_HL_SP_s8,
        // Add
        [0x80] = SM83_INST_ADD_B,
        [0x81] = SM83_INST_ADD_C,
        [0x82] = SM83_INST_ADD_D,
        [0x83] = SM83_INST_ADD_E,
        [0x84] = SM83_INST_ADD_H,
        [0x85] = SM83_INST_ADD_L,
        [0x86] = SM83_INST_ADD_pHL,
        [0x87] = SM83_INST_ADD_A,
        [0xC6] = SM83_INST_ADD_u8,
        [0x88] = SM83_INST_ADC_B,
        [0x89] = SM83_INST_ADC_C,
        [0x8A] = SM83_INST_ADC_D,
        [0x8B] = SM83_INST_ADC_E,
        [0x8C] = SM83_INST_ADC_H,
        [0x8D] = SM83_INST_ADC_L,
        [0x8E] = SM83_INST_ADC_pHL,
        [0x8F] = SM83_INST_ADC_A,
        [0xCE] = SM83_INST_ADC_u8,
        [0x09] = SM83_INST_ADD_HL_BC,
        [0x19] = SM83_INST_ADD_HL_DE,
        [0x29] = SM83_INST_ADD_HL_HL,
        [0x39] = SM83_INST_ADD_HL_SP,
        [0xE8] = SM83_INST_ADD_SP_s8,
        // // subtract
        [0x90] = SM83_INST_SUB_B,
        [0x91] = SM83_INST_SUB_C,
        [0x92] = SM83_INST_SUB_D,
        [0x93] = SM83_INST_SUB_E,
        [0x94] = SM83_INST_SUB_H,
        [0x95] = SM83_INST_SUB_L,
        [0x96] = SM83_INST_SUB_pHL,
        [0x97] = SM83_INST_SUB_A,
        [0xD6] = SM83_INST_SUB_u8,
        [0x98] = SM83_INST_SBC_B,
        [0x99] = SM83_INST_SBC_C,
        [0x9A] = SM83_INST_SBC_D,
        [0x9B] = SM83_INST_SBC_E,
        [0x9C] = SM83_INST_SBC_H,
        [0x9D] = SM83_INST_SBC_L,
        [0x9E] = SM83_INST_SBC_pHL,
        [0x9F] = SM83_INST_SBC_A,
        [0xDE] = SM83_INST_SBC_u8,
        // // inc
        [0x04] = SM83_INST_INC_B,
        [0x0C] = SM83_INST_INC_C,
        [0x14] = SM83_INST_INC_D,
        [0x1C] = SM83_INST_INC_E,
        [0x24] = SM83_INST_INC_H,
        [0x2C] = SM83_INST_INC_L,
        [0x34] = SM83_INST_INC_pHL,
        [0x3C] = SM83_INST_INC_A,
        [0x03] = SM83_INST_INC_BC,
        [0x13] = SM83_INST_INC_DE,
        [0x23] = SM83_INST_INC_HL,
        [0x33] = SM83_INST_INC_SP,
        // // dec
        [0x05] = SM83_INST_DEC_B,
        [0x0D] = SM83_INST_DEC_C,
        [0x15] = SM83_INST_DEC_D,
        [0x1D] = SM83_INST_DEC_E,
        [0x25] = SM83_INST_DEC_H,
        [0x2D] = SM83_INST_DEC_L,
        [0x35] = SM83_INST_DEC_pHL,
        [0x3D] = SM83_INST_DEC_A,
        [0x0B] = SM83_INST_DEC_BC,
        [0x1B] = SM83_INST_DEC_DE,
        [0x2B] = SM83_INST_DEC_HL,
        [0x3B] = SM83_INST_DEC_SP,
        // // compare
        [0xB8] = SM83_INST_CP_B,
        [0xB9] = SM83_INST_CP_C,
        [0xBA] = SM83_INST_CP_D,
        [0xBB] = SM83_INST_CP_E,
        [0xBC] = SM83_INST_CP_H,
        [0xBD] = SM83_INST_CP_L,
        [0xBE] = SM83_INST_CP_pHL,
        [0xBF] = SM83_INST_CP_A,
        [0xFE] = SM83_INST_CP_u8,
        // // and
        [0xA0] = SM83_INST_AND_B,
        [0xA1] = SM83_INST_AND_C,
        [0xA2] = SM83_INST_AND_D,
        [0xA3] = SM83_INST_AND_E,
        [0xA4] = SM83_INST_AND_H,
        [0xA5] = SM83_INST_AND_L,
        [0xA6] = SM83_INST_AND_pHL,
        [0xA7] = SM83_INST_AND_A,
        [0xE6] = SM83_INST_AND_u8,
        // // or
        [0xB0] = SM83_INST_OR_B,
        [0xB1] = SM83_INST_OR_C,
        [0xB2] = SM83_INST_OR_D,
        [0xB3] = SM83_INST_OR_E,
        [0xB4] = SM83_INST_OR_H,
        [0xB5] = SM83_INST_OR_L,
        [0xB6] = SM83_INST_OR_pHL,
        [0xB7] = SM83_INST_OR_A,
        [0xF6] = SM83_INST_OR_u8,
        // // xor
        [0xA8] = SM83_INST_XOR_B,
        [0xA9] = SM83_INST_XOR_C,
        [0xAA] = SM83_INST_XOR_D,
        [0xAB] = SM83_INST_XOR_E,
        [0xAC] = SM83_INST_XOR_H,
        [0xAD] = SM83_INST_XOR_L,
        [0xAE] = SM83_INST_XOR_pHL,
        [0xAF] = SM83_INST_XOR_A,
        [0xEE] = SM83_INST_XOR_u8,
        //
        [0xCB] = SM83_INST_CB,
        // null
        [0xD3] = NULL,
        [0xDB] = NULL,
        [0xDD] = NULL,
        [0xE3] = NULL,
        [0xE4] = NULL,
        [0xEB] = NULL,
        [0xEC] = NULL,
        [0xED] = NULL,
        [0xF4] = NULL,
        [0xFC] = NULL,
        [0xFD] = NULL,
    };

    return instructions[opcode];
}

void SM83_Execute(sm83_t * ctx, sm83_instruction_t inst)
{
    if (inst) {
        bool needEnableIME = ctx->RequestEnableIME;

        inst(ctx);

        if (needEnableIME) {
            ctx->IME = true;
            ctx->RequestEnableIME = false;
        }
    }
    else {
        Verbose(ctx, 1, LogWarn("unknown instruction at $%04X", ctx->PC));
    }
}

void SM83_Step(sm83_t * ctx)
{
    static char INST_DISASM[1024];

    if (ctx->Enabled) {
        if (ctx->VerboseLevel >= 3
            || ctx->internal->InstructionLoggingEnabled) {
            int offset = snprintf(
                INST_DISASM, sizeof(INST_DISASM), "$%04X: ", ctx->PC);

            SM83_Disassemble(ctx,
                INST_DISASM + offset,
                sizeof(INST_DISASM) - offset,
                ctx->PC);

            SM83_AddInstructionLogEntry(ctx, INST_DISASM);
            Verbose(ctx, 3, LogInfo("%s", INST_DISASM));
        }

        SM83_Execute(ctx, SM83_Decode(ctx, SM83_Fetch(ctx)));
    }
    else {
        SM83_Tick(ctx, 4);
    }

    SM83_CheckInterrupts(ctx);
}

void SM83_PrintRegisters(sm83_t * ctx)
{
    LogInfo(
        "AF=$%04X BC=$%04X DE=$%04X HL=$%04X SP=$%04X PC=$%04X F=[%c%c%c%c]",
        ctx->AF,
        ctx->BC,
        ctx->DE,
        ctx->HL,
        ctx->SP,
        ctx->PC,
        (ctx->FZ ? 'Z' : '-'),
        (ctx->FN ? 'N' : '-'),
        (ctx->FH ? 'H' : '-'),
        (ctx->FC ? 'C' : '-'));
}
