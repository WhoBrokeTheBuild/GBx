#ifndef GBX_CARTRIDGE_H
#define GBX_CARTRIDGE_H

#include <GBx/GBx.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

union GBx_CartridgeHeader
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

};

typedef union GBx_CartridgeHeader GBx_CartridgeHeader;

static_assert(
    sizeof(GBx_CartridgeHeader) == 80,
    "sizeof(GBx_CartridgeHeader) != 80"
);

// Memory Bank Controller
union GBx_MBC
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

};

typedef union GBx_MBC GBx_MBC;

static_assert(
    sizeof(GBx_MBC) == 1,
    "sizeof(GBx_MBC) != 1"
);

enum GBx_MBCType
{
    GBX_MBC_TYPE_NONE,
    GBX_MBC_TYPE_MBC1,
    GBX_MBC_TYPE_MBC2,
    GBX_MBC_TYPE_MBC3,
    GBX_MBC_TYPE_MBC5,
    GBX_MBC_TYPE_MBC6,
    GBX_MBC_TYPE_MBC7,
    GBX_MBC_TYPE_MMM01,

};

typedef enum GBx_MBCType GBx_MBCType;

#define GBX_ROM_BANK_COUNT      (512)
#define GBX_ROM_BANK_SIZE       (0x4000)

#define GBX_SRAM_BANK_COUNT     (8)
#define GBX_SRAM_BANK_SIZE      (0x2000)

#define GBX_ROM_HEADER_OFFSET   (0x0100)

struct GBx_Cartridge
{
    GBx_CartridgeHeader * Header;

    bool HaveSRAM;

    bool HaveBattery;

    bool HaveTimer;

    bool SRAMEnabled;

    GBx_MBCType MBCType;

    GBx_MBC MBC;

    uint8_t ROM[GBX_ROM_BANK_COUNT][GBX_ROM_BANK_SIZE];

    unsigned ROMBank;

    // Static RAM
    uint8_t SRAM[GBX_SRAM_BANK_COUNT][GBX_SRAM_BANK_SIZE];

    unsigned SRAMBank;
};

typedef struct GBx_Cartridge GBx_Cartridge;

GBx_Cartridge * GBx_GetCartridge(GBx * ctx);

bool GBx_Cartridge_Load(GBx * ctx, const char * filename);

void GBx_Cartridge_Reset(GBx * ctx);

void GBx_Cartridge_MBCWriteByte(GBx * ctx, uint16_t address, uint8_t data);

const char * GBx_Cartridge_GetTypeString(uint8_t type);

const char * GBx_Cartridge_GetROMTypeString(uint8_t type);

const char * GBx_Cartridge_GetSRAMTypeString(uint8_t type);

void GBx_Cartridge_PrintHeader(GBx * ctx);

void GBx_Cartridge_PrintMBC(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_CARTRIDGE_H