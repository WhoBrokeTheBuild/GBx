#ifndef JOYPAD_H
#define JOYPAD_H

#include <GBx/Macros.h>
#include <GBx/Types.h>

typedef union
{
    // clang-format off

    GBX_PACK(struct
    {
        bool    Right:1;
        bool    Left:1;
        bool    Up:1;
        bool    Down:1;
        uint8_t :4;
    });

    GBX_PACK(struct
    {
        bool    A:1;
        bool    B:1;
        bool    Select:1;
        bool    Start:1;
        uint8_t :4;
    });

    GBX_PACK(struct
    {
        uint8_t InputStates:4;
        bool    SelectDPad:1; 
        bool    SelectButtons:1;
        uint8_t :2;
    });

    // clang-format on

    uint8_t raw;

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

void ResetJoypad();

void UpdateJoypad();

void PrintJoypad();

#endif // JOYPAD_H