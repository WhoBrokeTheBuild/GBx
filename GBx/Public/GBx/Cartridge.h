#ifndef GBX_CARTRIDGE_H
#define GBX_CARTRIDGE_H

#include <GBx/GBx.h>

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

void GBx_Cartridge_Init(GBx * ctx);

void GBx_Cartridge_Reset(GBx * ctx);

bool GBx_Cartridge_Load(GBx * ctx, const char * filename);

const char * GBx_Cartridge_GetTypeString(uint8_t type);

const char * GBx_Cartridge_GetROMTypeString(uint8_t type);

const char * GBx_Cartridge_GetSRAMTypeString(uint8_t type);

void GBx_Cartridge_PrintHeader(GBx * ctx);

void GBx_Cartridge_PrintMBC(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_CARTRIDGE_H