#include "debug.h"

#include "breakpoint.h"

#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>

#include <GBx/apu.h>
#include <GBx/bootstrap.h>
#include <GBx/cartridge.h>
#include <GBx/cpu.h>
#include <GBx/instruction.h>
#include <GBx/interrupt.h>
#include <GBx/lcd.h>
#include <GBx/log.h>
#include <GBx/memory.h>
#include <GBx/timer.h>

#ifdef HAVE_READLINE
    #include <readline/readline.h>
    #include <readline/history.h>

    #define HISTORY_FILE ".gbx_history"
#endif

bool DebugEnabled = false;

void handleSignal(int sig)
{
    LogInfo("Caught signal %d", sig);

    if (sig == SIGINT) {
        SetBreakpoint("PC", R.PC);
    }
    else {
        DebugPrompt();
        exit(1);
    }
}

void DebugInit()
{
    signal(SIGINT, handleSignal);
    signal(SIGSEGV, handleSignal);

    #ifdef HAVE_READLINE
        read_history(HISTORY_FILE);
    #endif
}

void DebugTerm()
{   
    #ifdef HAVE_READLINE
        write_history(HISTORY_FILE);
    #endif
    
    signal(SIGINT, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
}

const char * help = 
"All commands may be shortened, ambiguous commands will be\n"
"processed in the order they are listed here.\n"
"e.g. `i r` will run `info registers`\n"
"but `r` will run `read` not `reset`\n"
"\n"
"  help         Print this information\n"
"  info         Print information, see `help info`\n"
"  break        Set a breakpoint, see `help break`\n"
"  delete       Delete a breakpoint, see `help delete`\n"
"  continue     Continue normal execution\n"
"  next         Run the next N instructions\n"
"  read         Read memory, see `help read`\n"
"  write        Write memory, see `help write`\n"
"  disassemble  Disassemble instructions at address\n"
"  quit         Exit the emulation\n"
"  reset        Reset the emulation\n";

const char * helpInfo = 
"  all          Print all information\n"
"  apu          Print Tone1, Tone2, Wave, Noise, and APU Control registers\n"
"  registers    Print all registers and their values\n"
"  interrupts   Print IE and IF, and IME values\n"
"  lcd          Print LCDC, STAT registers and LCD coordinates\n"
"  timer        Print TAC, TIMA, TMA and DIV info\n"
"  bank         Print ROM/RAM Bank info\n"
"  cartridge    Print Cartridge info\n";

const char * helpBreak = 
"break [CONDITION|ADDRESS]\n"
"\n"
"  Set a breakpoint at ADDRESS, or when CONDITION is met\n"
"\n"
"  CONDITION will be interpreted either as A=B\n"
"  where B is a hex number, and A can be one of the following:\n"
"    A, B, C, D, E, H, L, AF, BC, DE, HL, SP, PC, FZ, FN, FH, FC\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"\n"
"  If no CONDITION or ADDRESS are specified, all breakpoints\n"
"  will be listed\n"
"\n";

const char * helpDelete = 
"delete [ADDRESS]\n"
"\n"
"  Delete a breakpoint at ADDRESS, or all breakpoints\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"  If ADDRESS is not specified, all breakpoints will be deleted\n"
"\n";

const char * helpNext = 
"next [COUNT]\n"
"\n"
"  Run the next COUNT instructions\n"
"\n"
"  COUNT will be interpreted as an unsigned decimal integer\n"
"  If COUNT is not present, it will default to 1\n";

const char * helpRead = 
"read SIZE ADDRESS\n"
"\n"
"  Read SIZE bytes from ADDRESS\n"
"\n"
"  SIZE can be either 1 or 2\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n";

const char * helpWrite = 
"write SIZE ADDRESS VALUE\n"
"\n"
"  Write SIZE bytes to ADDRESS from VALUE\n"
"\n"
"  SIZE can be 1 or 2\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"\n"
"  VALUE will be interpreted as an 8-bit or 16-bit number, depending on SIZE\n";

const char * helpDisassemble = 
"disassemble [COUNT] [ADDRESS]\n"
"\n"
"  Dissassmble COUNT instructions at ADDRESS\n"
"\n"
"  COUNT will be interpreted as an unsigned decimal integer\n"
"  If COUNT is not present, it will default to 1\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"  If ADDRESS is not present, it will default to PC\n";

void cmdHelp(const char * input)
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
    else if (strncmp(input, "break", length) == 0) {
        printf("%s", helpBreak);
    }
    else if (strncmp(input, "delete", length) == 0) {
        printf("%s", helpDelete);
    }
    else if (strncmp(input, "next", length) == 0) {
        printf("%s", helpNext);
    }
    else if (strncmp(input, "read", length) == 0) {
        printf("%s", helpRead);
    }
    else if (strncmp(input, "write", length) == 0) {
        printf("%s", helpWrite);
    }
    else {
        printf("Unknown command '%s'", input);
    }
}

void cmdInfo(const char * input)
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

    if (strncmp(input, "all", length) == 0) {
        PrintR();
        PrintTone1();
        PrintTone2();
        PrintWave();
        PrintWaveRAM();
        PrintVolumeControl();
        PrintIE();
        PrintIF();
        PrintLCDCoordinates();
        PrintLCDC();
        PrintSTAT();
        PrintTimer();
        PrintCartridgeMBC();
        PrintCartridge();
    }
    else if (strncmp(input, "apu", length) == 0) {
        PrintTone1();
        PrintTone2();
        PrintWave();
        PrintWaveRAM();
        PrintVolumeControl();
    }
    else if (strncmp(input, "registers", length) == 0) {
        PrintR();
    }
    else if (strncmp(input, "interrupts", length) == 0) {
        PrintIE();
        PrintIF();
        LogInfo("IME=%s", (IME ? "true" : "false"));
    }
    else if (strncmp(input, "lcd", length) == 0) {
        PrintLCDCoordinates();
        PrintLCDC();
        PrintSTAT();
    }
    else if (strncmp(input, "timer", length) == 0) {
        PrintTimer();
    }
    else if (strncmp(input, "mbc", length) == 0) {
        PrintCartridgeMBC();
    }
    else if (strncmp(input, "cartridge", length) == 0) {
        PrintCartridge();
    }
    else {
        LogWarn("Unrecognized command 'info %s'", input);
    }
}

void cmdBreak(const char * input)
{
    if (!input) {
        PrintBreakpoints();
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        PrintBreakpoints();
        return;
    }
    
    char * equal = strchr(input, '=');
    if (equal) {
        *equal = '\0';

        unsigned int value;
        sscanf(equal + 1, "%04X", &value);

        SetBreakpoint(input, value);
        LogInfo("Breakpoint set when %s=%04Xh", input, value);
    }
    else {
        unsigned int pc;
        sscanf(input, "%04X", &pc);
        SetBreakpoint("PC", pc);
        LogInfo("Breakpoint set when PC=%04Xh", pc);
    }
}

void cmdDelete(const char * input)
{
    if (!input) {
        ClearAllBreakpoints();
        LogInfo("All Breakpoints deleted");
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        ClearAllBreakpoints();
        LogInfo("All Breakpoints deleted");
        return;
    }
    
    char * equal = strchr(input, '=');
    if (equal) {
        *equal = '\0';

        unsigned int value;
        sscanf(equal + 1, "%04X", &value);

        ClearBreakpoint(input, value);
        LogInfo("Breakpoint %s=%04Xh deleted", input, value);
    }
    else {
        unsigned int pc;
        sscanf(input, "%04X", &pc);
        ClearBreakpoint("PC", pc);
        LogInfo("Breakpoint PC=%04Xh deleted", pc);
    }
}

void cmdNext(const char * input)
{
    if (!input || strlen(input) == 0) {
        NextInstruction();
        return;
    }

    unsigned count;
    sscanf(input, "%u", &count);

    for (int i = 0; i < count; ++i) {
        NextInstruction();
    }
}

void cmdRead(const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", helpRead);
        return;
    }

    int size;
    word addr;
    sscanf(input, "%d %hX", &size, &addr);

    if (size == 8) {
        byte value = ReadByte(addr);
        LogInfo("(%04X) = %02X", addr, value);
    }
    else if (size == 16) {
        word value = ReadWord(addr);
        LogInfo("(%04X) = %04X", addr, value);
    }
    else {
        LogError("Unsupported size '%d'", size);
    }
}

void cmdWrite(const char * input)
{
    if (!input || strlen(input) == 0) {
        printf("%s", helpRead);
        return;
    }
    
    int size;
    word addr;
    sscanf(input, "%d %hX", &size, &addr);

    if (size == 8) {
        byte value;
        sscanf(input, "%*s %*s %hhX", &value);
        LogInfo("Writing %02X to %04X", value, addr);
        WriteByte(addr, value);
    }
    else if (size == 16) {
        word value;
        sscanf(input, "%*s %*s %hX", &value);
        LogInfo("Writing %04X to %04X", value, addr);
        WriteWord(addr, value);
    }
    else {
        LogError("Unsupported size '%d'", size);
    }
}

void cmdDisassemble(const char * input)
{
    unsigned count;
    word addr;

    if (!input || strlen(input) == 0) {
        count = 1;
        addr = R.PC;
    }

    char * space = strchr(input, ' ');
    if (space) {
        *space = '\0';

        sscanf(input, "%d", &count);
        sscanf(space + 1, "%4hX", &addr);
    }
    else {
        sscanf(input, "%d", &count);
        addr = R.PC;
    }

    LogInfo("Disassembling %u instructions at %04X", count, addr);

    char buffer[80];

    int tmpVerbose = VerboseLevel;
    VerboseLevel = 0;

    for (int i = 0; i < count; ++i) {
        word tmp = addr;
        addr = Disassemble(buffer, sizeof(buffer), addr);
        printf("%04X    %s\n", tmp, buffer);
    }

    VerboseLevel = tmpVerbose;

    printf("\n");
}

void DebugPrompt() 
{
#ifdef HAVE_READLINE

    signal(SIGINT, SIG_DFL);

    int oldVerboseLevel = VerboseLevel;
    VerboseLevel = 4;

    PrintR();

    char prompt[2048];
    snprintf(prompt, sizeof(prompt), "[%04X]> ", R.PC);

    word addr;
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
            NextInstruction();
        }
        else if (strncmp(input, "help", length) == 0) {
            cmdHelp(args);
        }
        else if (strncmp(input, "info", length) == 0) {
            cmdInfo(args);
        }
        else if (strncmp(input, "break", length) == 0) {
            cmdBreak(args);
        }
        else if (strncmp(input, "delete", length) == 0) {
            cmdDelete(args);
        }
        else if (strncmp(input, "continue", length) == 0) {
            VerboseLevel = oldVerboseLevel;
            break;
        }
        else if (strncmp(input, "next", length) == 0) {
            cmdNext(args);
        }
        else if (strncmp(input, "read", length) == 0) {
            cmdRead(args);
        }
        else if (strncmp(input, "write", length) == 0) {
            cmdWrite(args);
        }
        else if (strncmp(input, "disassemble", length) == 0) {
            cmdDisassemble(args);
        }
        else if (strncmp(input, "quit", length) == 0
            || strncmp(input, "exit", length) == 0) {
            // TODO: Exit gracefully
            exit(0);
            break;
        }
        else if (strncmp(input, "reset", length) == 0) {
            Reset();
        }

        free(input);
        input = NULL;

        PrintR();
        snprintf(prompt, sizeof(prompt), "[%04X]> ", R.PC);
    }

    free(input);

    signal(SIGINT, handleSignal);
    
#endif
}
