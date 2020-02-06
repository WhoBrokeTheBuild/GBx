#ifndef USAGE_H
#define USAGE_H

#include <stdio.h>

static void usage() 
{
    printf(
        "usage: GBx FILENAME [-d]\n"
        "\n"
        "-d     Run with the built-in debugger."
    );
}

#endif // USAGE_H