#include <GBx/Bootstrap.h>

#include "Context.h"

#include <stdio.h>
#include <string.h>

GBx_Bootstrap * GBx_GetBootstrap(GBx * ctx)
{
    return &ctx->Bootstrap;
}

void GBx_Bootstrap_Reset(GBx * ctx)
{
    GBx_Bootstrap * boot = &ctx->Bootstrap;

    boot->Enabled = false;
    memset(boot->ROM, 0, sizeof(boot->ROM));
}

bool GBx_Bootstrap_Load(GBx * ctx, const char * filename)
{
    const size_t DMG_BOOT_ROM_SIZE = 0x100;
    const size_t CGB_BOOT_ROM_SIZE = 0x900;

    GBxLogLoad("Opening Bootstrap ROM: '%s'", filename);

    FILE * file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    GBxLogVerbose("Bootstrap ROM Size %zu", size);

    size_t bytes = fread(ctx->Bootstrap.ROM, 1, sizeof(ctx->Bootstrap.ROM), file);

    fclose(file);

    // TODO: HaveBootstrap
    ctx->Bootstrap.Enabled = true;

    return true;
}