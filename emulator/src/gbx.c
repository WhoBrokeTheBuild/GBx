#include "gbx.h"

#include "bootstrap.h"
#include "cpu.h"
#include "debug.h"

int VerboseLevel = 0;

bool running = false;

void reset()
{
    bootstrap();
}

void run()
{
    running = true;
    
    for (; running;) {
        if (atBreakpoint()) {
            debugPrompt();
        } else {
            nextInstruction();
        }
    }
}

void stop()
{
    running = false;
}