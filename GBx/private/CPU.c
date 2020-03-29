#include <GBx/CPU.h>

#include <GBx/APU.h>
#include <GBx/Cartridge.h>
#include <GBx/Bootstrap.h>
#include <GBx/Interrupts.h>
#include <GBx/LCD.h>
#include <GBx/MMU.h>
#include <GBx/Timer.h>

#include <GBx/Log.h>

#include "Inst/ADC.h"
#include "Inst/ADD.h"
#include "Inst/ADDHL.h"

#include "Inst/ADC.h"
#include "Inst/ADD.h"
#include "Inst/ADDHL.h"
#include "Inst/AND.h"
#include "Inst/CALL.h"
#include "Inst/CB.h"
#include "Inst/CP.h"
#include "Inst/DEC.h"
#include "Inst/INC.h"
#include "Inst/JP.h"
#include "Inst/JR.h"
#include "Inst/LD.h"
#include "Inst/Misc.h"
#include "Inst/OR.h"
#include "Inst/POP.h"
#include "Inst/PUSH.h"
#include "Inst/RET.h"
#include "Inst/RL.h"
#include "Inst/RLC.h"
#include "Inst/RR.h"
#include "Inst/RRC.h"
#include "Inst/RST.h"
#include "Inst/SBC.h"
#include "Inst/SUB.h"
#include "Inst/XOR.h"

registers R;

bool CPUEnabled;

uint64_t TotalTicks;

unsigned ClockSpeed;

word LastInstructionAddress;

word StackBaseAddress;

typedef void(* inst_t)();

inst_t instructions[] = {
    // CB
    [0xCB] = _CB,
    // misc
    [0x00] = _NOP,
    [0x76] = _HALT,
    [0x10] = _STOP,
    [0xF3] = _DI,
    [0xFB] = _EI,
    [0x27] = _DAA,
    [0x37] = _SCF,
    [0x2F] = _CPL,
    [0x3F] = _CCF,
    // call
    [0xCD] = _CALL_uu,
    [0xC4] = _CALL_NZ_uu,
    [0xCC] = _CALL_Z_uu,
    [0xD4] = _CALL_NC_uu,
    [0xDC] = _CALL_C_uu,
    // restart
    [0xC7] = _RST_00,
    [0xCF] = _RST_08,
    [0xD7] = _RST_10,
    [0xDF] = _RST_18,
    [0xE7] = _RST_20,
    [0xEF] = _RST_28,
    [0xF7] = _RST_30,
    [0xFF] = _RST_38,
    // return
    [0xC9] = _RET,
    [0xC0] = _RET_NZ,
    [0xC8] = _RET_Z,
    [0xD0] = _RET_NC,
    [0xD8] = _RET_C,
    [0xD9] = _RETI,
    // rotate
    [0x07] = _RLC_A,
    [0x0F] = _RRC_A,
    [0x17] = _RL_A,
    [0x1F] = _RR_A,
    // jump
    [0xC3] = _JP_uu,
    [0xC2] = _JP_NZ_uu,
    [0xCA] = _JP_Z_uu,
    [0xD2] = _JP_NC_uu,
    [0xDA] = _JP_C_uu,
    [0x18] = _JR_s,
    [0x20] = _JR_NZ_s,
    [0x28] = _JR_Z_s,
    [0x30] = _JR_NC_s,
    [0x38] = _JR_C_s,
    [0xE9] = _JP_HL,
    // stack
    [0xF5] = _PUSH_AF,
    [0xC5] = _PUSH_BC,
    [0xD5] = _PUSH_DE,
    [0xE5] = _PUSH_HL,
    [0xF1] = _POP_AF,
    [0xC1] = _POP_BC,
    [0xD1] = _POP_DE,
    [0xE1] = _POP_HL,
    // load
    [0x40] = _LD_B_B,
    [0x41] = _LD_B_C,
    [0x42] = _LD_B_D,
    [0x43] = _LD_B_E,
    [0x44] = _LD_B_H,
    [0x45] = _LD_B_L,
    [0x46] = _LD_B_pHL,
    [0x47] = _LD_B_A,
    [0x48] = _LD_C_B,
    [0x49] = _LD_C_C,
    [0x4A] = _LD_C_D,
    [0x4B] = _LD_C_E,
    [0x4C] = _LD_C_H,
    [0x4D] = _LD_C_L,
    [0x4E] = _LD_C_pHL,
    [0x4F] = _LD_C_A,
    [0x50] = _LD_D_B,
    [0x51] = _LD_D_C,
    [0x52] = _LD_D_D,
    [0x53] = _LD_D_E,
    [0x54] = _LD_D_H,
    [0x55] = _LD_D_L,
    [0x56] = _LD_D_pHL,
    [0x57] = _LD_D_A,
    [0x58] = _LD_E_B,
    [0x59] = _LD_E_C,
    [0x5A] = _LD_E_D,
    [0x5B] = _LD_E_E,
    [0x5C] = _LD_E_H,
    [0x5D] = _LD_E_L,
    [0x5E] = _LD_E_pHL,
    [0x5F] = _LD_E_A,
    [0x60] = _LD_H_B,
    [0x61] = _LD_H_C,
    [0x62] = _LD_H_D,
    [0x63] = _LD_H_E,
    [0x64] = _LD_H_H,
    [0x65] = _LD_H_L,
    [0x66] = _LD_H_pHL,
    [0x67] = _LD_H_A,
    [0x68] = _LD_L_B,
    [0x69] = _LD_L_C,
    [0x6A] = _LD_L_D,
    [0x6B] = _LD_L_E,
    [0x6C] = _LD_L_H,
    [0x6D] = _LD_L_L,
    [0x6E] = _LD_L_pHL,
    [0x6F] = _LD_L_A,
    [0x70] = _LD_pHL_B,
    [0x71] = _LD_pHL_C,
    [0x72] = _LD_pHL_D,
    [0x73] = _LD_pHL_E,
    [0x74] = _LD_pHL_H,
    [0x75] = _LD_pHL_L,
    [0x77] = _LD_pHL_A,
    [0x78] = _LD_A_B,
    [0x79] = _LD_A_C,
    [0x7A] = _LD_A_D,
    [0x7B] = _LD_A_E,
    [0x7C] = _LD_A_H,
    [0x7D] = _LD_A_L,
    [0x7E] = _LD_A_pHL,
    [0x7F] = _LD_A_A,
    [0x02] = _LD_pBC_A,
    [0x12] = _LD_pDE_A,
    [0x06] = _LD_B_u,
    [0x0E] = _LD_C_u,
    [0x16] = _LD_D_u,
    [0x1E] = _LD_E_u,
    [0x26] = _LD_H_u,
    [0x2E] = _LD_L_u,
    [0x36] = _LD_pHL_u,
    [0x3E] = _LD_A_u,
    [0x0A] = _LD_A_pBC,
    [0x1A] = _LD_A_pDE,
    [0x01] = _LD_BC_uu,
    [0x11] = _LD_DE_uu,
    [0x21] = _LD_HL_uu,
    [0x31] = _LD_SP_uu,
    [0xF9] = _LD_SP_HL,
    [0x08] = _LD_puu_SP,
    [0xEA] = _LD_puu_A,
    [0xFA] = _LD_A_puu,
    [0x22] = _LDI_pHL_A,
    [0x32] = _LDD_pHL_A,
    [0x2A] = _LDI_A_pHL,
    [0x3A] = _LDD_A_pHL,
    [0xE0] = _LDH_pu_A,
    [0xF0] = _LDH_A_pu,
    [0xE2] = _LDH_pC_A,
    [0xF2] = _LDH_A_pC,
    [0xF8] = _LD_HL_SP_s,
    // add
    [0x80] = _ADD_B,
    [0x81] = _ADD_C,
    [0x82] = _ADD_D,
    [0x83] = _ADD_E,
    [0x84] = _ADD_H,
    [0x85] = _ADD_L,
    [0x86] = _ADD_pHL,
    [0x87] = _ADD_A,
    [0xC6] = _ADD_u,
    [0x88] = _ADC_B,
    [0x89] = _ADC_C,
    [0x8A] = _ADC_D,
    [0x8B] = _ADC_E,
    [0x8C] = _ADC_H,
    [0x8D] = _ADC_L,
    [0x8E] = _ADC_pHL,
    [0x8F] = _ADC_A,
    [0xCE] = _ADC_u,
    [0x09] = _ADD_HL_BC,
    [0x19] = _ADD_HL_DE,
    [0x29] = _ADD_HL_HL,
    [0x39] = _ADD_HL_SP,
    [0xE8] = _ADD_SP_s,
    // subtract
    [0x90] = _SUB_B,
    [0x91] = _SUB_C,
    [0x92] = _SUB_D,
    [0x93] = _SUB_E,
    [0x94] = _SUB_H,
    [0x95] = _SUB_L,
    [0x96] = _SUB_pHL,
    [0x97] = _SUB_A,
    [0xD6] = _SUB_u,
    [0x98] = _SBC_B,
    [0x99] = _SBC_C,
    [0x9A] = _SBC_D,
    [0x9B] = _SBC_E,
    [0x9C] = _SBC_H,
    [0x9D] = _SBC_L,
    [0x9E] = _SBC_pHL,
    [0x9F] = _SBC_A,
    [0xDE] = _SBC_u,
    // inc
    [0x04] = _INC_B,
    [0x0C] = _INC_C,
    [0x14] = _INC_D,
    [0x1C] = _INC_E,
    [0x24] = _INC_H,
    [0x2C] = _INC_L,
    [0x34] = _INC_pHL,
    [0x3C] = _INC_A,
    [0x03] = _INC_BC,
    [0x13] = _INC_DE,
    [0x23] = _INC_HL,
    [0x33] = _INC_SP,
    // dec
    [0x05] = _DEC_B,
    [0x0D] = _DEC_C,
    [0x15] = _DEC_D,
    [0x1D] = _DEC_E,
    [0x25] = _DEC_H,
    [0x2D] = _DEC_L,
    [0x35] = _DEC_pHL,
    [0x3D] = _DEC_A,
    [0x0B] = _DEC_BC,
    [0x1B] = _DEC_DE,
    [0x2B] = _DEC_HL,
    [0x3B] = _DEC_SP,
    // compare
    [0xB8] = _CP_B,
    [0xB9] = _CP_C,
    [0xBA] = _CP_D,
    [0xBB] = _CP_E,
    [0xBC] = _CP_H,
    [0xBD] = _CP_L,
    [0xBE] = _CP_pHL,
    [0xBF] = _CP_A,
    [0xFE] = _CP_u,
    // and
    [0xA0] = _AND_B,
    [0xA1] = _AND_C,
    [0xA2] = _AND_D,
    [0xA3] = _AND_E,
    [0xA4] = _AND_H,
    [0xA5] = _AND_L,
    [0xA6] = _AND_pHL,
    [0xA7] = _AND_A,
    [0xE6] = _AND_u,
    // or
    [0xB0] = _OR_B,
    [0xB1] = _OR_C,
    [0xB2] = _OR_D,
    [0xB3] = _OR_E,
    [0xB4] = _OR_H,
    [0xB5] = _OR_L,
    [0xB6] = _OR_pHL,
    [0xB7] = _OR_A,
    [0xF6] = _OR_u,
    // xor
    [0xA8] = _XOR_B,
    [0xA9] = _XOR_C,
    [0xAA] = _XOR_D,
    [0xAB] = _XOR_E,
    [0xAC] = _XOR_H,
    [0xAD] = _XOR_L,
    [0xAE] = _XOR_pHL,
    [0xAF] = _XOR_A,
    [0xEE] = _XOR_u,
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

void ResetCPU() 
{
    CPUEnabled = true;
    
    R.AF = 0x01B0;
    R.BC = 0x0013;
    R.DE = 0x00D8;
    R.HL = 0x014D;
    R.PC = (HasBootstrapROM ? 0x0000 : 0x0100);
    R.SP = 0xFFFE;

    LastInstructionAddress = R.PC;
    StackBaseAddress = R.SP;

    TotalTicks = 0;

    ClockSpeed = DMG_CLOCK_SPEED;
    
    if (SuperEnabled) {
        ClockSpeed = SGB_CLOCK_SPEED;
    }
    else if (ColorEnabled) {
        R.A = 0x11;
        ClockSpeed = CGB_CLOCK_SPEED;
    }
}

void Tick(unsigned ticks)
{
    TimerTick(ticks);
    LCDTick(ticks);
    APUTick(ticks);
    TotalTicks += ticks;
}

byte Fetch()
{
    LastInstructionAddress = R.PC;

    byte op = NextByte();
    Tick(4);
    return op;
}

void Execute(byte opcode)
{
    inst_t inst = instructions[opcode];
    if (inst) {
        inst();
    } else {
        LogWarn("unknown instruction at %04X, %02X", R.PC, opcode);
    }
}

void NextInstruction()
{
    if (CPUEnabled) {
        Execute(Fetch());
    } else {
        Tick(1);
    }

    CheckInterrupts();
}

void PrintRegisters()
{
    LogInfo("AF=%04X BC=%04X DE=%04X HL=%04X SP=%04X PC=%04X F=[%c%c%c%c]",
        R.AF, R.BC, R.DE, R.HL, R.SP, R.PC, 
        (R.FZ ? 'Z' : '-'), 
        (R.FN ? 'N' : '-'), 
        (R.FH ? 'H' : '-'), 
        (R.FC ? 'C' : '-')
    );
}

