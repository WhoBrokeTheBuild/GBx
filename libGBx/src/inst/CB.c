#include "CB.h"

#include "bit.h"
#include "reset.h"
#include "rotate.h"
#include "set.h"
#include "shift.h"
#include "swap.h"

#include "cpu.h"

typedef void(* inst_t)();
typedef void(* bit_inst_t)(int);

inst_t CB_instructions[] = {
    // rotate
    [0x00] = _RLC_B,
    [0x01] = _RLC_C,
    [0x02] = _RLC_D,
    [0x03] = _RLC_E,
    [0x04] = _RLC_H,
    [0x05] = _RLC_L,
    [0x06] = _RLC_pHL,
    [0x07] = _RLC_A,
    [0x08] = _RRC_B,
    [0x09] = _RRC_C,
    [0x0A] = _RRC_D,
    [0x0B] = _RRC_E,
    [0x0C] = _RRC_H,
    [0x0D] = _RRC_L,
    [0x0E] = _RRC_pHL,
    [0x0F] = _RRC_A,
    [0x10] = _RL_B,
    [0x11] = _RL_C,
    [0x12] = _RL_D,
    [0x13] = _RL_E,
    [0x14] = _RL_H,
    [0x15] = _RL_L,
    [0x16] = _RL_pHL,
    [0x17] = _RL_A,
    [0x18] = _RR_B,
    [0x19] = _RR_C,
    [0x1A] = _RR_D,
    [0x1B] = _RR_E,
    [0x1C] = _RR_H,
    [0x1D] = _RR_L,
    [0x1E] = _RR_pHL,
    [0x1F] = _RR_A,
    // shift
    [0x20] = _SLA_B,
    [0x21] = _SLA_C,
    [0x22] = _SLA_D,
    [0x23] = _SLA_E,
    [0x24] = _SLA_H,
    [0x25] = _SLA_L,
    [0x26] = _SLA_pHL,
    [0x27] = _SLA_A,
    [0x28] = _SRA_B,
    [0x29] = _SRA_C,
    [0x2A] = _SRA_D,
    [0x2B] = _SRA_E,
    [0x2C] = _SRA_H,
    [0x2D] = _SRA_L,
    [0x2E] = _SRA_pHL,
    [0x2F] = _SRA_A,
    [0x30] = _SWAP_B,
    [0x31] = _SWAP_C,
    [0x32] = _SWAP_D,
    [0x33] = _SWAP_E,
    [0x34] = _SWAP_H,
    [0x35] = _SWAP_L,
    [0x36] = _SWAP_pHL,
    [0x37] = _SWAP_A,
    [0x38] = _SRL_B,
    [0x39] = _SRL_C,
    [0x3A] = _SRL_D,
    [0x3B] = _SRL_E,
    [0x3C] = _SRL_H,
    [0x3D] = _SRL_L,
    [0x3E] = _SRL_pHL,
    [0x3F] = _SRL_A,
};

bit_inst_t bit_instructions[] = {
    _BIT_b_B,
    _BIT_b_C,
    _BIT_b_D,
    _BIT_b_E,
    _BIT_b_H,
    _BIT_b_L,
    _BIT_b_pHL,
    _BIT_b_A,
    _RES_b_B,
    _RES_b_C,
    _RES_b_D,
    _RES_b_E,
    _RES_b_H,
    _RES_b_L,
    _RES_b_pHL,
    _RES_b_A,
    _SET_b_B,
    _SET_b_C,
    _SET_b_D,
    _SET_b_E,
    _SET_b_H,
    _SET_b_L,
    _SET_b_pHL,
    _SET_b_A,
};

void _CB()
{
    byte op = Fetch();
    if (op < 0x40) {
        CB_instructions[op]();
    } else {
        byte tmp = op - 0x40;
        // 0-7
        int bit = (tmp % 8) + (tmp / 16);
        // 0-3
        int inst = tmp / 64;
        // 0-7
        int variant = tmp % 8;
        bit_instructions[(inst * 8) + variant](bit);
    }
}