#include <GBx/Joypad.h>
#include <GBx/Context.h>

#include "Log.h"

void GBx_ResetJoypad(gbx_t * ctx)
{
    ctx->JOYP.raw = 0xFF;
    
    ctx->RightPressed = false;
    ctx->LeftPressed = false;
    ctx->UpPressed = false;
    ctx->DownPressed = false;
    ctx->APressed = false;
    ctx->BPressed = false;
    ctx->SelectPressed = false;
    ctx->StartPressed = false;
}

void GBx_UpdateJoypad(gbx_t * ctx)
{
    ctx->JOYP.InputStates = 0xF;

    if (!ctx->JOYP.SelectButtons) {
        Verbose(ctx, 2, LogInfo("Updating Buttons"));

        ctx->JOYP.A &= !ctx->APressed;
        ctx->JOYP.B &= !ctx->BPressed;
        ctx->JOYP.Select &= !ctx->SelectPressed;
        ctx->JOYP.Start &= !ctx->StartPressed;
    }

    if (!ctx->JOYP.SelectDPad) {
        Verbose(ctx, 2, LogInfo("Updating DPad"));

        ctx->JOYP.Right &= !ctx->RightPressed;
        ctx->JOYP.Left &= !ctx->LeftPressed;
        ctx->JOYP.Up &= !ctx->UpPressed;
        ctx->JOYP.Down &= !ctx->DownPressed;
    }
}

void GBx_PrintJoypad(gbx_t * ctx)
{
    LogInfo("JOYP: DPad=%d Buttons=%d Right/A=%d Left/B=%d Up/Select=%d Down/Start=%d",
        ctx->JOYP.SelectDPad,
        ctx->JOYP.SelectButtons,
        ctx->JOYP.Right,
        ctx->JOYP.Left,
        ctx->JOYP.Up,
        ctx->JOYP.Down);
}