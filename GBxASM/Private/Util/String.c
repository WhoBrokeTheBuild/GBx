#include "String.h"

#include <stdint.h>

char * RemoveWhitespace(char * str)
{
    if (!str) {
        return NULL;
    }

    size_t length = strlen(str);

    for (size_t i = 0; i < length; ++i) {
        if (isspace(str[i])) {
            for (size_t j = i; j < length; ++j) {
                str[j] = str[j + 1];
            }
            --length;
            --i;
        }
    }

    return str;
}

bool StringEqualIgnoreCase(const char * s1, const char * s2, size_t n)
{
    if (s1 == s2) {
        return true;
    }

    if (!s1 || !s2) {
        return false;
    }

    if (n == 0) {
        n = SIZE_MAX;
    }

    int c1, c2;
    do {
        c1 = *s1++;
        c2 = *s2++;
        c1 = tolower((unsigned char)c1);
        c2 = tolower((unsigned char)c2);
        --n;
    }
    while(n > 0 && c1 == c2 && c1 != '\0');

    return (c1 - c2 == 0);
}

const long ParseNumber(const char * str)
{
    if (!str) {
        return LONG_MIN;
    }

    while (isspace(*str)) {
        ++str;
    }

    size_t length = strlen(str);
    if (length < 1) {
        return LONG_MIN;
    }

    long num = LONG_MIN;
    char * endptr = NULL;

    if (str[0] == '$') {
        num = strtol(str + 1, &endptr, 16);
    }
    else if (str[0] == '0') {
        num = strtol(str + 1, &endptr, 8);
    }
    else {
        num = strtol(str, &endptr, 10);
    }

    if (str == endptr) {
        return LONG_MIN;
    }

    return num;
}
