#ifndef GBX_MBC_H
#define GBX_MBC_H

#include <GBx/GBx.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void GBx_MBC_WriteByte(GBx * ctx, uint16_t address, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif // GBX_MBC_H