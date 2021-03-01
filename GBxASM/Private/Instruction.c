#include "Instruction.h"

#include "Util.h"

#include <stdio.h>
#include <ctype.h>

#include "InstructionList.inc.h"

int GetInstructionSize(Instruction * inst)
{
    int size = 1;

    if (inst->Opcode2 >= 0) {
        ++size;
    }

    switch (inst->Type1) {
    case ARG_TYPE_U8:
    case ARG_TYPE_S8:
    case ARG_TYPE_FF00_U8:
    case ARG_TYPE_SP_S8:
        ++size;
        break;
    case ARG_TYPE_U16:
    case ARG_TYPE_ADDR_U16:
        size += 2;
        break;
    default:
        break;
    }

    switch (inst->Type2) {
    case ARG_TYPE_U8:
    case ARG_TYPE_S8:
    case ARG_TYPE_FF00_U8:
    case ARG_TYPE_SP_S8:
        ++size;
        break;
    case ARG_TYPE_U16:
    case ARG_TYPE_ADDR_U16:
        size += 2;
        break;
    default:
        break;
    }

    return size;
}

bool ParseInstructionArgument(char out[INST_ARG_MAX_LEN], 
    const char * arg, ArgumentType type, const char * literal)
{
    if (!out) {
        return false;
    }

    *out = '\0';

    size_t length = (arg ? strlen(arg) : 0);

    if (length > INST_ARG_MAX_LEN) {
        fprintf(stderr, "Argument too long, %zu > %d", length, INST_ARG_MAX_LEN);
        return false;
    }

    if (type == ARG_TYPE_NONE) {
        if (length == 0) {
            return true;
        }
    }
    else if (type == ARG_TYPE_LITERAL) {
        if (arg && strcasecmp(arg, literal) == 0) {
            return true;
        }
    }
    else {
        if (type == ARG_TYPE_ADDR_U16) {
            // (#)
            if (length < 3) {
                return false;
            }

            if (arg[0] != '(' || arg[length - 1] != ')') {
                return false;
            }

            length -= 2;
            strncpy(out, arg + 1, length);
            out[length] = '\0';
        }
        else if (type == ARG_TYPE_FF00_U8) {
            // ($FF00+#)
            const char * prefix = "($FF00+";
            size_t prefixLength = strlen(prefix);

            if (length < prefixLength + 2) {
                return false;
            }

            if (strncasecmp(prefix, arg, prefixLength) != 0 || arg[length - 1] != ')') {
                return false;
            }

            length -= prefixLength + 1;
            strncpy(out, arg + prefixLength, length);
            out[length] = '\0';
        }
        else if (type == ARG_TYPE_SP_S8) {
            // SP+#
            const char * prefix = "SP+";
            size_t prefixLength = strlen(prefix);

            if (length < prefixLength + 1) {
                return false;
            }
            
            if (strncasecmp(prefix, arg, prefixLength) != 0 || arg[length - 1] != ')') {
                return false;
            }

            length -= prefixLength + 1;
            strncpy(out, arg + prefixLength, length);
            out[length] = '\0';
        }
        else {
            strcpy(out, arg);
        }

        if (strcmp(out, "$ff26") == 0) {
            printf("ass\n");
        }

        if (IsValidSymbol(out)) {
            return true;
        }

        long num;
        if (ParseNumber(out, &num)) {
            return true;
        }
    }

    return false;
}

Instruction * GetInstructionList()
{
    return _InstructionList;
}
