#include <GBx/Serial.h>
#include <GBx/Context.h>

void GBx_ResetSerial(gbx_t * ctx)
{
    ctx->SB = 0;
    ctx->SC.raw = 0x00;
}