#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#if defined(_WIN32)
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#else
#include <netinet/in.h>
#endif

void usage() 
{
    printf(
        "usage: romdump FILENAME\n"
        "\n"
    );
}

enum {
    OFF_BEGIN_CODE          = 0x0100,
    OFF_MAGIC_START         = 0x0104,
    OFF_MAGIC_END           = 0x0133,
    OFF_TITLE_START         = 0x0134,
    OFF_TITLE_END           = 0x0142,
    OFF_COLOR_ENABLE        = 0x0143,
    OFF_LICENSE             = 0x0144,
    OFF_SUPER_ENABLE        = 0x0146,
    OFF_CARTRIDGE_TYPE      = 0x0147,
    OFF_ROM_SIZE            = 0x0148,
    OFF_RAM_SIZE            = 0x0149,
    OFF_REGION              = 0x014A,
    OFF_LICENSE_OLD         = 0x014B,
    OFF_ROM_MASK            = 0x014C,
    OFF_COMPLEMENT_CHECK    = 0x014D,
    OFF_CHECKSUM            = 0x014E,
    OFF_HEADER_END          = 0x0150,
};

const uint8_t NINTENDO_MAGIC[] = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
    0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
};

typedef union 
{
    uint8_t data;
} instruction_t;

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    const char * filename = argv[1];
    FILE * file = fopen(filename, "rb");

    uint8_t header[OFF_HEADER_END];
    size_t bytesRead = 0;
    instruction_t inst;

    bytesRead = fread(header, 1, sizeof(header), file);
    if (bytesRead < sizeof(header)) {
        return 1;
    }
    
    bytesRead = fread((void *)&inst, 1, sizeof(inst), file);

    printf("start code:\t\t%02X %02X %02X %02X\n", header[OFF_BEGIN_CODE], header[OFF_BEGIN_CODE+1], header[OFF_BEGIN_CODE+2], header[OFF_BEGIN_CODE+3]);

    printf("nintendo graphic:\t");
    if (0 == memcmp(header + OFF_MAGIC_START, NINTENDO_MAGIC, sizeof(NINTENDO_MAGIC))) {
        printf("valid\n");
    } else {
        printf("invalid\n");
    }

    printf("title:\t\t\t%.*s\n", OFF_TITLE_END - OFF_TITLE_START, header + OFF_TITLE_START);

    printf("color supported:\t");
    if (header[OFF_COLOR_ENABLE] == 0x80) {
        printf("yes\n");
    } else {
        printf("no\n");
    }

    printf("functionality:\t\t");
    if (header[OFF_SUPER_ENABLE] == 0x00) {
        printf("GameBoy\n");
    } else {
        printf("Super GameBoy\n");
    }

    printf("cartridge type:\t\t");
    switch (header[OFF_CARTRIDGE_TYPE]) {
    case 0x00:
        printf("ROM ONLY\n");
        break;
    case 0x01:
        printf("ROM+MBC1\n");
        break;
    case 0x02:
        printf("ROM+MBC1+RAM\n");
        break;
    case 0x03:
        printf("ROM+MBC1+RAM+BATT\n");
        break;
    case 0x05:
        printf("ROM+MBC2\n");
        break;
    case 0x06:
        printf("ROM+MBC2+BATT\n");
        break;
    case 0x08:
        printf("ROM+RAM\n");
        break;
    case 0x09:
        printf("ROM+RAM+BATT\n");
        break;
    case 0x0B:
        printf("ROM+MMM01\n");
        break;
    case 0x0C:
        printf("ROM+MMM01+SRAM\n");
        break;
    case 0x0D:
        printf("ROM+MMM01+SRAM+BATT\n");
        break;
    case 0x0F:
        printf("ROM+MBC3+TIMER+BATT\n");
        break;
    case 0x10:
        printf("ROM+MBC3+TIMER+RAM+BATT\n");
        break;
    case 0x11:
        printf("ROM+MBC3\n");
        break;
    case 0x12:
        printf("ROM+MBC3+RAM\n");
        break;
    case 0x13:
        printf("ROM+MBC3+RAM+BATT\n");
        break;
    case 0x19:
        printf("ROM+MBC5\n");
        break;
    case 0x1A:
        printf("ROM+MBC5+RAM\n");
        break;
    case 0x1B:
        printf("ROM+MBC5+RAM+BATT\n");
        break;
    case 0x1C:
        printf("ROM+MBC5+RUMBLE\n");
        break;
    case 0x1D:
        printf("ROM+MBC5+RUMBLE+SRAM\n");
        break;
    case 0x1E:
        printf("ROM+MBC5+RUMBLE+SRAM+BATT\n");
        break;
    case 0x1F:
        printf("Pocket Camera\n");
        break;
    case 0xFD:
        printf("Bandai TAMA5\n");
        break;
    case 0xFE:
        printf("Hudson HuC-3\n");
        break;
    case 0xFF:
        printf("Hudson HuC-1\n");
        break;
    }

    printf("ROM size:\t\t");
    switch (header[OFF_ROM_SIZE]) {
    case 0x00:
        printf("256Kb\t32KB\t2 banks\n");
        break;
    case 0x01:
        printf("512Kb\t64KB\t4 banks\n");
        break;
    case 0x02:
        printf("1Mb\t128KB\t8 banks\n");
        break;
    case 0x03:
        printf("2Mb\t256KB\t16 banks\n");
        break;
    case 0x04:
        printf("4Mb\t512KB\t32 banks\n");
        break;
    case 0x05:
        printf("8Mb\t1MB\t64 banks\n");
        break;
    case 0x06:
        printf("16Mb\t2MB\t128 banks\n");
        break;
    case 0x52:
        printf("9Mb\t1.1MB\t72 banks\n");
        break;
    case 0x53:
        printf("10Mb\t1.2MB\t80 banks\n");
        break;
    case 0x54:
        printf("12Mb\t1.5MB\t96 banks\n");
        break;
    }

    printf("RAM size:\t\t");
    switch (header[OFF_RAM_SIZE]) {
    case 0x00:
        printf("None\n");
        break;
    case 0x01:
        printf("16Kb\t2KB\t1 bank\n");
        break;
    case 0x02:
        printf("64Kb\t8KB\t1 bank\n");
        break;
    case 0x03:
        printf("256Kb\t32KB\t4 bank\n");
        break;
    case 0x04:
        printf("1Mb\t128KB\t16 bank\n");
        break;
    }

    printf("country:\t\t");
    switch (header[OFF_REGION]) {
    case 0:
        printf("japan\n");
        break;
    case 1:
        printf("not japan\n");
        break;
    }

    printf("checksum:\t\t%d %d\n", header[OFF_CHECKSUM+1], header[OFF_CHECKSUM]);

    fclose(file);

    return 0;
}