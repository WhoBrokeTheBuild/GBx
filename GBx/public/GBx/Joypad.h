#ifndef GBX_JOYPAD_H
#define GBX_JOYPAD_H

#include <GBx/Types.h>

typedef struct gbx gbx_t;

void GBx_ResetJoypad(gbx_t * ctx);

void GBx_UpdateJoypad(gbx_t * ctx);

void GBx_PrintJoypad(gbx_t * ctx);

#endif // GBX_JOYPAD_H