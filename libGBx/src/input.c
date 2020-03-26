#include "input.h"

joypad_state_t JOYP;

void ResetInput()
{
    JOYP.raw = 0xFF;
}