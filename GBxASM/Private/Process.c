#include "Process.h"

#include "Util.h"

#include <ctype.h>

void ProcessData(GBxASM * ctx, char * args)
{
    char * pch = args;

    char * start = NULL;
    while (*pch != '\0') {
        if (*pch == ';') {
            break;
        }
        else if (*pch == '"') {
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
        else if (*pch == ',') {
            *pch = '\0';
            if (start) {
                long num;
                if (ParseNumber(start, &num)) {
                    WriteU8(ctx, num);
                }
                else {
                    fprintf(stderr, "Invalid data, '%s'\n", start);
                }

                start = NULL;
            }
        }
        else if (!start && !isspace(*pch)) {
            start = pch;
        }

        ++pch;
    }

    if (start) {
        long num;
        if (ParseNumber(start, &num)) {
            WriteU8(ctx, num);
        }
        else {
            fprintf(stderr, "Invalid data, '%s'\n", pch);
        }
    }
}

bool ProcessArgument(GBxASM * ctx, char * arg, ArgumentType type, long baseAddress)
{
    if (type == ARG_TYPE_NONE || type == ARG_TYPE_LITERAL) {
        return true;
    }

    size_t length = (arg ? strlen(arg) : 0);
    if (length == 0) {
        return false;
    }

    long num = 0;
    if (IsValidSymbol(arg)) {
        SymbolReferenceList_Add(&ctx->References, arg, type, 
            GetWriteAddress(ctx), baseAddress);
    }
    else if (!ParseNumber(arg, &num)) {
        return false;
    }

    if (type == ARG_TYPE_U8 || type == ARG_TYPE_FF00_U8) {
        if (num < INT8_MIN || num > UINT8_MAX) {
            fprintf(stderr, "Argument out of bounds for int8, $%lX (%ld) [%d, %d]\n",
                num, num, INT8_MIN, UINT8_MAX);
            
            return false;
        }
        
        WriteU8(ctx, num);
        return true;
    }
    else if (type == ARG_TYPE_U16 || type == ARG_TYPE_ADDR_U16) {
        if (num < INT16_MIN || num > UINT16_MAX) {
            fprintf(stderr, "Argument out of bounds for int16,  $%lX (%ld) [%d, %d]\n",
                num, num, INT16_MIN, UINT16_MAX);
            
            return false;
        }
        
        WriteU16(ctx, num);
        return true;
    }
    else if (type == ARG_TYPE_S8 || type == ARG_TYPE_SP_S8) {
        if (num < INT8_MIN || num > UINT8_MAX) {
            fprintf(stderr, "Argument out of bounds for int8, $%lX (%ld) [%d, %d]\n",
                num, num, INT8_MIN, UINT8_MAX);
            
            return false;
        }

        WriteS8(ctx, num);
        return true;
    }

    return false;
}

bool ProcessInstruction(GBxASM * ctx, char * name, char * args)
{
    args = RemoveWhitespace(args);

    char * arg1 = strtok(args, ",");
    char * arg2 = strtok(NULL, "");

    char tmp1[INST_ARG_MAX_LEN];
    char tmp2[INST_ARG_MAX_LEN];

    Instruction * inst = GetInstructionList();
    for (; inst->Name; ++inst) {
        if (strcasecmp(name, inst->Name) != 0) {
            continue;
        }

        if (!ParseInstructionArgument(tmp1, arg1, inst->Type1, inst->Literal1)) {
            continue;
        }

        if (!ParseInstructionArgument(tmp2, arg2, inst->Type2, inst->Literal2)) {
            continue;
        }

        long endAddress = GetWriteAddress(ctx) + GetInstructionSize(inst);

        WriteU8(ctx, inst->Opcode1);

        if (inst->Opcode2 >= 0) {
            // Used by STOP and 0xCB
            WriteU8(ctx, inst->Opcode2);
        }

        if (!ProcessArgument(ctx, tmp1, inst->Type1, endAddress)) {
            return false;
        }

        if (!ProcessArgument(ctx, tmp2, inst->Type2, endAddress)) {
            return false;
        }

        return true;
    }

    if (arg1) {
        if (arg2) {
            fprintf(stderr, "Invalid instruction '%s %s, %s'\n", name, arg1, arg2);
        }
        else {
            fprintf(stderr, "Invalid instruction '%s %s'\n", name, arg1);
        }
    }
    else {
        fprintf(stderr, "Invalid instruction '%s'\n", name);
    }

    return false;
}

bool ProcessFile(GBxASM * ctx, const char * filename)
{
    FILE * file = fopen(filename, "rt");
    if (!file) {
        fprintf(stderr, "Unable to open '%s'\n", filename);
        return false;
    }

    char * line = NULL;
    size_t capacity;

    char * dir = GetParentPath(filename);

    while (getline(&line, &capacity, file) != -1) {

        // Trim newline
        char * pch = strtok(line, "\n");

        if (!pch) {
            if (ctx->PreprocessOnly) {
                printf("\n");
            }

            continue;
        }

        size_t length = strlen(pch);

        // Trim trailing whitespace
        while (length > 0 && isspace(pch[length - 1])) {
            pch[length - 1] = '\0';
            --length;
        }

        // Preprocessor
        if (pch[0] == '#') {
            const char * include = "#include ";
            size_t includeLength = strlen(include);

            if (strncmp(include, pch, includeLength) == 0) {
                pch += includeLength;

                length = strlen(pch);
                if (pch[0] == '"' && pch[length - 1] == '"') {
                    pch[length - 1] = '\0';
                    ++pch;
                    length -= 2;
                }

                if (dir) {
                    char * incFilename = (char *)malloc(strlen(dir) + strlen(pch) + 1);
                    strcpy(incFilename, dir);
                    strcat(incFilename, "/");
                    strcat(incFilename, pch);

                    if (!ProcessFile(ctx, incFilename)) {
                        break;
                    }

                    free(incFilename);
                }
                else {
                    if (!ProcessFile(ctx, pch)) {
                        break;
                    }
                }
            }
            else {
                fprintf(stderr, "Invalid preprocessor statement, '%s'", line);
            }

            continue;
        }

        if (ctx->PreprocessOnly) {
            printf("%s\n", pch);
            continue;
        }

        // Labels
        char * colon = strchr(pch, ':');
        if (colon) {
            pch = strtok(pch, ":");

            if (IsValidSymbol(pch)) {
                if (pch[0] == '.') {
                    long tmp = SymbolTable_Get(&ctx->LocalSymbols, pch);
                    if (tmp == LONG_MIN) {
                        SymbolTable_Add(&ctx->LocalSymbols, pch, GetWriteAddress(ctx));
                    }
                    else {
                        fprintf(stderr, "Cannot redefine symbol, '%s'\n", pch);
                        continue;
                    }
                }
                else {
                    long tmp = SymbolTable_Get(&ctx->GlobalSymbols, pch);
                    if (tmp == LONG_MIN) {
                        SymbolTable_Add(&ctx->GlobalSymbols, pch, GetWriteAddress(ctx));

                        SymbolReferenceList_Write(&ctx->References, &ctx->LocalSymbols, ctx->Output);
                        SymbolTable_Clear(&ctx->LocalSymbols);
                    }
                    else {
                        fprintf(stderr, "Cannot redefine symbol, '%s'\n", pch);
                        continue;
                    }
                }
            }
            else {
                fprintf(stderr, "Invalid label, '%s'\n", pch);
                continue;
            }

            pch = strtok(NULL, "");
            if (!pch) {
                continue;
            }
        }

        pch = SkipWhitespace(pch);
        if (*pch == '\0') {
            continue;
        }

        const char * db = "DB ";
        size_t dbLength = strlen(db);

        if (strncasecmp("DB ", pch, dbLength) == 0) {
            ProcessData(ctx, pch + dbLength);
            continue;
        }

        char * semicolon = strchr(pch, ';');
        if (semicolon) {
            *semicolon = '\0';
        }

        length = strlen(pch);
        if (length == 0) {
            continue;
        }

        char * name = strtok(pch, " \t");
        char * args = strtok(NULL, "");

        if (strcasecmp("ORG", name) == 0) {
            long num;
            if (ParseNumber(args, &num)) {
                fseek(ctx->Output, num, SEEK_SET);
            }
        }
        else if (strcasecmp("DS", name) == 0) {
            long num;
            if (ParseNumber(args, &num)) {
                fseek(ctx->Output, num, SEEK_CUR);
            }
        }
        else {
            ProcessInstruction(ctx, name, args);
        }
    }

    if (dir) {
        free(dir);
        dir = NULL;
    }

    free(line);
    line = NULL;

    fclose(file);

    return true;
}
