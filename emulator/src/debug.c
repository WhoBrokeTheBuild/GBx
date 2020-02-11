#include "debug.h"

#include <stdlib.h>
#include <limits.h>

#include "cartridge.h"
#include "cpu.h"
#include "interrupt.h"
#include "log.h"
#include "memory.h"
#include "register.h"
#include "timer.h"
#include "video.h"

bool DebugMode = false;

bool RequestBreakpoint = false;

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
    if (RequestBreakpoint) {
        RequestBreakpoint = false;
        return true;
    }

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
    case BKCND_FC_EQ:
        if (R.FC == B.Value) {
            return true;
        }
        break;
    case BKCND_FH_EQ:
        if (R.FH == B.Value) {
            return true;
        }
        break;
    case BKCND_FN_EQ:
        if (R.FN == B.Value) {
            return true;
        }
        break;
    case BKCND_FZ_EQ:
        if (R.FZ == B.Value) {
            return true;
        }
        break;
    case BKCND_OP_EQ:
        if (readByte(R.PC) == B.Value) {
            return true;
        }
        break;
    default:
        break;
    };

    return false;
}

void requestBreakpoint()
{
    RequestBreakpoint = true;
}

#if defined(HAVE_READLINE)

#include <readline/readline.h>
#include <readline/history.h>

const char * help = 
"  help         Print this information\n"
"  quit         Exit the emulator\n"
"  continue     Continue normal execution\n"
"  info         Print information, see `help info`\n"
"  break        Set a breakpoint, see `help breakpoint`\n"
"  get          Get a flag or register\n"
"  set          Set a flag or register\n"
"  read         Read memory, see `help read`\n"
"  write        Write memory, see `help write`\n";

const char * helpInfo = 
"  registers    Print all registers and their values\n"
"  interrupts   Print IE and IF, and IME values\n"
"  lcd          Print LCDC values and LCD info\n"
"  stat         Print STAT values\n"
"  timer        Print TAC, TIMA, TMA and DIV info\n"
"  bank         Print ROM/RAM Bank info\n"
"  cartridge    Print Cartridge info\n";

const char * helpBreak = 
"  break ADDRESS\n"
"  break LHS=RHS\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"  This is a shortcut for saying `break PC=ADDRESS`\n"
"  LHS can be one of A, B, C, BC, D, E, DE, H, L, HL, FZ, FN, FH, FC, OP\n"
"  OP checks to the next opcode\n"
"  RHS will be interpreted as an integer\n"
"\n";

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
    else if (strncmp(input, "interrupts", length) == 0) {
        printIE();
        printIF();
        LogInfo("IME=%s", (IME ? "true" : "false"));
    }
    else if (strncmp(input, "lcd", length) == 0) {
        printLCDInfo();
        printLCDC();
    }
    else if (strncmp(input, "stat", length) == 0) {
        printSTAT();
    }
    else if (strncmp(input, "timer", length) == 0) {
        printTimer();
    }
    else if (strncmp(input, "bank", length) == 0) {
        printBank();
    }
    else if (strncmp(input, "cartridge", length) == 0) {
        printCartridge();
    }
    else {
        LogWarn("Unrecognized command 'info %s'", input);
    }
}

void debugBreak(const char * input)
{
    if (!input) {
        printf("%s", helpBreak);
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        printf("%s", helpInfo);
        return;
    }
    
    char * equal = strchr(input, '=');
    if (equal) {
        *equal = '\0';

        unsigned int rhs;
        sscanf(equal + 1, "%04X", &rhs);

        breakpoint_cond_t cond = BKCND_NONE;
        if (input[0] == 'A') {
            cond = BKCND_A_EQ;
        }
        else if (input[0] == 'B') {
            if (input[1] == 'C') {
                cond = BKCND_BC_EQ;
            }
            else {
                cond = BKCND_B_EQ;
            }
        }
        else if (input[0] == 'C') {
            cond = BKCND_C_EQ;
        }
        else if (input[0] == 'D') {
            if (input[1] == 'E') {
                cond = BKCND_DE_EQ;
            }
            else {
                cond = BKCND_D_EQ;
            }
        }
        else if (input[0] == 'E') {
            cond = BKCND_E_EQ;
        }
        else if (input[0] == 'H') {
            if (input[1] == 'L') {
                cond = BKCND_HL_EQ;
            }
            else {
                cond = BKCND_H_EQ;
            }
        }
        else if (input[0] == 'L') {
            cond = BKCND_L_EQ;
        }
        else if (input[0] == 'S' && input[1] == 'P') {
            cond = BKCND_SP_EQ;
        }
        else if (input[0] == 'P' && input[1] == 'C') {
            cond = BKCND_PC_EQ;
        }
        else if (input[0] == 'F') {
            if (input[1] == 'C') {
                cond = BKCND_FC_EQ;
            }
            else if (input[1] == 'H') {
                cond = BKCND_FH_EQ;
            }
            else if (input[1] == 'N') {
                cond = BKCND_FN_EQ;
            }
            else if (input[1] == 'Z') {
                cond = BKCND_FZ_EQ;
            }
        }
        else if (input[0] == 'O' && input[1] == 'P') {
            cond = BKCND_OP_EQ;
        }
        else {
            LogError("Unknown LHS '%s'", input);
        }

        LogDebug("Breakpoint set when %s=%04Xh", input, rhs);
        setBreakpoint(cond, rhs);
    }
    else {
        unsigned int pc;
        sscanf(input, "%04X", &pc);
        setBreakpoint(BKCND_PC_EQ, pc);
        LogDebug("Breakpoint set when PC=%04Xh", pc);
    }
}

void debugRead(const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", helpRead);
        return;
    }

    int size;
    uint16_t addr;
    sscanf(input, "%d %hX", &size, &addr);

    if (size == 8) {
        uint8_t value = readByte(addr);
        LogInfo("(%04X) = %02X", addr, value);
    }
    else if (size == 16) {
        uint16_t value = readWord(addr);
        LogInfo("(%04X) = %04X", addr, value);
    }
    else {
        LogError("Unsupported size '%d'", size);
    }
}

void debugWrite(const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", helpRead);
        return;
    }
    
    int size;
    uint16_t addr;
    sscanf(input, "%d %hX", &size, &addr);

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
        LogError("Unsupported size '%d'", size);
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
            debugBreak(args);
        }
        else if (strncmp(input, "get", length) == 0) {
            // TODO
        }
        else if (strncmp(input, "set", length) == 0) {
            // TODO
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
