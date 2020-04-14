#ifndef SM83_INTERRUPTS_H
#define SM83_INTERRUPTS_H

#include <SM83/Types.h>

typedef struct sm83 sm83_t;

void SM83_CallInterrupt(sm83_t * ctx, uint16_t addr);

void SM83_CheckInterrupts(sm83_t * ctx);

void SM83_PrintInterrupts(sm83_t * ctx);

#endif // SM83_INTERRUPTS_H