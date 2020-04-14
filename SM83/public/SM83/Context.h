#ifndef SM83_CONTEXT_H
#define SM83_CONTEXT_H

#include <SM83/Macros.h>
#include <SM83/Types.h>

typedef struct sm83_internal sm83_internal_t;

typedef struct sm83
{
    // General

    int VerboseLevel;

    bool Enabled;

    // Callbacks

    void * UserData;

    sm83_tick_func_t Tick;

    sm83_read_byte_func_t ReadByte;

    sm83_write_byte_func_t WriteByte;

    // Registers

    union
    {
        struct
        {
            union
            {
                // clang-format off

                SM83_PACK(struct
                {
                    uint8_t :4;
                    bool    FC:1; // Carry Flag
                    bool    FH:1; // Half Carry Flag
                    bool    FN:1; // Subtract Flag
                    bool    FZ:1; // Zero Flag
                });

                // clang-format on

                uint8_t F;
            };

            uint8_t A;
        };

        uint16_t AF;
    };

    union
    {
        struct
        {
            uint8_t C;
            uint8_t B;
        };

        uint16_t BC;
    };

    union
    {
        struct
        {
            uint8_t E;
            uint8_t D;
        };

        uint16_t DE;
    };

    union
    {
        struct
        {
            uint8_t L;
            uint8_t H;
        };

        uint16_t HL;
    };

    uint16_t SP;
    uint16_t PC;

    bool IME;
    bool RequestEnableIME;

    sm83_interrupt_flags_t IF;
    sm83_interrupt_flags_t IE;

    // Internal

    sm83_internal_t * internal;

} sm83_t;

#endif // SM83_CONTEXT_H