#ifndef GBX_INTERRUPT_H
#define GBX_INTERRUPT_H

#include <GBx/GBx.h>
#include <GBx/Types/Interrupt.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void GBx_CallInterrupt(GBx * ctx, uint16_t addr);

void GBx_CheckInterrupts(GBx * ctx);

#ifdef __cplusplus
}
#endif

#endif // GBX_INTERRUPT_H