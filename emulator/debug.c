#include "debug.h"

#include "cpu.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"
#include "register.h"
#include "video.h"
#include <stdlib.h>

bool DebugMode = false;

breakpoint_t B = { .PC = 0x10000 };

#if defined(HAVE_READLINE)
#include <readline/readline.h>
#include <readline/history.h>

void debugPrompt() 
{
    DebugMode = true;

    char prompt[2048];
    snprintf(prompt, sizeof(prompt), "[%04X]> ", R.PC);

    uint16_t addr;
    char * input = NULL;
    while ((input = readline(prompt)) != NULL) {
        if (strlen(input) > 0) {
            add_history(input);
        }

        if (strlen(input) == 0) {
            execute(fetch());
            B.PC = R.PC;
            break;
        }
        else if (strcmp(input, "quit") == 0 
            || strcmp(input, "q") == 0) {
            exit(0);
        }
        else if (strcmp(input, "continue") == 0 
            || strcmp(input, "c") == 0) {
            DebugMode = false;
            break;
        }
        else if (strcmp(input, "reg") == 0 
            || strcmp(input, "r") == 0) {
            printR();
        }
        else if (strcmp(input, "ie") == 0) {
            printIE();
        }
        else if (strcmp(input, "lcdc") == 0) {
            printLCDC();
        }
        else if (strcmp(input, "stat") == 0) {
            printSTAT();
        }
        else if (strcmp(input, "lcdinfo") == 0) {
            printLCDInfo();
        }
        else if (strncmp(input, "b ", 2) == 0
            || strncmp(input, "break ", 6) == 0) {
            sscanf(input, "%*s %04X", (unsigned int *)&B.PC);
            LogInfo("breakpoint set at %04Xh", B.PC);
        }
        else if (strncmp(input, "r8 ", 3) == 0
            || strncmp(input, "read8 ", 6) == 0) {
            sscanf(input, "%*s %04X", (unsigned int *)&addr);
            LogDebug("%02X", readByte(addr));
        }
        else if (strncmp(input, "r16 ", 4) == 0
            || strncmp(input, "read16 ", 7) == 0) {
            sscanf(input, "%*s %04X", (unsigned int *)&addr);
            LogDebug("%04X", readWord(addr));
        }

        free(input);
        input = NULL;
    }

    free(input);
}

#else

void debugPrompt()
{ }

#endif
