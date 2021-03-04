#ifndef GBX_TYPES_CPU_H
#define GBX_TYPES_CPU_H

#include <GBx/Macros.h>

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union GBx_Flags
{
    GBX_PACK(struct {
        uint8_t :4;
        uint8_t C:1; // Carry Flag
        uint8_t H:1; // Half-Carry Flag
        uint8_t N:1; // Subtract Flag
        uint8_t Z:1; // Zero Flag
    });

    uint8_t _raw;

} GBx_Flags;

static_assert(
    sizeof(GBx_Flags) == 1,
    "sizeof(GBx_Flags) != 1"
);

#ifdef __cplusplus
}
#endif

#endif // GBX_TYPES_CPU_H