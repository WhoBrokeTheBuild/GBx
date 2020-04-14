#ifndef CMD_HELP_H
#define CMD_HELP_H

#include <GBx/GBx.h>

extern const char * CMD_HELP;
extern const char * CMD_HELP_INFO;
extern const char * CMD_HELP_BREAK;
extern const char * CMD_HELP_DELETE;
extern const char * CMD_HELP_NEXT;
extern const char * CMD_HELP_READ;
extern const char * CMD_HELP_WRITE;
extern const char * CMD_HELP_DISASSEMBLE;

void cmdHelp(gbx_t * ctx, const char * input);

#endif // CMD_HELP_H