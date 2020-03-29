#ifndef MBC_H
#define MBC_H

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
    struct {
        union {
            struct {
                byte Lower:5;
                byte Upper:2;
            };
            byte Full:7;
        };
        bool RAMMode:1;
    };
    byte raw;

} mbc_bank_control_t;

extern mbc_type_t MBCType;

extern mbc_bank_control_t MBCBankControl;

void ResetMBC();

void WriteMBC(word address, byte data);

void PrintMBC();

#endif // MBC_H