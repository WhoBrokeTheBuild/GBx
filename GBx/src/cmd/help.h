#ifndef CMD_HELP_H
#define CMD_HELP_H

extern const char * CMD_HELP;
extern const char * CMD_HELP_INFO;
extern const char * CMD_HELP_BREAK;
extern const char * CMD_HELP_DELETE;
extern const char * CMD_HELP_NEXT;
extern const char * CMD_HELP_READ;
extern const char * CMD_HELP_WRITE;
extern const char * CMD_HELP_DISASSEMBLE;

void cmdHelp(const char * input);

#endif // CMD_HELP_H