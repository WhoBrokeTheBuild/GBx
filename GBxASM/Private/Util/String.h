#ifndef GBXASM_STRING_H
#define GBXASM_STRING_H

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static inline char * SkipWhitespace(char * pch)
{
    if (pch) {
        while (isspace(*pch)) {
            ++pch;
        }
    }

    return pch;
}

char * RemoveWhitespace(char * str);

static inline bool StringEqual(const char * s1, const char * s2, size_t n)
{
    if (n > 0) {
        return (strncmp(s1, s2, n) == 0);
    }

    return (strcmp(s1, s2) == 0);
}

bool StringEqualIgnoreCase(const char * s1, const char * s2, size_t n);

const long ParseNumber(const char * num);

#endif // GBXASM_STRING_H