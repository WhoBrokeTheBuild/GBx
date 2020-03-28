#ifndef INPUT_H
#define INPUT_H

#include "types.h"

typedef union
{
    struct {
        bool Right:1;
        bool Left:1;
        bool Up:1;
        bool Down:1;
        uint :4;
    };
    struct {
        bool A:1;
        bool B:1;
        bool Select:1;
        bool Start:1;
        uint :4;
    };
    struct {
        uint InputStates:4;
        bool SelectDPad:1; 
        bool SelectButtons:1;
        uint :2;
    };
    byte raw;

} joypad_state_t;

extern bool RightPressed;
extern bool LeftPressed;
extern bool UpPressed;
extern bool DownPressed;
extern bool APressed;
extern bool BPressed;
extern bool SelectPressed;
extern bool StartPressed;

extern joypad_state_t JOYP;

#define JOYP_WRITE_MASK (0b00110000)

void ResetInput();

void UpdateInput();

void PrintInput();

#endif // INPUT_H