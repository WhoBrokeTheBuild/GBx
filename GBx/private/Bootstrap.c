#include <GBx/Bootstrap.h>

#include "Internal.h"
#include "Log.h"

bool GBx_LoadBootstrap(gbx_t * ctx, const char * filename)
{
    const size_t DMG_BOOT_ROM_SIZE = 0x100;
    const size_t CGB_BOOT_ROM_SIZE = 0x900;

    Verbose(ctx, 1, LogInfo("Opening Bootstrap ROM: '%s'", filename));

    FILE * file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    Verbose(ctx, 1, LogInfo("Bootstrap ROM Size %zu", size));

    size_t bytesRead = fread(ctx->Bootstrap, 1, 
        sizeof(ctx->Bootstrap), file);
    
    fclose(file);

    if (bytesRead != DMG_BOOT_ROM_SIZE && bytesRead == CGB_BOOT_ROM_SIZE) {
        LogWarn("Unknown Bootstrap ROM Type");
    }

    ctx->BootstrapEnabled = true;
    ctx->internal->HaveBootstrap = true;
    return true;
}

void GBx_StubBootstrap(gbx_t * ctx)
{
    ctx->CPU->AF = 0x01B0;
    ctx->CPU->BC = 0x0013;
    ctx->CPU->DE = 0x00D8;
    ctx->CPU->HL = 0x014D;
    ctx->CPU->PC = 0x0100;
    ctx->CPU->SP = 0xFFFE;

    sm83_mode_t mode = SM83_GetMode(ctx->CPU);

    if (mode == SM83_MODE_CGB) {
        ctx->CPU->A = 0x11;
    }
}