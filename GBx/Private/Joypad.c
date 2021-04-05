#include <GBx/Joypad.h>
#include <GBx/Context.h>

void GBx_Joypad_Reset(GBx * ctx)
{
    ctx->JOYP._raw = 0xFF;
}