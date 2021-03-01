
#include <cflags.h>

#include "Context.h"
#include "Process.h"

int main(int argc, char ** argv)
{
    cflags_t * flags = NULL;
    
    flags = cflags_init();

    bool help = false;
    cflags_add_bool(flags, 'h', "help", &help,
        "Display this help and exit");
    
    const char * output = NULL;
    cflags_add_string(flags, 'o', NULL, &output,
        "Write the output to this file");
    
    bool preprocessOnly = false;
    cflags_add_bool(flags, 'E', NULL, &preprocessOnly,
        "Preprocess only");

    cflags_parse(flags, argc, argv);

    if (help || flags->argc == 1) {
        cflags_print_usage(flags, "[OPTION]... FILENAME...",
            "A Toy GameBoy Assembler",
            "Additional information about this program can be found at:\n"
            "  http://github.com/WhoBrokeTheBuild/GBx.git");
        
        cflags_free(flags);
        return 0;
    }

    if (!output) {
        output = "out.gb";
    }

    GBxASM ctx = {
        .PreprocessOnly = preprocessOnly,
    };

    ctx.Output = fopen(output, "wb");
    if (!ctx.Output) {
        printf("Failed to open %s for writing\n", output);
        return 1;
    }

    SymbolTable_Init(&ctx.GlobalSymbols);
    SymbolTable_Init(&ctx.LocalSymbols);

    SymbolReferenceList_Init(&ctx.References);

    for (int i = 1; i < flags->argc; ++i) {
        ProcessFile(&ctx, flags->argv[i]);
    }

    SymbolReferenceList_Write(&ctx.References, &ctx.GlobalSymbols, ctx.Output);
    SymbolReferenceList_PrintUnresolvedSymbols(&ctx.References);

    SymbolReferenceList_Term(&ctx.References);

    SymbolTable_Term(&ctx.GlobalSymbols);
    SymbolTable_Term(&ctx.LocalSymbols);

    fclose(ctx.Output);

    cflags_free(flags);

    return 0;
}