#include <stdint.h>
#include <string.h>

#include "log.h"
#include "usage.h"
#include "memory.h"
#include "register.h"
#include "interrupt.h"

#include "alu.h"

const int CLOCK_SPEED = 4194304; // Hz

void tick() 
{
    int cycles = 0;

    uint8_t op = readByte(R.PC++);
    LogVerbose("op %02X", op);

    uint8_t b = 0;
    uint8_t sop = 0;

    switch (op) {
    case 0x00: // NOP
        LogVerbose("NOP");
        cycles = 4;
        break;

    case 0x76: // HALT
        break;

    case 0xF3:
        LogVerbose("DI");
        cycles = 4;
        IME = false;
        break;
    case 0xFB:
        LogVerbose("EI");
        cycles = 4;
        IME = true;
        break;

    case 0x01: // LD BC,nn
        LogVerbose("LD BC,%04X", readWord(R.PC));
        cycles = 12;
        R.BC = readWord(R.PC);
        R.PC += 2;
        break;
    case 0x11: // LD DE,nn
        LogVerbose("LD DE,%04X", readWord(R.PC));
        cycles = 12;
        R.DE = readWord(R.PC);
        R.PC += 2;
        break;
    case 0x21: // LD HL,nn
        LogVerbose("LD HL,%04X", readWord(R.PC));
        cycles = 12;
        R.HL = readWord(R.PC);
        R.PC += 2;
        break;
    case 0x31: // LD SP,nn
        LogVerbose("LD SP,%04X", readWord(R.PC));
        cycles = 12;
        R.SP = readWord(R.PC);
        R.PC += 2;
        break;
    case 0x02: // LD (BC),A
        LogVerbose("LD (BC),A");
        cycles = 8;
        writeByte(R.BC, R.A);
        break;
    case 0x12: // LD (DE),A
        LogVerbose("LD (DE),A");
        cycles = 8;
        writeByte(R.DE, R.A);
        break;
    case 0x77: // LD (HL),A
        LogVerbose("LD (HL),A");
        cycles = 8;
        writeByte(R.HL, R.A);
        break;
    case 0x7F: // LD A,A
        LogVerbose("LD A,A");
        cycles = 4;
        R.A = R.A;
        break;
    case 0x47: // LD B,A
        LogVerbose("LD A,A");
        cycles = 4;
        R.B = R.A;
        break;
    case 0x4F: // LD C,A
        LogVerbose("LD C,A");
        cycles = 4;
        R.C = R.A;
        break;
    case 0x57: // LD D,A
        LogVerbose("LD D,A");
        cycles = 4;
        R.D = R.A;
        break;
    case 0x5F: // LD E,A
        LogVerbose("LD E,A");
        cycles = 4;
        R.E = R.A;
        break;
    case 0x67: // LD H,A
        LogVerbose("LD H,A");
        cycles = 4;
        R.H = R.A;
        break;
    case 0x6F: // LD L,A
        LogVerbose("LD L,A");
        cycles = 4;
        R.L = R.A;
        break;
    case 0xEA: // LD (nn),A
        LogVerbose("LD (%04X),A", readWord(R.PC));
        cycles = 16;
        writeByte(readWord(R.PC), R.A);
        R.PC += 2;
        break;
    case 0xE0: // LDH (n),A
        LogVerbose("LDH (%02X),A", readByte(R.PC));
        cycles = 12;
        writeByte(0xFF00 + readByte(R.PC++), R.A);
        break;
    case 0xF0: // LDH A,(n)
        LogVerbose("LDH A,(%02X)", readByte(R.PC));
        cycles = 12;
        R.A = readByte(0xFF00 + readByte(R.PC++));
        break;
    case 0x78: // LD A,B
        LogVerbose("LD A,B");
        cycles = 4;
        R.A = R.B;
        break;
    case 0x79: // LD A,C
        LogVerbose("LD A,C");
        cycles = 4;
        R.A = R.C;
        break;
    case 0x7A: // LD A,D
        LogVerbose("LD A,D");
        cycles = 4;
        R.A = R.D;
        break;
    case 0x7B: // LD A,E
        LogVerbose("LD A,E");
        cycles = 4;
        R.A = R.E;
        break;
    case 0x7C: // LD A,H
        LogVerbose("LD A,H");
        cycles = 4;
        R.A = R.H;
        break;
    case 0x7D: // LD A,L
        LogVerbose("LD A,L");
        cycles = 4;
        R.A = R.L;
        break;
    case 0x0A: // LD A,(BC)
        LogVerbose("LD A,(BC)");
        cycles = 8;
        R.A = readByte(R.BC);
        break;
    case 0x1A: // LD A,(DE)
        LogVerbose("LD A,(DE)");
        cycles = 8;
        R.A = readByte(R.DE);
        break;
    case 0x7E: // LD A,(HL)
        LogVerbose("LD A,(HL)");
        cycles = 8;
        R.A = readByte(R.HL);
        break;
    case 0xFA: // LD A,(nn)
        LogVerbose("LD A,(%04X)", readWord(R.PC));
        cycles = 8;
        R.A = readByte(readWord(R.PC));
        R.PC += 2;
        break;
    case 0x3E: // LD A,#
        LogVerbose("LD A,%02X", readByte(R.PC));
        cycles = 8;
        R.A = readByte(R.PC);
        break;
    
    case 0xCB: // Bit Manipulation
        sop = readByte(R.PC++);
        LogVerbose("sop %02X", sop);
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
            cycles = 8;
            break;
        case 0x00: // RLC B
            LogVerbose("RLC B");
            cycles = 8;
            break;
        case 0x01: // RLC C
            LogVerbose("RLC C");
            cycles = 8;
            break;
        case 0x02: // RLC D
            LogVerbose("RLC D");
            cycles = 8;
            break;
        case 0x03: // RLC E
            LogVerbose("RLC E");
            cycles = 8;
            break;
        case 0x04: // RLC H
            LogVerbose("RLC H");
            cycles = 8;
            break;
        case 0x05: // RLC L
            LogVerbose("RLC L");
            cycles = 8;
            break;
        case 0x06: // RLC (HL)
            LogVerbose("RLC (HL)");
            cycles = 16;
            break;

        case 0x17: // RL A
            LogVerbose("RL A");
            cycles = 8;
            break;
        case 0x10: // RL B
            LogVerbose("RL B");
            cycles = 8;
            break;
        case 0x11: // RL C
            LogVerbose("RL C");
            cycles = 8;
            break;
        case 0x12: // RL D
            LogVerbose("RL D");
            cycles = 8;
            break;
        case 0x13: // RL E
            LogVerbose("RL E");
            cycles = 8;
            break;
        case 0x14: // RL H
            LogVerbose("RL H");
            cycles = 8;
            break;
        case 0x15: // RL L
            LogVerbose("RL L");
            cycles = 8;
            break;
        case 0x16: // RL (HL)
            LogVerbose("RL (HL)");
            cycles = 16;
            break;

        case 0x0F: // RRC A
            LogVerbose("RRC A");
            cycles = 8;
            break;
        case 0x08: // RRC B
            LogVerbose("RRC B");
            cycles = 8;
            break;
        case 0x09: // RRC C
            LogVerbose("RRC C");
            cycles = 8;
            break;
        case 0x0A: // RRC D
            LogVerbose("RRC D");
            cycles = 8;
            break;
        case 0x0B: // RRC E
            LogVerbose("RRC E");
            cycles = 8;
            break;
        case 0x0C: // RRC H
            LogVerbose("RRC H");
            cycles = 8;
            break;
        case 0x0D: // RRC L
            LogVerbose("RRC L");
            cycles = 8;
            break;
        case 0x0E: // RRC (HL)
            LogVerbose("RRC (HL)");
            cycles = 16;
            break;
        
        case 0x1F: // RR A
            LogVerbose("RR A");
            cycles = 8;
            break;
        case 0x18: // RR B
            LogVerbose("RR B");
            cycles = 8;
            break;
        case 0x19: // RR C
            LogVerbose("RR C");
            cycles = 8;
            break;
        case 0x1A: // RR D
            LogVerbose("RR D");
            cycles = 8;
            break;
        case 0x1B: // RR E
            LogVerbose("RR E");
            cycles = 8;
            break;
        case 0x1C: // RR H
            LogVerbose("RR H");
            cycles = 8;
            break;
        case 0x1D: // RR L
            LogVerbose("RR L");
            cycles = 8;
            break;
        case 0x1E: // RR (HL)
            LogVerbose("RR (HL)");
            cycles = 16;
            break;
        
        case 0x27: // SLA A
            LogVerbose("SLA A");
            cycles = 8;
            break;
        case 0x20: // SLA B
            LogVerbose("SLA B");
            cycles = 8;
            break;
        case 0x21: // SLA C
            LogVerbose("SLA C");
            cycles = 8;
            break;
        case 0x22: // SLA D
            LogVerbose("SLA D");
            cycles = 8;
            break;
        case 0x23: // SLA E
            LogVerbose("SLA E");
            cycles = 8;
            break;
        case 0x24: // SLA H
            LogVerbose("SLA H");
            cycles = 8;
            break;
        case 0x25: // SLA L
            LogVerbose("SLA L");
            cycles = 8;
            break;
        case 0x26: // SLA (HL)
            LogVerbose("SLA (HL)");
            cycles = 16;
            break;

        case 0x2F: // SRA A
            LogVerbose("SRA A");
            cycles = 8;
            break;
        case 0x28: // SRA B
            LogVerbose("SRA B");
            cycles = 8;
            break;
        case 0x29: // SRA C
            LogVerbose("SRA C");
            cycles = 8;
            break;
        case 0x2A: // SRA D
            LogVerbose("SRA D");
            cycles = 8;
            break;
        case 0x2B: // SRA E
            LogVerbose("SRA E");
            cycles = 8;
            break;
        case 0x2C: // SRA H
            LogVerbose("SRA H");
            cycles = 8;
            break;
        case 0x2D: // SRA L
            LogVerbose("SRA L");
            cycles = 8;
            break;
        case 0x2E: // SRA (HL)
            LogVerbose("SRA (HL)");
            cycles = 16;
            break;
        
        case 0x3F: // SRL A
            LogVerbose("SRL A");
            cycles = 8;
            break;
        case 0x38: // SRL B
            LogVerbose("SRL B");
            cycles = 8;
            break;
        case 0x39: // SRL C
            LogVerbose("SRL C");
            cycles = 8;
            break;
        case 0x3A: // SRL D
            LogVerbose("SRL D");
            cycles = 8;
            break;
        case 0x3B: // SRL E
            LogVerbose("SRL E");
            cycles = 8;
            break;
        case 0x3C: // SRL H
            LogVerbose("SRL H");
            cycles = 8;
            break;
        case 0x3D: // SRL L
            LogVerbose("SRL L");
            cycles = 8;
            break;
        case 0x3E: // SRL (HL)
            LogVerbose("SRL (HL)");
            cycles = 16;
            break;
        
        case 0x47: // BIT b,A
            LogVerbose("BIT b,A");
            cycles = 8;
            break;
        case 0x40: // BIT b,B
            LogVerbose("BIT b,B");
            cycles = 8;
            break;
        case 0x41: // BIT b,C
            LogVerbose("BIT b,C");
            cycles = 8;
            break;
        case 0x42: // BIT b,D
            LogVerbose("BIT b,D");
            cycles = 8;
            break;
        case 0x43: // BIT b,E
            LogVerbose("BIT b,E");
            cycles = 8;
            break;
        case 0x44: // BIT b,H
            LogVerbose("BIT b,H");
            cycles = 8;
            break;
        case 0x45: // BIT b,L
            LogVerbose("BIT b,L");
            cycles = 8;
            break;
        case 0x46: // BIT b,(HL)
            LogVerbose("BIT b,(HL)");
            cycles = 16;
            break;
        
        case 0xC7: // SET b,A
            LogVerbose("SET b,A");
            cycles = 8;
            break;
        case 0xC0: // SET b,B
            LogVerbose("SET b,B");
            cycles = 8;
            break;
        case 0xC1: // SET b,C
            LogVerbose("SET b,C");
            cycles = 8;
            break;
        case 0xC2: // SET b,D
            LogVerbose("SET b,D");
            cycles = 8;
            break;
        case 0xC3: // SET b,E
            LogVerbose("SET b,E");
            cycles = 8;
            break;
        case 0xC4: // SET b,H
            LogVerbose("SET b,H");
            cycles = 8;
            break;
        case 0xC5: // SET b,L
            LogVerbose("SET b,L");
            cycles = 8;
            break;
        case 0xC6: // SET b,(HL)
            LogVerbose("SET b,(HL)");
            cycles = 16;
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
            cycles = 8;
            R.A = R.A & ~(1 << b);
            break;
        case 0x80: // RES b,B
            LogVerbose("RES b,B");
            cycles = 8;
            break;
        case 0x81: // RES b,C
            LogVerbose("RES b,C");
            cycles = 8;
            break;
        case 0x82: // RES b,D
            LogVerbose("RES b,D");
            cycles = 8;
            break;
        case 0x83: // RES b,E
            LogVerbose("RES b,E");
            cycles = 8;
            break;
        case 0x84: // RES b,H
            LogVerbose("RES b,H");
            cycles = 8;
            break;
        case 0x85: // RES b,L
            LogVerbose("RES b,L");
            cycles = 8;
            break;
        case 0x86: // RES b,(HL)
            LogVerbose("RES b,(HL)");
            cycles = 16;
            break;
        }
        break;

    case 0xF5: // PUSH AF
        LogVerbose("PUSH AF");
        cycles = 16;
        R.SP -= 2;
        writeWord(R.SP, R.AF);
        break;
    case 0xC5: // PUSH BC
        LogVerbose("PUSH BC");
        cycles = 16;
        R.SP -= 2;
        writeWord(R.SP, R.BC);
        break;
    case 0xD5: // PUSH DE
        LogVerbose("PUSH DE");
        cycles = 16;
        R.SP -= 2;
        writeWord(R.SP, R.DE);
        break;
    case 0xE5: // PUSH HL
        LogVerbose("PUSH HL");
        cycles = 16;
        R.SP -= 2;
        writeWord(R.SP, R.HL);
        break;

    case 0xF1: // POP AF
        LogVerbose("POP AF");
        cycles = 12;
        R.AF = readWord(R.SP);
        R.SP += 2;
        break;
    case 0xC1: // POP BC
        LogVerbose("POP BC");
        cycles = 12;
        R.BC = readWord(R.SP);
        R.SP += 2;
        break;
    case 0xD1: // POP DE
        LogVerbose("POP DE");
        cycles = 12;
        R.DE = readWord(R.SP);
        R.SP += 2;
        break;
    case 0xE1: // POP HL
        LogVerbose("POP HL");
        cycles = 12;
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
        cycles = 4;
        R.A = add8(R.A, R.A);
        break;
    case 0x80: // ADD A,B
        LogVerbose("ADD A,B");
        cycles = 4;
        R.A = add8(R.A, R.B);
        break;
    case 0x81: // ADD A,C
        LogVerbose("ADD A,C");
        cycles = 4;
        R.A = add8(R.A, R.C);
        break;
    case 0x82: // ADD A,D
        LogVerbose("ADD A,D");
        cycles = 4;
        R.A = add8(R.A, R.D);
        break;
    case 0x83: // ADD A,E
        LogVerbose("ADD A,E");
        cycles = 4;
        R.A = add8(R.A, R.E);
        break;
    case 0x84: // ADD A,H
        LogVerbose("ADD A,H");
        cycles = 4;
        R.A = add8(R.A, R.H);
        break;
    case 0x85: // ADD A,L
        LogVerbose("ADD A,L");
        cycles = 4;
        R.A = add8(R.A, R.L);
        break;
    case 0x86: // ADD A,(HL)
        LogVerbose("ADD A,(HL)");
        cycles = 8;
        R.A = add8(R.A, readByte(R.HL));
        break;
    case 0xC6: // ADD A,#
        LogVerbose("ADD A,#");
        cycles = 8;
        R.A = add8(R.A, readByte(R.PC++));
        break;
    
    case 0x97: // SUB A
        LogVerbose("SUB A");
        cycles = 4;
        R.A = sub8(R.A, R.A);
        break;
    case 0x90: // SUB B
        LogVerbose("SUB B");
        cycles = 4;
        R.A = sub8(R.A, R.B);
        break;
    case 0x91: // SUB C
        LogVerbose("SUB C");
        cycles = 4;
        R.A = sub8(R.A, R.C);
        break;
    case 0x92: // SUB D
        LogVerbose("SUB D");
        cycles = 4;
        R.A = sub8(R.A, R.D);
        break;
    case 0x93: // SUB E
        LogVerbose("SUB E");
        cycles = 4;
        R.A = sub8(R.A, R.E);
        break;
    case 0x94: // SUB H
        LogVerbose("SUB H");
        cycles = 4;
        R.A = sub8(R.A, R.H);
        break;
    case 0x95: // SUB L
        LogVerbose("SUB L");
        cycles = 4;
        R.A = sub8(R.A, R.L);
        break;
    case 0x96: // SUB (HL)
        LogVerbose("SUB (HL)");
        cycles = 8;
        R.A = sub8(R.A, readByte(R.HL));
        break;
    case 0xD6: // SUB #
        LogVerbose("SUB #");
        cycles = 8;
        R.A = sub8(R.A, readByte(R.PC++));
        break;

    case 0xA7: // AND A
        LogVerbose("AND A");
        cycles = 4;
        R.A = and8(R.A, R.A);
        break;
    case 0xA0: // AND B
        LogVerbose("AND B");
        cycles = 4;
        R.A = and8(R.A, R.B);
        break;
    case 0xA1: // AND C
        LogVerbose("AND C");
        cycles = 4;
        R.A = and8(R.A, R.C);
        break;
    case 0xA2: // AND D
        LogVerbose("AND D");
        cycles = 4;
        R.A = and8(R.A, R.D);
        break;
    case 0xA3: // AND E
        LogVerbose("AND E");
        cycles = 4;
        R.A = and8(R.A, R.E);
        break;
    case 0xA4: // AND H
        LogVerbose("AND H");
        cycles = 4;
        R.A = and8(R.A, R.H);
        break;
    case 0xA5: // AND L
        LogVerbose("AND L");
        cycles = 4;
        R.A = and8(R.A, R.L);
        break;
    case 0xA6: // AND (HL)
        LogVerbose("AND (HL)");
        cycles = 8;
        R.A = and8(R.A, readByte(R.HL));
        break;
    case 0xE6: // AND #
        LogVerbose("AND #");
        cycles = 8;
        R.A = and8(R.A, readByte(R.PC++));
        break;
    
    case 0xB7: // OR A
        LogVerbose("OR A");
        cycles = 4;
        R.A = or8(R.A, R.A);
        break;
    case 0xB0: // OR B
        LogVerbose("OR B");
        cycles = 4;
        R.A = or8(R.A, R.B);
        break;
    case 0xB1: // OR C
        LogVerbose("OR C");
        cycles = 4;
        R.A = or8(R.A, R.C);
        break;
    case 0xB2: // OR D
        LogVerbose("OR D");
        cycles = 4;
        R.A = or8(R.A, R.D);
        break;
    case 0xB3: // OR E
        LogVerbose("OR E");
        cycles = 4;
        R.A = or8(R.A, R.E);
        break;
    case 0xB4: // OR H
        LogVerbose("OR H");
        cycles = 4;
        R.A = or8(R.A, R.H);
        break;
    case 0xB5: // OR L
        LogVerbose("OR L");
        cycles = 4;
        R.A = or8(R.A, R.L);
        break;
    case 0xB6: // OR (HL)
        LogVerbose("OR (HL)");
        cycles = 8;
        R.A = or8(R.A, readByte(R.HL));
        break;
    case 0xF6: // OR #
        LogVerbose("OR #");
        cycles = 8;
        R.A = or8(R.A, readByte(R.PC++));
        break;
    
    case 0xAF: // XOR A
        LogVerbose("XOR A");
        cycles = 4;
        R.A = xor8(R.A, R.A);
        break;
    case 0xA8: // XOR B
        LogVerbose("XOR B");
        cycles = 4;
        R.A = xor8(R.A, R.B);
        break;
    case 0xA9: // XOR C
        LogVerbose("XOR C");
        cycles = 4;
        R.A = xor8(R.A, R.C);
        break;
    case 0xAA: // XOR D
        LogVerbose("XOR D");
        cycles = 4;
        R.A = xor8(R.A, R.D);
        break;
    case 0xAB: // XOR E
        LogVerbose("XOR E");
        cycles = 4;
        R.A = xor8(R.A, R.E);
        break;
    case 0xAC: // XOR H
        LogVerbose("XOR H");
        cycles = 4;
        R.A = xor8(R.A, R.H);
        break;
    case 0xAD: // XOR L
        LogVerbose("XOR L");
        cycles = 4;
        R.A = xor8(R.A, R.L);
        break;
    case 0xAE: // XOR (HL)
        LogVerbose("XOR (HL)");
        cycles = 4;
        R.A = xor8(R.A, readByte(R.HL));
        break;
    case 0xEE: // XOR #
        LogVerbose("XOR #");
        cycles = 4;
        R.A = xor8(R.A, readByte(R.PC++));
        break;

    case 0xBF: // CP A
        LogVerbose("CP A");
        cycles = 4;
        sub8(R.A, R.A);
        break;
    case 0xB8: // CP B
        LogVerbose("CP B");
        cycles = 4;
        sub8(R.A, R.B);
        break;
    case 0xB9: // CP C
        LogVerbose("CP C");
        cycles = 4;
        sub8(R.A, R.C);
        break;
    case 0xBA: // CP D
        LogVerbose("CP D");
        cycles = 4;
        sub8(R.A, R.D);
        break;
    case 0xBB: // CP E
        LogVerbose("CP E");
        cycles = 4;
        sub8(R.A, R.E);
        break;
    case 0xBC: // CP H
        LogVerbose("CP H");
        cycles = 4;
        sub8(R.A, R.H);
        break;
    case 0xBD: // CP L
        LogVerbose("CP L");
        cycles = 4;
        sub8(R.A, R.L);
        break;
    case 0xBE: // CP (HL)
        LogVerbose("CP (HL)");
        cycles = 8;
        sub8(R.A, readByte(R.HL));
        break;
    case 0xFE: // CP #
        LogVerbose("CP %02X", readByte(R.PC));
        cycles = 8;
        sub8(R.A, readByte(R.PC++));
        break;
    
    case 0x3C: // INC A
        LogVerbose("INC A");
        cycles = 4;
        R.A = inc8(R.A);
        break;
    case 0x04: // INC B
        LogVerbose("INC B");
        cycles = 4;
        R.B = inc8(R.B);
        break;
    case 0x0C: // INC C
        LogVerbose("INC C");
        cycles = 4;
        R.C = inc8(R.C);
        break;
    case 0x14: // INC D
        LogVerbose("INC D");
        cycles = 4;
        R.D = inc8(R.D);
        break;
    case 0x1C: // INC E
        LogVerbose("INC E");
        cycles = 4;
        R.E = inc8(R.E);
        break;
    case 0x24: // INC H
        LogVerbose("INC H");
        cycles = 4;
        R.H = inc8(R.H);
        break;
    case 0x2C: // INC L
        LogVerbose("INC L");
        cycles = 4;
        R.L = inc8(R.L);
        break;
    case 0x34: // INC 
        LogVerbose("INC (HL)");
        cycles = 12;
        writeByte(R.HL, inc8(readByte(R.HL)));
        break;

    case 0xC3: // JP nn
        LogVerbose("JP %02X", readWord(R.PC));
        cycles = 12;
        R.PC = readWord(R.PC);
        break;
    case 0xC2: // JP NZ,nn
        break;
    case 0xC9: // RET
        LogVerbose("RET");
        cycles = 12;
        R.PC = readWord(R.SP);
        R.SP += 2;
        break;
    case 0xCA: // JP Z,nn
        break;
    case 0xCD: // CALL nn
        LogVerbose("CALL %04X", readWord(R.PC));
        cycles = 12;
        R.SP -= 2;
        writeWord(R.SP, R.PC+2);
        R.PC = readWord(R.PC);
        break;
    case 0xD2: // JP NC,nn
        break;
    case 0xDA: // JP C,nn
        break;
    
    case 0x18: // JR *
        LogVerbose("JR %02X", readByte(R.PC));
        cycles = 8;
        R.PC += (int8_t)readByte(R.PC);
        break;
    case 0x20: // JR NZ,*
        LogVerbose("JR NZ,%02X", readByte(R.PC));
        cycles = 8;
        if (!R.FZ) {
            cycles = 12;
            R.PC += (int8_t)readByte(R.PC);
        }
        break;
    case 0x28: // JR Z,*
        LogVerbose("JR Z,%02X", readByte(R.PC));
        cycles = 8;
        if (R.FZ) {
            cycles = 12;
            R.PC += (int8_t)readByte(R.PC);
        }
        break;
    case 0x30: // JR NC,*
        LogVerbose("JR NC,%02X", readByte(R.PC));
        cycles = 8;
        if (!R.FC) {
            cycles = 12;
            R.PC += (int8_t)readByte(R.PC);
        }
        break;
    case 0x38: // JR C,*
        LogVerbose("JR C,%02X", readByte(R.PC));
        cycles = 8;
        if (R.FC) {
            cycles = 12;
            R.PC += (int8_t)readByte(R.PC);
        }
        break;
    }

    // TODO: delay by cycles
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    if (!loadROM(argv[1])) {
        return 1;
    }

    R.A  = 0x01;
    R.F  = 0b10110000;
    R.PC = 0x0100;
    R.BC = 0x0013;
    R.DE = 0x00D8;
    R.HL = 0x014D;
    R.SP = 0xFFFE;
    printR();

    const int LIMIT = 1000;

    for (int i = 0; i < LIMIT; ++i) {
        tick();
        printR();
        getchar();
    }

    freeROM();

    return 0;
}