#include "help.h"

#include <stdio.h>
#include <string.h>

const char * CMD_HELP = 
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

const char * CMD_HELP_INFO = 
"  all          Print all information\n"
"  apu          Print Tone1, Tone2, Wave, Noise, and APU Control registers\n"
"  registers    Print all registers and their values\n"
"  interrupts   Print IE and IF, and IME values\n"
"  lcd          Print LCDC, STAT registers and LCD coordinates\n"
"  timer        Print TAC, TIMA, TMA and DIV info\n"
"  bank         Print ROM/RAM Bank info\n"
"  cartridge    Print Cartridge info\n"
"  input        Print Input info\n";

const char * CMD_HELP_BREAK = 
"break [CONDITION|ADDRESS]\n"
"\n"
"  Set a breakpoint at ADDRESS, or when CONDITION is met\n"
"\n"
"  CONDITION will be interpreted either as A=B\n"
"  where B is a hex number, and A can be one of the following:\n"
"    A, B, C, D, E, H, L, AF, BC, DE, HL, SP, PC, FZ, FN, FH, FC, LY\n"
"  CONDITION can also be SPCHNG, which will break when the stack changes.\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"\n"
"  If no CONDITION or ADDRESS are specified, all breakpoints\n"
"  will be listed\n"
"\n";

const char * CMD_HELP_DELETE = 
"delete [ADDRESS]\n"
"\n"
"  Delete a breakpoint at ADDRESS, or all breakpoints\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"  If ADDRESS is not specified, all breakpoints will be deleted\n"
"\n";

const char * CMD_HELP_NEXT = 
"next [COUNT]\n"
"\n"
"  Run the next COUNT instructions\n"
"\n"
"  COUNT will be interpreted as an unsigned decimal integer\n"
"  If COUNT is not present, it will default to 1\n";

const char * CMD_HELP_READ = 
"read SIZE ADDRESS\n"
"\n"
"  Read SIZE bytes from ADDRESS\n"
"\n"
"  SIZE can be either 1 or 2\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n";

const char * CMD_HELP_WRITE = 
"write SIZE ADDRESS VALUE\n"
"\n"
"  Write SIZE bytes to ADDRESS from VALUE\n"
"\n"
"  SIZE can be 1 or 2\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"\n"
"  VALUE will be interpreted as an 8-bit or 16-bit number, depending on SIZE\n";

const char * CMD_HELP_DISASSEMBLE = 
"disassemble [COUNT] [ADDRESS]\n"
"\n"
"  Dissassmble COUNT instructions at ADDRESS\n"
"\n"
"  COUNT will be interpreted as an unsigned decimal integer\n"
"  If COUNT is not present, it will default to 1\n"
"\n"
"  ADDRESS will be interpreted as a 16-bit hex number\n"
"  If ADDRESS is not present, it will default to PC\n";

void cmdHelp(gbx_t * ctx, const char * input)
{
    if (!input) {
        printf("%s", CMD_HELP);
        return;
    }

    size_t length = strlen(input);
    
    if (length == 0) {
        printf("%s", CMD_HELP);
        return;
    }

    if (strncmp(input, "info", length) == 0) {
        printf("%s", CMD_HELP_INFO);
    }
    else if (strncmp(input, "break", length) == 0) {
        printf("%s", CMD_HELP_BREAK);
    }
    else if (strncmp(input, "delete", length) == 0) {
        printf("%s", CMD_HELP_DELETE);
    }
    else if (strncmp(input, "next", length) == 0) {
        printf("%s", CMD_HELP_NEXT);
    }
    else if (strncmp(input, "read", length) == 0) {
        printf("%s", CMD_HELP_READ);
    }
    else if (strncmp(input, "write", length) == 0) {
        printf("%s", CMD_HELP_WRITE);
    }
    else {
        printf("Unknown command '%s'", input);
    }
}
