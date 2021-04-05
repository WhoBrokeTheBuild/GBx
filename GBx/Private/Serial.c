#include <GBx/Serial.h>
#include <GBx/Context.h>

void GBx_Serial_Init(GBx * ctx)
{
    ctx->SerialReadCallback = NULL;
    ctx->SerialWriteCallback = NULL;
    ctx->SerialUserData = NULL;

    GBx_Serial_Reset(ctx);
}

void GBx_Serial_Reset(GBx * ctx)
{
    ctx->SC._raw = 0x00;
    ctx->SB = 0x00;
}

void GBx_Serial_Tick(GBx * ctx, unsigned cycles)
{
    if (ctx->SC.TransferStart) {
        uint8_t incoming = 0x00;
        if (ctx->SerialWriteCallback) {
            incoming = ctx->SerialWriteCallback(ctx->SerialUserData);
        }

        if (ctx->SerialReadCallback) {
            ctx->SerialReadCallback(ctx->SerialUserData, ctx->SB);
        }

        ctx->SB = incoming;
        ctx->SC.TransferStart = 0;

        // Trigger Transfer Complete Interrupt
        ctx->IF.Int58 = 1;
    }
}

void GBx_Serial_SetCallbacks(GBx * ctx,
    GBx_SerialReadCallback readCallback, 
    GBx_SerialWriteCallback writeCallback, 
    void * userData
)
{
    ctx->SerialReadCallback = readCallback;
    ctx->SerialWriteCallback = writeCallback;
    ctx->SerialUserData = userData;
}
