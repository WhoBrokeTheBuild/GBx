#ifndef MBC_H
#define MBC_H

#include <GBx/Macros.h>
#include <GBx/Types.h>

typedef enum
{
    MBC_TYPE_NONE,
    MBC_TYPE_MBC1,
    MBC_TYPE_MBC2,
    MBC_TYPE_MBC3,
    MBC_TYPE_MBC5,
    MBC_TYPE_MBC6,
    MBC_TYPE_MBC7,
    MBC_TYPE_MMM01,

} mbc_type_t;

typedef struct
{
    GBX_PACK(struct {
        uint8_t Lower : 5;
        uint8_t Upper : 2;
    uint8_t:
        1;
    });

    GBX_PACK(struct {
        uint8_t Full : 7;
        bool    RAMMode : 1;
    });

    uint8_t raw;

} mbc_bank_control_t;

extern mbc_type_t MBCType;

extern mbc_bank_control_t MBCBankControl;

void ResetMBC();

void WriteMBC(uint16_t address, uint8_t data);

void PrintMBC();

#endif // MBC_H