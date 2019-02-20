#include "cpu.h"

#include "log.h"
#include "memory.h"

#include "inst/add.h"
#include "inst/bit.h"
#include "inst/call.h"
#include "inst/intctl.h"
#include "inst/jump.h"
#include "inst/misc.h"
#include "inst/reset.h"
#include "inst/restart.h"
#include "inst/return.h"
#include "inst/set.h"
#include "inst/subtract.h"

#define GB_CLOCK_SPEED 4194304 // Hz

uint32_t ClockSpeed = GB_CLOCK_SPEED;
uint64_t TickCounter = 0;

uint8_t fetch() 
{
    uint8_t op = readByte(R.PC++);
    TickCounter += 4;
    return op;
}

typedef void(* CB_inst_t)(uint8_t);
inst CB_instructions[0x100] = {
    // bit
    [0x47] = _BIT_b_A,
    [0x40] = _BIT_b_B,
    [0x41] = _BIT_b_C,
    [0x42] = _BIT_b_D,
    [0x43] = _BIT_b_E,
    [0x44] = _BIT_b_H,
    [0x45] = _BIT_b_L,
    [0x46] = _BIT_b_HL,
};

void _CB() 
{
    uint8_t op = fetch();
    CB_inst_t inst = CB_instructions[op];
    if (inst) {
        inst(op);
    } else {
        LogWarn("unknown instruction CB%02X", op);
    }
}

typedef void(* inst_t)();
inst_t instructions[0x100] = {
    // CB
    [0xCB] = _CB,
    // misc
    [0x00] = _NOP,
    [0x76] = _HALT,
    // intctl
    [0xF3] = _DI,
    [0xFB] = _EI,
    // call
    [0xCD] = _CALL_nn,
    [0xC4] = _CALL_NZ_nn,
    [0xCC] = _CALL_Z_nn,
    [0xD4] = _CALL_NC_nn,
    [0xDC] = _CALL_C_nn,
    // rst
    [0xC7] = _RST_00,
    [0xCF] = _RST_08,
    [0xD7] = _RST_10,
    [0xDF] = _RST_18,
    [0xE7] = _RST_20,
    [0xDF] = _RST_28,
    [0xF7] = _RST_30,
    [0xFF] = _RST_38,
    // ret
    [0xC9] = _RET,
    [0xC0] = _RET_NZ,
    [0xC8] = _RET_Z,
    [0xD0] = _RET_NC,
    [0xD8] = _RET_C,
    [0xD9] = _RETI,
};

void execute(uint8_t op) 
{
    inst_t inst = instructions[op];
    if (inst) {
        inst();
    } else {
        LogWarn("unknown instruction %02X", op);
    }
}

void CPUTick(int cycles)
{
    uint8_t op = fetch();
    execute(op);
}

void tick(uint8_t c) { }

void nextInstruction() 
{
    uint8_t op;
    uint8_t b = 0;
    uint8_t sop = 0;

    switch (op) {
    case 0x00: // NOP
        LogVerbose("NOP");
        tick(4);
        break;

    case 0x76: // HALT
        break;

    case 0xF3:
        LogVerbose("DI");
        tick(4);
        IME = false;
        break;
    case 0xFB:
        LogVerbose("EI");
        tick(4);
        IME = true;
        break;

    case 0xCD: // CALL nn
        LogVerbose("CALL %04X", readWord(R.PC));
        tick(12);
        R.SP -= 2;
        writeWord(R.SP, R.PC+2);
        R.PC = readWord(R.PC);
        break;
    case 0xC9: // RET
        LogVerbose("RET");
        tick(12);
        R.PC = readWord(R.SP);
        R.SP += 2;
        break;
    case 0xD9: // RETI
        LogVerbose("RETI");
        tick(8);
        IME = true;
        R.PC = readWord(R.SP);
        R.SP += 2;
        break;

    case 0x01: // LD BC,nn
        LogVerbose("LD BC,%04X", readWord(R.PC));
        tick(12);
        R.BC = readWord(R.PC);
        R.PC += 2;
        break;
    case 0x11: // LD DE,nn
        LogVerbose("LD DE,%04X", readWord(R.PC));
        tick(12);
        R.DE = readWord(R.PC);
        R.PC += 2;
        break;
    case 0x21: // LD HL,nn
        LogVerbose("LD HL,%04X", readWord(R.PC));
        tick(12);
        R.HL = readWord(R.PC);
        R.PC += 2;
        break;
    case 0x31: // LD SP,nn
        LogVerbose("LD SP,%04X", readWord(R.PC));
        tick(12);
        R.SP = readWord(R.PC);
        R.PC += 2;
        break;
    case 0x02: // LD (BC),A
        LogVerbose("LD (BC),A");
        tick(8);
        writeByte(R.BC, R.A);
        break;
    case 0x12: // LD (DE),A
        LogVerbose("LD (DE),A");
        tick(8);
        writeByte(R.DE, R.A);
        break;
    case 0x77: // LD (HL),A
        LogVerbose("LD (HL),A");
        tick(8);
        writeByte(R.HL, R.A);
        break;
    case 0x7F: // LD A,A
        LogVerbose("LD A,A");
        tick(4);
        R.A = R.A;
        break;
    case 0x47: // LD B,A
        LogVerbose("LD A,A");
        tick(4);
        R.B = R.A;
        break;
    case 0x4F: // LD C,A
        LogVerbose("LD C,A");
        tick(4);
        R.C = R.A;
        break;
    case 0x57: // LD D,A
        LogVerbose("LD D,A");
        tick(4);
        R.D = R.A;
        break;
    case 0x5F: // LD E,A
        LogVerbose("LD E,A");
        tick(4);
        R.E = R.A;
        break;
    case 0x67: // LD H,A
        LogVerbose("LD H,A");
        tick(4);
        R.H = R.A;
        break;
    case 0x6F: // LD L,A
        LogVerbose("LD L,A");
        tick(4);
        R.L = R.A;
        break;
    case 0xEA: // LD (nn),A
        LogVerbose("LD (%04X),A", readWord(R.PC));
        tick(16);
        writeByte(readWord(R.PC), R.A);
        R.PC += 2;
        break;
    case 0xE0: // LDH (n),A
        LogVerbose("LDH (%02X),A", readByte(R.PC));
        tick(12);
        writeByte(0xFF00 + readByte(R.PC++), R.A);
        break;
    case 0xF0: // LDH A,(n)
        LogVerbose("LDH A,(%02X)", readByte(R.PC));
        tick(12);
        R.A = readByte(0xFF00 + readByte(R.PC++));
        break;
    case 0x78: // LD A,B
        LogVerbose("LD A,B");
        tick(4);
        R.A = R.B;
        break;
    case 0x79: // LD A,C
        LogVerbose("LD A,C");
        tick(4);
        R.A = R.C;
        break;
    case 0x7A: // LD A,D
        LogVerbose("LD A,D");
        tick(4);
        R.A = R.D;
        break;
    case 0x7B: // LD A,E
        LogVerbose("LD A,E");
        tick(4);
        R.A = R.E;
        break;
    case 0x7C: // LD A,H
        LogVerbose("LD A,H");
        tick(4);
        R.A = R.H;
        break;
    case 0x7D: // LD A,L
        LogVerbose("LD A,L");
        tick(4);
        R.A = R.L;
        break;
    case 0x0A: // LD A,(BC)
        LogVerbose("LD A,(BC)");
        tick(8);
        R.A = readByte(R.BC);
        break;
    case 0x1A: // LD A,(DE)
        LogVerbose("LD A,(DE)");
        tick(8);
        R.A = readByte(R.DE);
        break;
    case 0x7E: // LD A,(HL)
        LogVerbose("LD A,(HL)");
        tick(8);
        R.A = readByte(R.HL);
        break;
    case 0xFA: // LD A,(nn)
        LogVerbose("LD A,(%04X)", readWord(R.PC));
        tick(8);
        R.A = readByte(readWord(R.PC));
        R.PC += 2;
        break;
    case 0x3E: // LD A,#
        LogVerbose("LD A,%02X", readByte(R.PC));
        tick(8);
        R.A = readByte(R.PC);
        break;
    
    case 0xCB: // Bit Manipulation
        sop = readByte(R.PC++);
        //LogVerbose("sop %02X", sop);
        switch (sop) {
        case 0x37: // SWAP A
            LogVerbose("SWAP A");
            R.A = ((R.A & 0x0F) << 4) | ((R.A & 0xF0) >> 4);
            break;
        case 0x30: // SWAP B
            LogVerbose("SWAP B");
            R.B = ((R.B & 0x0F) << 4) | ((R.B & 0xF0) >> 4);
            break;
        case 0x31: // SWAP C
            LogVerbose("SWAP C");
            R.C = ((R.C & 0x0F) << 4) | ((R.C & 0xF0) >> 4);
            break;
        case 0x32: // SWAP D
            LogVerbose("SWAP D");
            R.D = ((R.D & 0x0F) << 4) | ((R.D & 0xF0) >> 4);
            break;
        case 0x33: // SWAP E
            LogVerbose("SWAP E");
            R.E = ((R.E & 0x0F) << 4) | ((R.E & 0xF0) >> 4);
            break;
        case 0x34: // SWAP H
            LogVerbose("SWAP H");
            R.H = ((R.H & 0x0F) << 4) | ((R.H & 0xF0) >> 4);
            break;
        case 0x35: // SWAP L
            LogVerbose("SWAP L");
            R.L = ((R.L & 0x0F) << 4) | ((R.L & 0xF0) >> 4);
            break;
        case 0x36: // SWAP (HL)
            LogVerbose("SWAP (HL)");
            {
                uint8_t n = readByte(R.HL);
                writeByte(R.HL, ((n & 0x0F) << 4) | ((n & 0xF0) >> 4));
            }
            break;
        
        case 0x07: // RLC A
            LogVerbose("RLC A");
            tick(8);
            break;
        case 0x00: // RLC B
            LogVerbose("RLC B");
            tick(8);
            break;
        case 0x01: // RLC C
            LogVerbose("RLC C");
            tick(8);
            break;
        case 0x02: // RLC D
            LogVerbose("RLC D");
            tick(8);
            break;
        case 0x03: // RLC E
            LogVerbose("RLC E");
            tick(8);
            break;
        case 0x04: // RLC H
            LogVerbose("RLC H");
            tick(8);
            break;
        case 0x05: // RLC L
            LogVerbose("RLC L");
            tick(8);
            break;
        case 0x06: // RLC (HL)
            LogVerbose("RLC (HL)");
            tick(16);
            break;

        case 0x17: // RL A
            LogVerbose("RL A");
            tick(8);
            break;
        case 0x10: // RL B
            LogVerbose("RL B");
            tick(8);
            break;
        case 0x11: // RL C
            LogVerbose("RL C");
            tick(8);
            break;
        case 0x12: // RL D
            LogVerbose("RL D");
            tick(8);
            break;
        case 0x13: // RL E
            LogVerbose("RL E");
            tick(8);
            break;
        case 0x14: // RL H
            LogVerbose("RL H");
            tick(8);
            break;
        case 0x15: // RL L
            LogVerbose("RL L");
            tick(8);
            break;
        case 0x16: // RL (HL)
            LogVerbose("RL (HL)");
            tick(16);
            break;

        case 0x0F: // RRC A
            LogVerbose("RRC A");
            tick(8);
            break;
        case 0x08: // RRC B
            LogVerbose("RRC B");
            tick(8);
            break;
        case 0x09: // RRC C
            LogVerbose("RRC C");
            tick(8);
            break;
        case 0x0A: // RRC D
            LogVerbose("RRC D");
            tick(8);
            break;
        case 0x0B: // RRC E
            LogVerbose("RRC E");
            tick(8);
            break;
        case 0x0C: // RRC H
            LogVerbose("RRC H");
            tick(8);
            break;
        case 0x0D: // RRC L
            LogVerbose("RRC L");
            tick(8);
            break;
        case 0x0E: // RRC (HL)
            LogVerbose("RRC (HL)");
            tick(16);
            break;
        
        case 0x1F: // RR A
            LogVerbose("RR A");
            tick(8);
            break;
        case 0x18: // RR B
            LogVerbose("RR B");
            tick(8);
            break;
        case 0x19: // RR C
            LogVerbose("RR C");
            tick(8);
            break;
        case 0x1A: // RR D
            LogVerbose("RR D");
            tick(8);
            break;
        case 0x1B: // RR E
            LogVerbose("RR E");
            tick(8);
            break;
        case 0x1C: // RR H
            LogVerbose("RR H");
            tick(8);
            break;
        case 0x1D: // RR L
            LogVerbose("RR L");
            tick(8);
            break;
        case 0x1E: // RR (HL)
            LogVerbose("RR (HL)");
            tick(16);
            break;
        
        case 0x27: // SLA A
            LogVerbose("SLA A");
            tick(8);
            break;
        case 0x20: // SLA B
            LogVerbose("SLA B");
            tick(8);
            break;
        case 0x21: // SLA C
            LogVerbose("SLA C");
            tick(8);
            break;
        case 0x22: // SLA D
            LogVerbose("SLA D");
            tick(8);
            break;
        case 0x23: // SLA E
            LogVerbose("SLA E");
            tick(8);
            break;
        case 0x24: // SLA H
            LogVerbose("SLA H");
            tick(8);
            break;
        case 0x25: // SLA L
            LogVerbose("SLA L");
            tick(8);
            break;
        case 0x26: // SLA (HL)
            LogVerbose("SLA (HL)");
            tick(16);
            break;

        case 0x2F: // SRA A
            LogVerbose("SRA A");
            tick(8);
            break;
        case 0x28: // SRA B
            LogVerbose("SRA B");
            tick(8);
            break;
        case 0x29: // SRA C
            LogVerbose("SRA C");
            tick(8);
            break;
        case 0x2A: // SRA D
            LogVerbose("SRA D");
            tick(8);
            break;
        case 0x2B: // SRA E
            LogVerbose("SRA E");
            tick(8);
            break;
        case 0x2C: // SRA H
            LogVerbose("SRA H");
            tick(8);
            break;
        case 0x2D: // SRA L
            LogVerbose("SRA L");
            tick(8);
            break;
        case 0x2E: // SRA (HL)
            LogVerbose("SRA (HL)");
            tick(16);
            break;
        
        case 0x3F: // SRL A
            LogVerbose("SRL A");
            tick(8);
            break;
        case 0x38: // SRL B
            LogVerbose("SRL B");
            tick(8);
            break;
        case 0x39: // SRL C
            LogVerbose("SRL C");
            tick(8);
            break;
        case 0x3A: // SRL D
            LogVerbose("SRL D");
            tick(8);
            break;
        case 0x3B: // SRL E
            LogVerbose("SRL E");
            tick(8);
            break;
        case 0x3C: // SRL H
            LogVerbose("SRL H");
            tick(8);
            break;
        case 0x3D: // SRL L
            LogVerbose("SRL L");
            tick(8);
            break;
        case 0x3E: // SRL (HL)
            LogVerbose("SRL (HL)");
            tick(16);
            break;
        
        
        case 0xC7: // SET b,A
            LogVerbose("SET b,A");
            tick(8);
            break;
        case 0xC0: // SET b,B
            LogVerbose("SET b,B");
            tick(8);
            break;
        case 0xC1: // SET b,C
            LogVerbose("SET b,C");
            tick(8);
            break;
        case 0xC2: // SET b,D
            LogVerbose("SET b,D");
            tick(8);
            break;
        case 0xC3: // SET b,E
            LogVerbose("SET b,E");
            tick(8);
            break;
        case 0xC4: // SET b,H
            LogVerbose("SET b,H");
            tick(8);
            break;
        case 0xC5: // SET b,L
            LogVerbose("SET b,L");
            tick(8);
            break;
        case 0xC6: // SET b,(HL)
            LogVerbose("SET b,(HL)");
            tick(16);
            break;
        
        case 0x87: // RES 0,A
        case 0x8F: // RES 1,A
        case 0x97: // RES 2,A
        case 0x9F: // RES 3,A
        case 0xA7: // RES 4,A
        case 0xAF: // RES 5,A
        case 0xB7: // RES 6,A
        case 0xBF: // RES 7,A
            b = (0x87 - sop) / 8;
            LogVerbose("RES %d,A", b);
            tick(8);
            R.A = R.A & ~(1 << b);
            break;
        case 0x80: // RES b,B
            LogVerbose("RES b,B");
            tick(8);
            break;
        case 0x81: // RES b,C
            LogVerbose("RES b,C");
            tick(8);
            break;
        case 0x82: // RES b,D
            LogVerbose("RES b,D");
            tick(8);
            break;
        case 0x83: // RES b,E
            LogVerbose("RES b,E");
            tick(8);
            break;
        case 0x84: // RES b,H
            LogVerbose("RES b,H");
            tick(8);
            break;
        case 0x85: // RES b,L
            LogVerbose("RES b,L");
            tick(8);
            break;
        case 0x86: // RES b,(HL)
            LogVerbose("RES b,(HL)");
            tick(16);
            break;
        }
        break;

    case 0xF5: // PUSH AF
        LogVerbose("PUSH AF");
        tick(16);
        R.SP -= 2;
        writeWord(R.SP, R.AF);
        break;
    case 0xC5: // PUSH BC
        LogVerbose("PUSH BC");
        tick(16);
        R.SP -= 2;
        writeWord(R.SP, R.BC);
        break;
    case 0xD5: // PUSH DE
        LogVerbose("PUSH DE");
        tick(16);
        R.SP -= 2;
        writeWord(R.SP, R.DE);
        break;
    case 0xE5: // PUSH HL
        LogVerbose("PUSH HL");
        tick(16);
        R.SP -= 2;
        writeWord(R.SP, R.HL);
        break;

    case 0xF1: // POP AF
        LogVerbose("POP AF");
        tick(12);
        R.AF = readWord(R.SP);
        R.SP += 2;
        break;
    case 0xC1: // POP BC
        LogVerbose("POP BC");
        tick(12);
        R.BC = readWord(R.SP);
        R.SP += 2;
        break;
    case 0xD1: // POP DE
        LogVerbose("POP DE");
        tick(12);
        R.DE = readWord(R.SP);
        R.SP += 2;
        break;
    case 0xE1: // POP HL
        LogVerbose("POP HL");
        tick(12);
        R.HL = readWord(R.SP);
        R.SP += 2;
        break;

    case 0x09: // ADD HL,BC
        LogVerbose("ADD HL,BC");
        R.HL = add16(R.HL, R.BC);
        break;
    case 0x19: // ADD HL,DE
        LogVerbose("ADD HL,DE");
        R.HL = add16(R.HL, R.DE);
        break;
    case 0x29: // ADD HL,HL
        LogVerbose("ADD HL,HL");
        R.HL = add16(R.HL, R.HL);
        break;
    case 0x39: // ADD HL,SP
        LogVerbose("ADD HL,SP");
        R.HL = add16(R.HL, R.SP);
        break;

    case 0x87: // ADD A,A
        LogVerbose("ADD A,A");
        tick(4);
        R.A = add8(R.A, R.A);
        break;
    case 0x80: // ADD A,B
        LogVerbose("ADD A,B");
        tick(4);
        R.A = add8(R.A, R.B);
        break;
    case 0x81: // ADD A,C
        LogVerbose("ADD A,C");
        tick(4);
        R.A = add8(R.A, R.C);
        break;
    case 0x82: // ADD A,D
        LogVerbose("ADD A,D");
        tick(4);
        R.A = add8(R.A, R.D);
        break;
    case 0x83: // ADD A,E
        LogVerbose("ADD A,E");
        tick(4);
        R.A = add8(R.A, R.E);
        break;
    case 0x84: // ADD A,H
        LogVerbose("ADD A,H");
        tick(4);
        R.A = add8(R.A, R.H);
        break;
    case 0x85: // ADD A,L
        LogVerbose("ADD A,L");
        tick(4);
        R.A = add8(R.A, R.L);
        break;
    case 0x86: // ADD A,(HL)
        LogVerbose("ADD A,(HL)");
        tick(8);
        R.A = add8(R.A, readByte(R.HL));
        break;
    case 0xC6: // ADD A,#
        LogVerbose("ADD A,#");
        tick(8);
        R.A = add8(R.A, readByte(R.PC++));
        break;
    
    case 0x97: // SUB A
        LogVerbose("SUB A");
        tick(4);
        R.A = sub8(R.A, R.A);
        break;
    case 0x90: // SUB B
        LogVerbose("SUB B");
        tick(4);
        R.A = sub8(R.A, R.B);
        break;
    case 0x91: // SUB C
        LogVerbose("SUB C");
        tick(4);
        R.A = sub8(R.A, R.C);
        break;
    case 0x92: // SUB D
        LogVerbose("SUB D");
        tick(4);
        R.A = sub8(R.A, R.D);
        break;
    case 0x93: // SUB E
        LogVerbose("SUB E");
        tick(4);
        R.A = sub8(R.A, R.E);
        break;
    case 0x94: // SUB H
        LogVerbose("SUB H");
        tick(4);
        R.A = sub8(R.A, R.H);
        break;
    case 0x95: // SUB L
        LogVerbose("SUB L");
        tick(4);
        R.A = sub8(R.A, R.L);
        break;
    case 0x96: // SUB (HL)
        LogVerbose("SUB (HL)");
        tick(8);
        R.A = sub8(R.A, readByte(R.HL));
        break;
    case 0xD6: // SUB #
        LogVerbose("SUB #");
        tick(8);
        R.A = sub8(R.A, readByte(R.PC++));
        break;

    case 0xA7: // AND A
        LogVerbose("AND A");
        tick(4);
        R.A = and8(R.A, R.A);
        break;
    case 0xA0: // AND B
        LogVerbose("AND B");
        tick(4);
        R.A = and8(R.A, R.B);
        break;
    case 0xA1: // AND C
        LogVerbose("AND C");
        tick(4);
        R.A = and8(R.A, R.C);
        break;
    case 0xA2: // AND D
        LogVerbose("AND D");
        tick(4);
        R.A = and8(R.A, R.D);
        break;
    case 0xA3: // AND E
        LogVerbose("AND E");
        tick(4);
        R.A = and8(R.A, R.E);
        break;
    case 0xA4: // AND H
        LogVerbose("AND H");
        tick(4);
        R.A = and8(R.A, R.H);
        break;
    case 0xA5: // AND L
        LogVerbose("AND L");
        tick(4);
        R.A = and8(R.A, R.L);
        break;
    case 0xA6: // AND (HL)
        LogVerbose("AND (HL)");
        tick(8);
        R.A = and8(R.A, readByte(R.HL));
        break;
    case 0xE6: // AND #
        LogVerbose("AND #");
        tick(8);
        R.A = and8(R.A, readByte(R.PC++));
        break;
    
    case 0xB7: // OR A
        LogVerbose("OR A");
        tick(4);
        R.A = or8(R.A, R.A);
        break;
    case 0xB0: // OR B
        LogVerbose("OR B");
        tick(4);
        R.A = or8(R.A, R.B);
        break;
    case 0xB1: // OR C
        LogVerbose("OR C");
        tick(4);
        R.A = or8(R.A, R.C);
        break;
    case 0xB2: // OR D
        LogVerbose("OR D");
        tick(4);
        R.A = or8(R.A, R.D);
        break;
    case 0xB3: // OR E
        LogVerbose("OR E");
        tick(4);
        R.A = or8(R.A, R.E);
        break;
    case 0xB4: // OR H
        LogVerbose("OR H");
        tick(4);
        R.A = or8(R.A, R.H);
        break;
    case 0xB5: // OR L
        LogVerbose("OR L");
        tick(4);
        R.A = or8(R.A, R.L);
        break;
    case 0xB6: // OR (HL)
        LogVerbose("OR (HL)");
        tick(8);
        R.A = or8(R.A, readByte(R.HL));
        break;
    case 0xF6: // OR #
        LogVerbose("OR #");
        tick(8);
        R.A = or8(R.A, readByte(R.PC++));
        break;
    
    case 0xAF: // XOR A
        LogVerbose("XOR A");
        tick(4);
        R.A = xor8(R.A, R.A);
        break;
    case 0xA8: // XOR B
        LogVerbose("XOR B");
        tick(4);
        R.A = xor8(R.A, R.B);
        break;
    case 0xA9: // XOR C
        LogVerbose("XOR C");
        tick(4);
        R.A = xor8(R.A, R.C);
        break;
    case 0xAA: // XOR D
        LogVerbose("XOR D");
        tick(4);
        R.A = xor8(R.A, R.D);
        break;
    case 0xAB: // XOR E
        LogVerbose("XOR E");
        tick(4);
        R.A = xor8(R.A, R.E);
        break;
    case 0xAC: // XOR H
        LogVerbose("XOR H");
        tick(4);
        R.A = xor8(R.A, R.H);
        break;
    case 0xAD: // XOR L
        LogVerbose("XOR L");
        tick(4);
        R.A = xor8(R.A, R.L);
        break;
    case 0xAE: // XOR (HL)
        LogVerbose("XOR (HL)");
        tick(4);
        R.A = xor8(R.A, readByte(R.HL));
        break;
    case 0xEE: // XOR #
        LogVerbose("XOR #");
        tick(4);
        R.A = xor8(R.A, readByte(R.PC++));
        break;

    case 0xBF: // CP A
        LogVerbose("CP A");
        tick(4);
        sub8(R.A, R.A);
        break;
    case 0xB8: // CP B
        LogVerbose("CP B");
        tick(4);
        sub8(R.A, R.B);
        break;
    case 0xB9: // CP C
        LogVerbose("CP C");
        tick(4);
        sub8(R.A, R.C);
        break;
    case 0xBA: // CP D
        LogVerbose("CP D");
        tick(4);
        sub8(R.A, R.D);
        break;
    case 0xBB: // CP E
        LogVerbose("CP E");
        tick(4);
        sub8(R.A, R.E);
        break;
    case 0xBC: // CP H
        LogVerbose("CP H");
        tick(4);
        sub8(R.A, R.H);
        break;
    case 0xBD: // CP L
        LogVerbose("CP L");
        tick(4);
        sub8(R.A, R.L);
        break;
    case 0xBE: // CP (HL)
        LogVerbose("CP (HL)");
        tick(8);
        sub8(R.A, readByte(R.HL));
        break;
    case 0xFE: // CP #
        LogVerbose("CP %02X", readByte(R.PC));
        tick(8);
        sub8(R.A, readByte(R.PC++));
        break;
    
    case 0x3C: // INC A
        LogVerbose("INC A");
        tick(4);
        R.A = inc8(R.A);
        break;
    case 0x04: // INC B
        LogVerbose("INC B");
        tick(4);
        R.B = inc8(R.B);
        break;
    case 0x0C: // INC C
        LogVerbose("INC C");
        tick(4);
        R.C = inc8(R.C);
        break;
    case 0x14: // INC D
        LogVerbose("INC D");
        tick(4);
        R.D = inc8(R.D);
        break;
    case 0x1C: // INC E
        LogVerbose("INC E");
        tick(4);
        R.E = inc8(R.E);
        break;
    case 0x24: // INC H
        LogVerbose("INC H");
        tick(4);
        R.H = inc8(R.H);
        break;
    case 0x2C: // INC L
        LogVerbose("INC L");
        tick(4);
        R.L = inc8(R.L);
        break;
    case 0x34: // INC 
        LogVerbose("INC (HL)");
        tick(12);
        writeByte(R.HL, inc8(readByte(R.HL)));
        break;

    case 0xC3: // JP nn
        LogVerbose("JP %02X", readWord(R.PC));
        tick(12);
        R.PC = readWord(R.PC);
        break;
    case 0xC2: // JP NZ,nn
        break;
    case 0xCA: // JP Z,nn
        break;
    case 0xD2: // JP NC,nn
        break;
    case 0xDA: // JP C,nn
        break;
    
    case 0x18: // JR *
        LogVerbose("JR %02X", readByte(R.PC));
        tick(8);
        R.PC += (int8_t)readByte(R.PC);
        break;
    case 0x20: // JR NZ,*
        LogVerbose("JR NZ,%02X", readByte(R.PC));
        tick(8);
        if (!R.FZ) {
            tick(12);
            R.PC += (int8_t)readByte(R.PC);
        }
        break;
    case 0x28: // JR Z,*
        LogVerbose("JR Z,%02X", readByte(R.PC));
        tick(8);
        if (R.FZ) {
            tick(12);
            R.PC += (int8_t)readByte(R.PC);
        }
        break;
    case 0x30: // JR NC,*
        LogVerbose("JR NC,%02X", readByte(R.PC));
        tick(8);
        if (!R.FC) {
            tick(12);
            R.PC += (int8_t)readByte(R.PC);
        }
        break;
    case 0x38: // JR C,*
        LogVerbose("JR C,%02X", readByte(R.PC));
        tick(8);
        if (R.FC) {
            tick(12);
            R.PC += (int8_t)readByte(R.PC);
        }
        break;
    }

    // TODO: delay by cycles
}