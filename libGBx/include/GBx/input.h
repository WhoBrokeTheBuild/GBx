#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

typedef union
{
    struct {
        bool     Right:1;
        bool     Left:1;
        bool     Up:1;
        bool     Down:1;
        bool     SelectDPad:1; 
        unsigned :3;
    };
    struct {
        bool     A:1;
        bool     B:1;
        bool     Select:1;
        bool     Start:1;
        unsigned :1;
        bool     SelectButtons:1;
        unsigned :4;
    };
    uint8_t raw;

} joypad_state_t;

extern joypad_state_t JOYP;

#define JOYP_WRITE_MASK (0b00001100)

#endif // INPUT_H