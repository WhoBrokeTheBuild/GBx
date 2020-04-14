#ifndef GBX_TYPES_APU_H
#define GBX_TYPES_APU_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <GBx/Macros.h>

typedef union gbx_tone_control
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t  Shift:3;
        bool     Negate:1;
        uint8_t  SweepPeriod:3;
        uint8_t  :1;
        uint8_t  LengthLoad:6;
        uint8_t  Duty:2;
        uint8_t  Period:3;
        bool     EnvelopeAddMode:1;
        uint8_t  Volume:4;
        uint16_t Frequency:11;
        uint8_t  :3;
        bool     LengthEnabled:1;
        bool     Trigger:1;
    });

    // clang-format on

    uint8_t raw[5];

} gbx_tone_control_t;

static_assert(sizeof(gbx_tone_control_t) == 5,
    "sizeof(gbx_tone_control_t) != 5");

#define GBX_TONE_READ_MASK0 (0b11111111)
#define GBX_TONE_READ_MASK1 (0b00000011)
#define GBX_TONE_READ_MASK2 (0b11111111)
#define GBX_TONE_READ_MASK3 (0b00000000)
#define GBX_TONE_READ_MASK4 (0b00000010)

typedef union gbx_wave_control
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t  :7;
        bool     DACPower:1;
        uint8_t  LengthLoad;
        uint8_t  :5;
        uint8_t  VolumeCode:2;
        uint8_t  :1;
        uint16_t Frequency:11;
        uint8_t  :3;
        bool     LengthEnabled:1;
        bool     Trigger:1;
    });

    // clang-format on

    uint8_t raw[5];

} gbx_wave_control_t;

static_assert(sizeof(gbx_wave_control_t) == 5,
    "sizeof(gbx_wave_control_t) != 5");

#define GBX_WAVE_READ_MASK0 (0b00000001)
#define GBX_WAVE_READ_MASK1 (0b11111111)
#define GBX_WAVE_READ_MASK2 (0b00000110)
#define GBX_WAVE_READ_MASK3 (0b00000000)
#define GBX_WAVE_READ_MASK4 (0b00000010)

typedef union gbx_noise_control
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t LengthLoad:6;
        uint8_t :2;
        uint8_t Period:3;
        bool    EnvelopeAddMode:1;
        uint8_t Volume:4;
        uint8_t DivisorCode:3;
        bool    LFSRWidthMode:1;
        uint8_t ClockShift:4;
        uint8_t :6;
        bool    LengthEnabled:1;
        bool    Trigger:1;
    });

    // clang-format on

    uint8_t raw[4];

} gbx_noise_control_t;

static_assert(sizeof(gbx_noise_control_t) == 4,
    "sizeof(gbx_noise_control_t) != 4");

#define GBX_NOISE_READ_MASK0 (0b1111111)
#define GBX_NOISE_READ_MASK1 (0b1111111)
#define GBX_NOISE_READ_MASK2 (0b1111111)
#define GBX_NOISE_READ_MASK3 (0b0000010)

typedef union gbx_volume_control
{
    // clang-format off

    GBX_PACK(struct
    {
        uint8_t RightVolume:3;
        bool    RightVinEnabled:1;
        uint8_t LeftVolume:3;
        bool    LeftVinEnabled:1;
    });

    // clang-format on

    uint8_t raw;

} gbx_volume_control_t;

static_assert(sizeof(gbx_volume_control_t) == 1,
    "sizeof(gbx_volume_control_t) != 1");

typedef union gbx_sound_output_terminal
{
    // clang-format off

    GBX_PACK(struct
    {
        bool Tone1ToSO1:1;
        bool Tone2ToSO1:1;
        bool WaveToSO1:1;
        bool NoiseToSO1:1;
        bool Tone1ToSO2:1;
        bool Tone2ToSO2:1;
        bool WaveToSO2:1;
        bool NoiseToSO2:1;
    });

    // clang-format on

    uint8_t raw;

} gbx_sound_output_terminal_t;

static_assert(sizeof(gbx_sound_output_terminal_t) == 1,
    "sizeof(gbx_sound_output_terminal_t) != 1");

typedef union gbx_sound_control
{
    // clang-format off

    GBX_PACK(struct
    {
        bool Tone1Playing:1;
        bool Tone2Playing:1;
        bool WavePlaying:1;
        bool NoisePlaying:1;
        uint8_t :2;
        bool Enabled:1;
    });

    // clang-format on

    uint8_t raw;

} gbx_sound_control_t;

static_assert(sizeof(gbx_sound_control_t) == 1,
    "sizeof(gbx_sound_control_t) != 1");

#define GBX_APUC_WRITE_MASK (0b0000001)

#endif // GBX_TYPES_APU_H