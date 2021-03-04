#include <GBx/Bootstrap.h>
#include <GBx/Context.h>
#include <GBx/Log.h>

#include <stdio.h>
#include <string.h>

void GBx_Bootstrap_Init(GBx * ctx)
{
    ctx->HaveBootstrap = false;
    ctx->HaveBootstrapCGB = false;
    ctx->BootstrapMapped = false;

    memset(ctx->BootROM, 0, sizeof(ctx->BootROM));
}

void GBx_Bootstrap_Reset(GBx * ctx)
{
    if (ctx->HaveBootstrap) {
        ctx->BootstrapMapped = true;
    }
}

bool GBx_Bootstrap_Load(GBx * ctx, const char * filename)
{
    GBxLogLoad("Opening Bootstrap ROM: '%s'", filename);

    FILE * file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    GBxLogVerbose("Bootstrap ROM Size %zu", size);

    size_t bytes = fread(ctx->BootROM, 1, sizeof(ctx->BootROM), file);

    fclose(file);

    if (bytes >= GBX_BOOT_ROM_SIZE_DMG) {
        ctx->HaveBootstrap = true;
    }

    if (bytes == GBX_BOOT_ROM_SIZE_CGB) {
        ctx->HaveBootstrapCGB = true;
    }

    return true;
}