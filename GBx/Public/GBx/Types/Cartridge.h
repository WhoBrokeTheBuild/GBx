#ifndef GBX_TYPES_CARTRIDGE_H
#define GBX_TYPES_CARTRIDGE_H

#include <GBx/Macros.h>

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union GBx_CartridgeHeader
{
    GBX_PACK(struct {
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
        uint8_t SRAMType;

        // $014A
        uint8_t Region;

        // $014B
        uint8_t LicenseCode;

        // $014C
        uint8_t Version;

        // $014D
        uint8_t ComplementCheck;

        // $014E - $014F
        uint8_t Checksum[2];
    });
    
    uint8_t _raw[80];

} GBx_CartridgeHeader;

static_assert(
    sizeof(GBx_CartridgeHeader) == 80,
    "sizeof(GBx_CartridgeHeader) != 80"
);

typedef enum GBx_MBCType
{
    GBX_MBC_TYPE_NONE,
    GBX_MBC_TYPE_MBC1,
    GBX_MBC_TYPE_MBC2,
    GBX_MBC_TYPE_MBC3,
    GBX_MBC_TYPE_MBC5,
    GBX_MBC_TYPE_MBC6,
    GBX_MBC_TYPE_MBC7,
    GBX_MBC_TYPE_MMM01,

} GBx_MBCType;

typedef union GBx_MBC
{
    GBX_PACK(struct {
        uint8_t Lower:5;
        uint8_t Upper:2;
        uint8_t :1;
    });

    GBX_PACK(struct {
        uint8_t Full:7;
        uint8_t Mode:1;
    });

    uint8_t _raw;

} GBx_MBC;

static_assert(
    sizeof(GBx_MBC) == 1,
    "sizeof(GBx_MBC) != 1"
);

#ifdef __cplusplus
}
#endif

#endif // GBX_TYPES_CARTRIDGE_H