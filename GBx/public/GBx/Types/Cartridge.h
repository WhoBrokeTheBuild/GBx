#ifndef GBX_TYPES_CARTRIDGE_H
#define GBX_TYPES_CARTRIDGE_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <GBx/Macros.h>

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        // $0100 - $0103
        uint8_t Code[4];

        // $0104 - $0133
        uint8_t Magic[48];

        // $0134 - $0143
        char Title[15];

        // $0143
        uint8_t ColorEnabled;

        // $0144 - $0145
        uint8_t NewLicenseCode[2];

        // $0146
        uint8_t SuperEnabled;

        // $0147
        uint8_t CartridgeType;

        // $0148
        uint8_t ROMType;

        // $0149
        uint8_t RAMType;

        // $014A
        uint8_t Region;

        // $014B
        uint8_t License;

        // $014C
        uint8_t Version;
        
        // $014D
        uint8_t ComplementCheck;

        // $014E - 014F
        uint8_t Checksum[2];
    });

    // clang-format on

    uint8_t raw[80];

} gbx_cartridge_header_t;

static_assert(sizeof(gbx_cartridge_header_t) == 80,
    "sizeof(gbx_cartridge_header_t) != 80");

typedef enum
{
    GBX_MBC_TYPE_NONE,
    GBX_MBC_TYPE_MBC1,
    GBX_MBC_TYPE_MBC2,
    GBX_MBC_TYPE_MBC3,
    GBX_MBC_TYPE_MBC5,
    GBX_MBC_TYPE_MBC6,
    GBX_MBC_TYPE_MBC7,
    GBX_MBC_TYPE_MMM01,

} gbx_mbc_type_t;

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t Lower:5;
        uint8_t Upper:2;
        uint8_t :1;
    });

    GBX_PACK(struct
    {
        uint8_t Full:7;
        bool RAMMode:1;
    });

    // clang-format on

    uint8_t raw;

} gbx_mbc_bank_control_t;

static_assert(sizeof(gbx_mbc_bank_control_t) == 1,
    "sizeof(gbx_mbc_bank_control_t) != 1");

#endif // GBX_TYPES_CARTRIDGE_H