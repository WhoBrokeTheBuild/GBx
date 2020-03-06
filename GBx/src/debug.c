#include "debug.h"

#include <limits.h>
#include <stdlib.h>

#include "apu.h"
#include "cartridge.h"
#include "cpu.h"
#include "interrupt.h"
#include "lcd.h"
#include "log.h"
#include "memory.h"
#include "timer.h"

#include <signal.h>
#include <stdarg.h>

#ifdef HAVE_READLINE
    #include <readline/readline.h>
    #include <readline/history.h>

    #define HISTORY_FILE ".gbx_history"
#endif

bool DebugEnable = false;

uint16_t breakpointAddress = UINT16_MAX;

void handleSignal(int sig)
{
    LogInfo("Caught signal %d", sig);

    debugPrompt();

    if (sig == SIGSEGV) {
        exit(1);
    }
}

void debugInit()
{
    signal(SIGINT, handleSignal);
    signal(SIGSEGV, handleSignal);

    #ifdef HAVE_READLINE
        read_history(HISTORY_FILE);
    #endif
}

void debugTerm()
{   
    #ifdef HAVE_READLINE
        write_history(HISTORY_FILE);
    #endif
    
    signal(SIGINT, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
}

void setBreakpoint(uint16_t address)
{
    breakpointAddress = address;
}

void clearBreakpoint()
{
    breakpointAddress = UINT16_MAX;
}

bool atBreakpoint()
{
    return (R.PC == breakpointAddress);
}

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
"  apu          Print tone, wave, noise, and volume registers\n"
"  registers    Print all registers and their values\n"
"  interrupts   Print IE and IF, and IME values\n"
"  lcd          Print LCDC values and LCD info\n"
"  lcd          Print LCDC values and LCD info\n"
"  stat         Print STAT values\n"
"  timer        Print TAC, TIMA, TMA and DIV info\n"
"  bank         Print ROM/RAM Bank info\n"
"  cartridge    Print Cartridge info\n";

const char * helpBreak = 
"  break ADDRESS\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
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

void infoPrompt(const char * input)
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

    if (strncmp(input, "apu", length) == 0) {
        printTone1();
        printTone2();
        printWave();
        printWaveRAM();
        printVolumeControl();
    }
    else if (strncmp(input, "registers", length) == 0) {
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

void breakPrompt(const char * input)
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
        // *equal = '\0';

        // unsigned int rhs;
        // sscanf(equal + 1, "%04X", &rhs);

        // LogInfo("Breakpoint set when %s=%04Xh", input, rhs);
        // addBreakpoint(cond, rhs);
    }
    else {
        unsigned int pc;
        sscanf(input, "%04X", &pc);
        setBreakpoint(pc);
        LogInfo("Breakpoint set when PC=%04Xh", pc);
    }
}

void readPrompt(const char * input)
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

void writePrompt(const char * input)
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

void helpPrompt(const char * input)
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
#ifdef HAVE_READLINE

    int oldVerboseLevel = VerboseLevel;
    VerboseLevel = 4;

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
        else if (strncmp(input, "quit", length) == 0
            || strncmp(input, "exit", length) == 0) {
            // TODO: Exit gracefully
            exit(0);
            break;
        }
        else if (strncmp(input, "continue", length) == 0) {
            VerboseLevel = oldVerboseLevel;
            break;
        }
        else if (strncmp(input, "info", length) == 0) {
            infoPrompt(args);
        }
        else if (strncmp(input, "break", length) == 0) {
            breakPrompt(args);
        }
        else if (strncmp(input, "get", length) == 0) {
            // TODO
        }
        else if (strncmp(input, "set", length) == 0) {
            // TODO
        }
        else if (strncmp(input, "reset", length) == 0) {
            // TODO
            // reset();
        }
        else if (strncmp(input, "read", length) == 0) {
            readPrompt(args);
        }
        else if (strncmp(input, "write", length) == 0) {
            writePrompt(args);
        }
        else if (strncmp(input, "help", length) == 0) {
            helpPrompt(args);
        }

        free(input);
        input = NULL;

        snprintf(prompt, sizeof(prompt), "[%04X]> ", R.PC);
    }

    free(input);

#endif
}

void showinstructionLogWindow()
{

}

void showStatusWindow()
{

}

void showVideoRAMWindow()
{

}