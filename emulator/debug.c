#include "debug.h"

#include <stdlib.h>
#include <limits.h>

#include "cpu.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"
#include "register.h"
#include "video.h"

bool DebugMode = false;

typedef struct {
    breakpoint_cond_t Condition;
    uint32_t Value;

} breakpoint_t;

breakpoint_t B = {
    .Condition = BKCND_NONE,
    .Value = 0,
};

void setBreakpoint(breakpoint_cond_t cond, uint32_t value)
{
    B.Condition = cond;
    B.Value = value;
}

void clearBreakpoint()
{
    B.Condition = BKCND_NONE;
}

bool atBreakpoint()
{
    switch (B.Condition) {
    case BKCND_A_EQ:
        if (R.A == B.Value) {
            return true;
        }
        break;
    case BKCND_B_EQ:
        if (R.B == B.Value) {
            return true;
        }
        break;
    case BKCND_C_EQ:
        if (R.C == B.Value) {
            return true;
        }
        break;
    case BKCND_BC_EQ:
        if (R.BC == B.Value) {
            return true;
        }
        break;
    case BKCND_D_EQ:
        if (R.D == B.Value) {
            return true;
        }
        break;
    case BKCND_E_EQ:
        if (R.E == B.Value) {
            return true;
        }
        break;
    case BKCND_DE_EQ:
        if (R.DE == B.Value) {
            return true;
        }
        break;
    case BKCND_H_EQ:
        if (R.H == B.Value) {
            return true;
        }
        break;
    case BKCND_L_EQ:
        if (R.L == B.Value) {
            return true;
        }
        break;
    case BKCND_HL_EQ:
        if (R.HL == B.Value) {
            return true;
        }
        break;
    case BKCND_SP_EQ:
        if (R.SP == B.Value) {
            return true;
        }
        break;
    case BKCND_PC_EQ:
        if (R.PC == B.Value) {
            return true;
        }
        break;
    case BKCND_NEXT_OP_EQ:
        if (readByte(R.PC) == B.Value) {
            return true;
        }
        break;
    };

    return false;
}

#if defined(HAVE_READLINE)

#include <readline/readline.h>
#include <readline/history.h>

const char * help = 
"  help         Print this information\n"
"  quit         Exit the emulator\n"
"  continue     Continue normal execution\n"
"  info         Print information, see `help info`\n"
"  breakpoint   Set a breakpoint, see `help breakpoint`\n"
"  read         Read memory, see `help read`\n"
"  write        Write memory, see `help write`\n";

const char * helpInfo = 
"  registers    Print all registers and their values\n"
"  ie           Print IE values\n"
"  lcd          Print LCDC values and LCD info\n"
"  stat         Print STAT values\n";

const char * helpRead = 
"  read SIZE ADDRESS\n"
"\n"
"  SIZE can be 8 or 16\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n";

const char * helpWrite = 
"  write SIZE ADDRESS VALUE\n"
"\n"
"  SIZE can be 8 or 16\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"  VALUE will be interpreted as an 8-bit or 16-bit number, depending on SIZE\n";

void debugInfo(const char * input)
{
    if (!input) {
        printf("%s", helpInfo);
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        printf("%s", helpInfo);
        return;
    }

    if (strncmp(input, "registers", length) == 0) {
        printR();
    }
    else if (strncmp(input, "ie", length) == 0) {
        printIE();
    }
    else if (strncmp(input, "lcd", length) == 0) {
        printLCDInfo();
        printLCDC();
    }
    else if (strncmp(input, "stat", length) == 0) {
        printSTAT();
    }
    else {
        LogWarn("Unrecognized command 'info %s'", input);
    }
}

void debugRead(const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", helpRead);
        return;
    }

    uint8_t size;
    uint16_t addr;
    sscanf(input, "%hhu %hX", &size, &addr);

    if (size == 8) {
        uint8_t value = readWord(addr);
        LogInfo("(%04X) = %02X", addr, value);
    }
    else if (size == 16) {
        uint16_t value = readWord(addr);
        LogInfo("(%04X) = %04X", addr, value);
    }
    else {
        LogError("Unsupported size '%hhu'", size);
    }
}

void debugWrite(const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", helpRead);
        return;
    }
    
    uint8_t size;
    uint16_t addr;
    sscanf(input, "%hhu %hX", &size, &addr);

    if (size == 8) {
        uint8_t value;
        sscanf(input, "%*s %*s %hhX", &value);
        LogInfo("Writing %02X to %04X", value, addr);
        writeByte(addr, value);
    }
    else if (size == 16) {
        uint16_t value;
        sscanf(input, "%*s %*s %hX", &value);
        LogInfo("Writing %04X to %04X", value, addr);
        writeWord(addr, value);
    }
    else {
        LogError("Unsupported size '%hhu'", size);
    }
}

void debugHelp(const char * input)
{
    if (!input) {
        printf("%s", help);
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        printf("%s", helpInfo);
        return;
    }

    if (strncmp(input, "info", length) == 0) {
        printf("%s", helpInfo);
    }
    else if (strncmp(input, "read", length) == 0) {
        printf("%s", helpRead);
    }
    else if (strncmp(input, "write", length) == 0) {
        printf("%s", helpWrite);
    }
}

void debugPrompt() 
{
    DebugMode = true;

    char prompt[2048];
    snprintf(prompt, sizeof(prompt), "[%04X]> ", R.PC);

    uint16_t addr;
    char * input = NULL;
    while ((input = readline(prompt)) != NULL) {

        size_t length = 0;
        char * sep = strchr(input, ' ');
        char * args = NULL;
        if (sep) {
            length = sep - input;
            args = input + length + 1;
        }
        else {
            length = strlen(input);
        }

        if (length > 0) {
            add_history(input);
        }

        if (length == 0) {
            nextInstruction();
        }
        else if (strncmp(input, "quit", length) == 0) {
            exit(0);
        }
        else if (strncmp(input, "continue", length) == 0) {
            DebugMode = false;
            break;
        }
        else if (strncmp(input, "info", length) == 0) {
            debugInfo(args);
        }
        else if (strncmp(input, "break", length) == 0) {
            unsigned int pc;
            sscanf(input, "%*s %04X", &pc);
            setBreakpoint(BKCND_PC_EQ, pc);
            LogDebug("breakpoint set at %04Xh", pc);
        }
        else if (strncmp(input, "read", length) == 0) {
            debugRead(args);
        }
        else if (strncmp(input, "write", length) == 0) {
            debugWrite(args);
        }
        else if (strncmp(input, "help", length) == 0) {
            debugHelp(args);
        }

        free(input);
        input = NULL;

        snprintf(prompt, sizeof(prompt), "[%04X]> ", R.PC);
    }

    free(input);
}

#else

void debugPrompt()
{ }

#endif
