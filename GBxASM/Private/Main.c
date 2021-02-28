
#include <cflags.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "Context.h"

#include "Util/String.h"
#include "Instruction.h"

bool IsValidSymbol(const char * name, size_t length)
{
    const char * RESERVED_SYMBOLS[] = {
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "AF",
        "BC",
        "DE",
        "HL",
        "SP",
        NULL
    };

    if (!name) {
        return false;
    }

    if (length == 0) {
        return false;
    }

    // Local Symbol
    if (name[0] == '.') {
        ++name;
    }

    if (isdigit(name[0])) {
        return false;
    }

    for (int i = 0; RESERVED_SYMBOLS[i]; ++i) {
        if (StringEqualIgnoreCase(name, RESERVED_SYMBOLS[i], length)) {
            return false;
        }
    }

    // A-Z0-9_, doesn't start with a number
    for (size_t i = 0; i < length; ++i) {
        if (!isalnum(name[i]) && name[i] != '_') {
            return false;
        }
    }

    return true;
}

bool AdjustSpecialArguments(ArgumentType type, const char ** pstr, size_t * plength)
{
    if (!pstr || !*pstr || !plength) {
        return false;
    }

    const char * str = *pstr;
    size_t length = *plength;
    
    if (type == ARG_TYPE_ADDR_U16) {
        if (str[0] != '(' || str[length - 1] != ')') {
            return false;
        }

        ++str;
        length -= 2;
    }
    else if (type == ARG_TYPE_FF00_U8) {
        const char * prefix = "($FF00+";
        size_t prefixLength = strlen(prefix);

        if (length < prefixLength) {
            return false;
        }

        if (!StringEqualIgnoreCase(prefix, str, prefixLength) || str[length - 1] != ')') {
            return false;
        }

        str += prefixLength;
        length -= prefixLength + 1;
    }
    else if (type == ARG_TYPE_SP_S8) {
        const char * prefix = "SP+";
        size_t prefixLength = strlen(prefix);

        if (length < prefixLength) {
            return false;
        }
        if (!StringEqualIgnoreCase(prefix, str, prefixLength) || str[length - 1] != ')') {
            return false;
        }

        str += prefixLength;
        length -= prefixLength + 1;
    }

    *pstr = str;
    *plength = length;

    return true;
}

bool CheckArgument(GBxASM * ctx, const char * str, ArgumentType type, const char * literal)
{
    size_t length = (str ? strlen(str) : 0);

    if (type == ARG_TYPE_NONE) {
        if (length == 0) {
            return true;
        }
    }
    else if (type == ARG_TYPE_LITERAL) {
        if (StringEqualIgnoreCase(str, literal, 0)) {
            return true;
        }
    }
    else {
        if (!AdjustSpecialArguments(type, &str, &length)) {
            return false;
        }

        if (IsValidSymbol(str, length)) {
            return true;
        }
        
        if (isdigit(str[0]) || str[0] == '$') {
            long num = ParseNumber(str);

            if (num != LONG_MIN) {
                return true;
            }
        }
    }

    return false;
}

bool WriteArgument(GBxASM * ctx, const char * str, ArgumentType type, long endAddress)
{
    if (type == ARG_TYPE_NONE || type == ARG_TYPE_LITERAL) {
        return true;
    }

    if (!str) {
        return false;
    }

    size_t length = strlen(str);

    if (!AdjustSpecialArguments(type, &str, &length)) {
        return false;
    }

    long num = 0;
    if (IsValidSymbol(str, length)) {
        SymbolReferenceType refType = SYM_REF_TYPE_NONE;
        switch (type) {
        case ARG_TYPE_U8:
        case ARG_TYPE_FF00_U8:
            refType = SYM_REF_TYPE_U8;
            break;
        case ARG_TYPE_U16:
        case ARG_TYPE_ADDR_U16:
            refType = SYM_REF_TYPE_U16;
            break;
        case ARG_TYPE_S8:
        case ARG_TYPE_SP_S8:
            refType = SYM_REF_TYPE_S8;
            break;
        default:
            break;
        }

        // TODO: Honor length
        SymbolReferenceList_Add(&ctx->References, str, refType, 
            GetWriteAddress(ctx), endAddress);
    }
    else {
        num = ParseNumber(str);
        if (num == LONG_MIN) {
            return false;
        }
    }

    if (type == ARG_TYPE_U8 || type == ARG_TYPE_FF00_U8) {
        if (num < 0 || num > UINT8_MAX) {
            printf("Argument out of bounds, 0 <= %ld <= %d\n",
                num, UINT8_MAX);
            
            return false;
        }
        
        WriteU8(ctx, num);
    }
    else if (type == ARG_TYPE_U16 || type == ARG_TYPE_ADDR_U16) {
        if (num < 0 || num > UINT16_MAX) {
            printf("Argument out of bounds, 0 <= %ld <= %d\n",
                num, UINT16_MAX);
            
            return false;
        }
        
        WriteU16(ctx, num);
    }
    else if (type == ARG_TYPE_S8 || type == ARG_TYPE_SP_S8) {
        // if (num < INT8_MIN || num > INT8_MAX) {
        //     printf("Argument out of bounds, %d <= %ld <= %d\n",
        //         INT8_MIN, num, INT8_MAX);
            
        //     return false;
        // }

        WriteS8(ctx, num);
    }

    return true;
}

void ProcessData(GBxASM * ctx, char * name, char * args)
{
    char * pch = strtok(args, ",");
    while (pch) {
        pch = SkipWhitespace(pch);
        if (*pch == '\0') {
            break;
        }

        if (*pch == '"') {
            ++pch;

            bool inEscape = false;

            while (*pch != '\0') {
                if (inEscape) {
                    inEscape = false;

                    WriteU8(ctx, *pch);
                }
                else if (*pch == '\\') {
                    inEscape = true;
                }
                else if (*pch == '"') {
                    break;
                }
                else {
                    WriteU8(ctx, *pch);
                }

                ++pch;
            }
        }
        else {
            long num = ParseNumber(pch);

            if (StringEqualIgnoreCase("DB", name, 0)) {
                WriteU8(ctx, num);
            }
            else if (StringEqualIgnoreCase("DS", name, 0)) {
                WriteU16(ctx, num);
            }
            
        }

        pch = strtok(NULL, ",");
    }
}

bool ProcessInstruction(GBxASM * ctx, char * name, char * args)
{
    args = RemoveWhitespace(args);

    char * arg1 = strtok(args, ",");
    char * arg2 = strtok(NULL, "");

    Instruction * inst = _InstructionList;
    for (; inst->Name; ++inst) {
        if (!StringEqualIgnoreCase(name, inst->Name, 0)) {
            continue;
        }

        if (GetWriteAddress(ctx) >= 0x001f) {
            printf("ass\n");
        }

        if (!CheckArgument(ctx, arg1, inst->Type1, inst->Literal1)) {
            continue;
        }

        if (!CheckArgument(ctx, arg2, inst->Type2, inst->Literal2)) {
            continue;
        }

        long endAddress = GetWriteAddress(ctx) + GetInstructionSize(inst);

        // At this point, we're pretty sure this is the right instruction
        WriteU8(ctx, inst->Opcode1);

        if (inst->Opcode2 >= 0) {
            // Used by STOP and 0xCB
            WriteU8(ctx, inst->Opcode2);
        }

        if (!WriteArgument(ctx, arg1, inst->Type1, endAddress)) {
            break;
        }

        if (!WriteArgument(ctx, arg2, inst->Type2, endAddress)) {
            break;
        }

        return true;
    }

    printf("Failed to process '%s %s, %s'\n", name, arg1, arg2);
    return false;
}

void ProcessFile(GBxASM * ctx, const char * filename)
{
    FILE * file = fopen(filename, "rt");

    char * line = NULL;
    size_t capacity;

    while (getline(&line, &capacity, file) != -1) {

        // Trim newline or comments
        strtok(line, ";\n");

        size_t length = strlen(line);

        // Trim trailing whitespace
        while (length > 0 && isspace(line[length - 1])) {
            line[length - 1] = '\0';
            --length;
        }

        bool inPreprocessor = false;
        bool inGlobalLabel = false;
        bool inLocalLabel = false;
        bool inIndent = false;
        bool inInstruction = false;
        bool inData = false;

        if (line[0] == ';') {
            // Comment
            continue;
        }
        else if (isspace(line[0])) {
            inIndent = true;
        }
        else if (line[0] == '#') {
            inPreprocessor = true;
        }
        else if (line[0] == '.') {
            inLocalLabel = true;
        }
        else if (isalpha(line[0])) {
            inGlobalLabel = true;
        }
        else {
            // Error
        }

        if (inPreprocessor) {
            // TODO:
            continue;
        }

        if (inLocalLabel || inGlobalLabel) {
            char * pch = strtok(line, ":");
            
            if (pch) {
                if (inGlobalLabel) {
                    // printf("Adding Global Label %s: %04lX\n", line, GetWriteAddress(ctx));
                    SymbolTable_Add(&ctx->GlobalSymbols, line, GetWriteAddress(ctx));

                    SymbolReferenceList_Write(&ctx->References, &ctx->LocalSymbols, ctx->Output);
                    SymbolTable_Clear(&ctx->LocalSymbols);
                }
                else if (inLocalLabel) {
                    // printf("Adding Local Label %s: %04lX\n", line, GetWriteAddress(ctx));
                    SymbolTable_Add(&ctx->LocalSymbols, line, GetWriteAddress(ctx));
                }
                else {
                    // Error
                }
            }
            else {
                // Error
            }
        }

        if (inIndent) {
            char * pch = line;
            
            pch = SkipWhitespace(pch);
            if (*pch == '\0') {
                continue;
            }

            char * name = strtok(pch, " \t");
            char * args = strtok(NULL, "");

            if (StringEqualIgnoreCase("ORG", name, 0)) {
                long num = ParseNumber(args);
                if (num >= 0) {
                    fseek(ctx->Output, num, SEEK_SET);
                }
            }
            else if (StringEqualIgnoreCase("DB", name, 0) || StringEqualIgnoreCase("DS", name, 0)) {
                ProcessData(ctx, name, args);
            }
            else {
                ProcessInstruction(ctx, name, args);
            }
        }
    }

    free(line);
    line = NULL;

    fclose(file);
}

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
    cflags_add_string(flags, 'E', NULL, &output,
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

    GBxASM ctx;

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