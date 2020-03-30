#include <GBx/Disassembler.h>

#include <GBx/MMU.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// $w = unsigned word
// $u = unsigned byte
// $s = signed byte

const char * instructionDefinitions[] = {
    [0xCB] = "",
    // misc
    [0x00] = "NOP",
    [0x76] = "HALT",
    [0x10] = "STOP",
    [0xF3] = "DI",
    [0xFB] = "EI",
    [0x27] = "DAA",
    [0x37] = "SCF",
    [0x2F] = "CPL",
    [0x3F] = "CCF",
    // call
    [0xCD] = "CALL $w",
    [0xC4] = "CALL NZ, $w",
    [0xCC] = "CALL Z, $w",
    [0xD4] = "CALL NC, $w",
    [0xDC] = "CALL C, $w",
    // restart
    [0xC7] = "RST 00",
    [0xCF] = "RST 08",
    [0xD7] = "RST 10",
    [0xDF] = "RST 18",
    [0xE7] = "RST 20",
    [0xEF] = "RST 28",
    [0xF7] = "RST 30",
    [0xFF] = "RST 38",
    // return
    [0xC9] = "RET",
    [0xC0] = "RET NZ",
    [0xC8] = "RET Z",
    [0xD0] = "RET NC",
    [0xD8] = "RET C",
    [0xD9] = "RETI",
    // rotate
    [0x07] = "RLCA",
    [0x0F] = "RRCA",
    [0x17] = "RLA",
    [0x1F] = "RRA",
    // jump
    [0xC3] = "JP $w",
    [0xC2] = "JP NZ, $w",
    [0xCA] = "JP Z, $w",
    [0xD2] = "JP NC, $w",
    [0xDA] = "JP C, $w",
    [0x18] = "JR $s",
    [0x20] = "JR NZ, $s",
    [0x28] = "JR Z, $s",
    [0x30] = "JR NC, $s",
    [0x38] = "JR C, $s",
    [0xE9] = "JP HL",
    // stack
    [0xF5] = "PUSH AF",
    [0xC5] = "PUSH BC",
    [0xD5] = "PUSH DE",
    [0xE5] = "PUSH HL",
    [0xF1] = "POP AF",
    [0xC1] = "POP BC",
    [0xD1] = "POP DE",
    [0xE1] = "POP HL",
    // load
    [0x40] = "LD B, B",
    [0x41] = "LD B, C",
    [0x42] = "LD B, D",
    [0x43] = "LD B, E",
    [0x44] = "LD B, H",
    [0x45] = "LD B, L",
    [0x46] = "LD B, (HL)",
    [0x47] = "LD B, A",
    [0x48] = "LD C, B",
    [0x49] = "LD C, C",
    [0x4A] = "LD C, D",
    [0x4B] = "LD C, E",
    [0x4C] = "LD C, H",
    [0x4D] = "LD C, L",
    [0x4E] = "LD C, (HL)",
    [0x4F] = "LD C, A",
    [0x50] = "LD D, B",
    [0x51] = "LD D, C",
    [0x52] = "LD D, D",
    [0x53] = "LD D, E",
    [0x54] = "LD D, H",
    [0x55] = "LD D, L",
    [0x56] = "LD D, (HL)",
    [0x57] = "LD D, A",
    [0x58] = "LD E, B",
    [0x59] = "LD E, C",
    [0x5A] = "LD E, D",
    [0x5B] = "LD E, E",
    [0x5C] = "LD E, H",
    [0x5D] = "LD E, L",
    [0x5E] = "LD E, (HL)",
    [0x5F] = "LD E, A",
    [0x60] = "LD H, B",
    [0x61] = "LD H, C",
    [0x62] = "LD H, D",
    [0x63] = "LD H, E",
    [0x64] = "LD H, H",
    [0x65] = "LD H, L",
    [0x66] = "LD H, (HL)",
    [0x67] = "LD H, A",
    [0x68] = "LD L, B",
    [0x69] = "LD L, C",
    [0x6A] = "LD L, D",
    [0x6B] = "LD L, E",
    [0x6C] = "LD L, H",
    [0x6D] = "LD L, L",
    [0x6E] = "LD L, (HL)",
    [0x6F] = "LD L, A",
    [0x70] = "LD (HL), B",
    [0x71] = "LD (HL), C",
    [0x72] = "LD (HL), D",
    [0x73] = "LD (HL), E",
    [0x74] = "LD (HL), H",
    [0x75] = "LD (HL), L",
    [0x77] = "LD (HL), A",
    [0x78] = "LD A, B",
    [0x79] = "LD A, C",
    [0x7A] = "LD A, D",
    [0x7B] = "LD A, E",
    [0x7C] = "LD A, H",
    [0x7D] = "LD A, L",
    [0x7E] = "LD A, (HL)",
    [0x7F] = "LD A, A",
    [0x02] = "LD (BC), A",
    [0x12] = "LD (DE), A",
    [0x06] = "LD B, $u",
    [0x0E] = "LD C, $u",
    [0x16] = "LD D, $u",
    [0x1E] = "LD E, $u",
    [0x26] = "LD H, $u",
    [0x2E] = "LD L, $u",
    [0x36] = "LD (HL), $u",
    [0x3E] = "LD A, $u",
    [0x0A] = "LD A, (BC)",
    [0x1A] = "LD A, (DE)",
    [0x01] = "LD BC, $w",
    [0x11] = "LD DE, $w",
    [0x21] = "LD HL, $w",
    [0x31] = "LD SP, $w",
    [0xF9] = "LD SP, HL",
    [0x08] = "LD ($w), SP",
    [0xEA] = "LD ($w), A",
    [0xFA] = "LD A, ($w)",
    [0x22] = "LDI (HL), A",
    [0x32] = "LDD (HL), A",
    [0x2A] = "LDI A, (HL)",
    [0x3A] = "LDD A, (HL)",
    [0xE0] = "LDH ($u), A",
    [0xF0] = "LDH A, ($u)",
    [0xE2] = "LDH (C), A",
    [0xF2] = "LDH A, (C)",
    [0xF8] = "LD HL, SP+$s",
    // add
    [0x80] = "ADD B",
    [0x81] = "ADD C",
    [0x82] = "ADD D",
    [0x83] = "ADD E",
    [0x84] = "ADD H",
    [0x85] = "ADD L",
    [0x86] = "ADD (HL)",
    [0x87] = "ADD A",
    [0xC6] = "ADD $u",
    [0x88] = "ADC B",
    [0x89] = "ADC C",
    [0x8A] = "ADC D",
    [0x8B] = "ADC E",
    [0x8C] = "ADC H",
    [0x8D] = "ADC L",
    [0x8E] = "ADC (HL)",
    [0x8F] = "ADC A",
    [0xCE] = "ADC $u",
    [0x09] = "ADD HL, BC",
    [0x19] = "ADD HL, DE",
    [0x29] = "ADD HL, HL",
    [0x39] = "ADD HL, SP",
    [0xE8] = "ADD SP, $s",
    // subtract
    [0x90] = "SUB B",
    [0x91] = "SUB C",
    [0x92] = "SUB D",
    [0x93] = "SUB E",
    [0x94] = "SUB H",
    [0x95] = "SUB L",
    [0x96] = "SUB (HL)",
    [0x97] = "SUB A",
    [0xD6] = "SUB $u",
    [0x98] = "SBC B",
    [0x99] = "SBC C",
    [0x9A] = "SBC D",
    [0x9B] = "SBC E",
    [0x9C] = "SBC H",
    [0x9D] = "SBC L",
    [0x9E] = "SBC (HL)",
    [0x9F] = "SBC A",
    [0xDE] = "SBC $u",
    // inc
    [0x04] = "INC B",
    [0x0C] = "INC C",
    [0x14] = "INC D",
    [0x1C] = "INC E",
    [0x24] = "INC H",
    [0x2C] = "INC L",
    [0x34] = "INC (HL)",
    [0x3C] = "INC A",
    [0x03] = "INC BC",
    [0x13] = "INC DE",
    [0x23] = "INC HL",
    [0x33] = "INC SP",
    // dec
    [0x05] = "DEC B",
    [0x0D] = "DEC C",
    [0x15] = "DEC D",
    [0x1D] = "DEC E",
    [0x25] = "DEC H",
    [0x2D] = "DEC L",
    [0x35] = "DEC (HL)",
    [0x3D] = "DEC A",
    [0x0B] = "DEC BC",
    [0x1B] = "DEC DE",
    [0x2B] = "DEC HL",
    [0x3B] = "DEC SP",
    // compare
    [0xB8] = "CP B",
    [0xB9] = "CP C",
    [0xBA] = "CP D",
    [0xBB] = "CP E",
    [0xBC] = "CP H",
    [0xBD] = "CP L",
    [0xBE] = "CP (HL)",
    [0xBF] = "CP A",
    [0xFE] = "CP $u",
    // and
    [0xA0] = "AND B",
    [0xA1] = "AND C",
    [0xA2] = "AND D",
    [0xA3] = "AND E",
    [0xA4] = "AND H",
    [0xA5] = "AND L",
    [0xA6] = "AND (HL)",
    [0xA7] = "AND A",
    [0xE6] = "AND $u",
    // or
    [0xB0] = "OR B",
    [0xB1] = "OR C",
    [0xB2] = "OR D",
    [0xB3] = "OR E",
    [0xB4] = "OR H",
    [0xB5] = "OR L",
    [0xB6] = "OR (HL)",
    [0xB7] = "OR A",
    [0xF6] = "OR $u",
    // xor
    [0xA8] = "XOR B",
    [0xA9] = "XOR C",
    [0xAA] = "XOR D",
    [0xAB] = "XOR E",
    [0xAC] = "XOR H",
    [0xAD] = "XOR L",
    [0xAE] = "XOR (HL)",
    [0xAF] = "XOR A",
    [0xEE] = "XOR $u",
    // null
    [0xD3] = "Not Imp.",
    [0xDB] = "Not Imp.",
    [0xDD] = "Not Imp.",
    [0xE3] = "Not Imp.",
    [0xE4] = "Not Imp.",
    [0xEB] = "Not Imp.",
    [0xEC] = "Not Imp.",
    [0xED] = "Not Imp.",
    [0xF4] = "Not Imp.",
    [0xFC] = "Not Imp.",
    [0xFD] = "Not Imp.",
};

const char * CB_instructionDefinitions[] = {
    // rotate
    [0x00] = "RLC B",
    [0x01] = "RLC C",
    [0x02] = "RLC D",
    [0x03] = "RLC E",
    [0x04] = "RLC H",
    [0x05] = "RLC L",
    [0x06] = "RLC (HL)",
    [0x07] = "RLC A",
    [0x08] = "RRC B",
    [0x09] = "RRC C",
    [0x0A] = "RRC D",
    [0x0B] = "RRC E",
    [0x0C] = "RRC H",
    [0x0D] = "RRC L",
    [0x0E] = "RRC (HL)",
    [0x0F] = "RRC A",
    [0x10] = "RL B",
    [0x11] = "RL C",
    [0x12] = "RL D",
    [0x13] = "RL E",
    [0x14] = "RL H",
    [0x15] = "RL L",
    [0x16] = "RL (HL)",
    [0x17] = "RL A",
    [0x18] = "RR B",
    [0x19] = "RR C",
    [0x1A] = "RR D",
    [0x1B] = "RR E",
    [0x1C] = "RR H",
    [0x1D] = "RR L",
    [0x1E] = "RR (HL)",
    [0x1F] = "RR A",
    // shift
    [0x20] = "SLA B",
    [0x21] = "SLA C",
    [0x22] = "SLA D",
    [0x23] = "SLA E",
    [0x24] = "SLA H",
    [0x25] = "SLA L",
    [0x26] = "SLA (HL)",
    [0x27] = "SLA A",
    [0x28] = "SRA B",
    [0x29] = "SRA C",
    [0x2A] = "SRA D",
    [0x2B] = "SRA E",
    [0x2C] = "SRA H",
    [0x2D] = "SRA L",
    [0x2E] = "SRA (HL)",
    [0x2F] = "SRA A",
    [0x30] = "SWAP B",
    [0x31] = "SWAP C",
    [0x32] = "SWAP D",
    [0x33] = "SWAP E",
    [0x34] = "SWAP H",
    [0x35] = "SWAP L",
    [0x36] = "SWAP (HL)",
    [0x37] = "SWAP A",
    [0x38] = "SRL B",
    [0x39] = "SRL C",
    [0x3A] = "SRL D",
    [0x3B] = "SRL E",
    [0x3C] = "SRL H",
    [0x3D] = "SRL L",
    [0x3E] = "SRL (HL)",
    [0x3F] = "SRL A",
};

// $b = bit

const char * bit_instructionDefinitions[] = {
    "BIT $b, B",
    "BIT $b, C",
    "BIT $b, D",
    "BIT $b, E",
    "BIT $b, H",
    "BIT $b, L",
    "BIT $b, (HL)",
    "BIT $b, A",
    "RES $b, B",
    "RES $b, C",
    "RES $b, D",
    "RES $b, E",
    "RES $b, H",
    "RES $b, L",
    "RES $b, (HL)",
    "RES $b, A",
    "SET $b, B",
    "SET $b, C",
    "SET $b, D",
    "SET $b, E",
    "SET $b, H",
    "SET $b, L",
    "SET $b, (HL)",
    "SET $b, A",
};

word Disassemble(char * str, size_t len, word addr)
{
    int bit = 0;
    const char * pch;
    
    byte op = ReadByte(addr);
    ++addr;
    
    if (op == 0xCB) {
        op = ReadByte(addr);
        ++addr;
        
        if (op < 0x40) {
            pch = CB_instructionDefinitions[op];
        }
        else {
            byte tmp = op - 0x40;
            // 0-7
            bit = (tmp % 8) + (tmp / 16);
            // 0-3
            int inst = tmp / 64;
            // 0-7
            int variant = tmp % 8;
            pch = bit_instructionDefinitions[(inst * 8) + variant];
        }
    }
    else {
        pch = instructionDefinitions[op];
    }
    
    unsigned i = 0;
    while (i < len - 1) {
        if (*pch == '$') {
            ++pch;
            switch (*pch) {
            case 'w': // unsigned word
                i += snprintf(str + i, len - i, "$%04X", ReadWord(addr));
                addr += 2;
                break;
            case 'u': // unsigned byte
                i += snprintf(str + i, len - i, "$%02X", ReadByte(addr));
                ++addr;
                break;
            case 's': // signed byte
                i += snprintf(str + i, len - i, "%d", (sbyte)ReadByte(addr));
                ++addr;
                break;
            case 'b': // bit, for CB instructions
                i += snprintf(str + i, len - i, "%d", bit);
                ++addr;
                break;
            }
            ++pch;
        }
        else {
            str[i] = *pch;
            if (*pch == '\0') {
                break;
            }
            ++i;
            ++pch;
        }
    }

    return addr;
}