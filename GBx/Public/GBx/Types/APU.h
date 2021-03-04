#ifndef GBX_TYPES_APU_H
#define GBX_TYPES_APU_H

#include <GBx/Macros.h>

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union GBx_ToneControl
{
    GBX_PACK(struct {
        // Number of frequency sweeps
        uint8_t  FrequencySweepCount:3;

        // 0 = Addition, 1 = Subtraction
        uint8_t  FrequencySweepNegate:1;

        // Frequency change = (FrequencySweepPeriod / 128) Hz
        uint8_t  FrequencySweepPeriod:3;

        // The chance in frequency at each shift is:
        //   X(t) = X(t-1) +/- X(t-1)/2^n

        uint8_t  :1;

        // Sound Length = (64 - Length) * (1/256) Sec
        // Enabled if (LegnthEnabled == 1)
        uint8_t  Length:6;

        // Duty Cycle:
        //   00 - 12.5%
        //   01 - 25%
        //   10 - 50%
        //   11 - 75%
        uint8_t  DutyCode:2;

        // Number of envelope sweeps
        uint8_t  EnvelopeSweepCount:3;

        // 0 = Decrease, 1 = Increase
        uint8_t  EnvelopeSweepDirection:1;

        // Initial volume of envelope
        uint8_t  EnvelopeInitialVolume:4;

        // Sound Frequency = 131072 / (2048 - Frequency) Hz
        uint16_t Frequency:11;

        uint8_t  :3;

        // Enables Length
        uint8_t  LengthEnabled:1;

        // Writing 1 to this will restart the sound
        uint8_t  Trigger:1;
    });

    uint8_t _raw[5];

} GBx_ToneControl;

static_assert(
    sizeof(GBx_ToneControl) == 5,
    "sizeof(GBx_ToneControl) != 5"
);

#define GBX_TONE_READ_MASK_0    0b10000000
#define GBX_TONE_READ_MASK_1    0b00111111
#define GBX_TONE_READ_MASK_2    0b00000000
#define GBX_TONE_READ_MASK_3    0b11111111
#define GBX_TONE_READ_MASK_4    0b10111111

typedef union GBx_WaveControl
{
    GBX_PACK(struct {
        uint8_t  :7;

        // 0 = Stopped, 1 = Playing
        uint8_t  Enable:1;

        // Sound Length = (256 - Length) * (1/256) Sec
        // Enabled if (LegnthEnabled == 1)
        uint8_t  Length;

        uint8_t  :5;

        // Volume:
        //   00 - 0%
        //   01 - 100%
        //   10 - 50%
        //   11 - 25%
        uint8_t  VolumeCode:2;

        uint8_t  :1;

        // Frequency = 65536 / (2048 - Frequency) Hz
        uint16_t Frequency:11;
        uint8_t  :3;

        // Enables Length
        uint8_t  LengthEnabled:1;

        // Writing 1 to this will restart the sound
        uint8_t  Trigger:1;
    });

    uint8_t _raw[5];

} GBx_WaveControl;

static_assert(
    sizeof(GBx_WaveControl) == 5,
    "sizeof(GBx_WaveControl) != 5"
);

#define GBX_WAVE_READ_MASK_0    0b01111111
#define GBX_WAVE_READ_MASK_1    0b11111111
#define GBX_WAVE_READ_MASK_2    0b10011111
#define GBX_WAVE_READ_MASK_3    0b11111111
#define GBX_WAVE_READ_MASK_4    0b10111111

typedef union GBx_NoiseControl
{
    GBX_PACK(struct {
        // Length = (64 - Length) * (1/256) Sec
        // Enabled if (LegnthEnabled == 1)
        uint8_t Length:6;
        uint8_t :2;

        // Length per Step = EnvelopeSweep * (1/64) Sec
        // Disabled if (EnvelopeSweep == 0)
        uint8_t EnvelopeSweep:3;

        // Decrease = 0, Increase = 1
        uint8_t EnvelopeDirection:1;

        uint8_t Volume:4;
        uint8_t DivisorCode:3;
        uint8_t LFSRWidthMode:1;
        uint8_t ClockShift:4;
        uint8_t :6;
        uint8_t LengthEnabled:1;
        uint8_t Trigger:1;
    });

    uint8_t _raw[4];

} GBx_NoiseControl;

static_assert(
    sizeof(GBx_NoiseControl) == 4,
    "sizeof(GBx_NoiseControl) != 4"
);

#define GBX_NOISE_READ_MASK_0   0b11111111
#define GBX_NOISE_READ_MASK_1   0b00000000
#define GBX_NOISE_READ_MASK_2   0b00000000
#define GBX_NOISE_READ_MASK_3   0b10111111

// Right = SO1
// Left  = SO2

typedef union GBx_VolumeControl
{
    GBX_PACK(struct {
        uint8_t RightVolume:3;
        uint8_t RightVinEnabled:1;
        uint8_t LeftVolume:3;
        uint8_t LeftVinEnabled:1;
    });

    uint8_t _raw;

} GBx_VolumeControl;

static_assert(
    sizeof(GBx_VolumeControl) == 1,
    "sizeof(GBx_VolumeControl) != 1"
);

typedef union GBx_OutputTerminalControl
{
    GBX_PACK(struct {
        uint8_t RightTone1:1;
        uint8_t RightTone2:1;
        uint8_t RightWave:1;
        uint8_t RightNoise:1;
        uint8_t LeftTone1:1;
        uint8_t LeftTone2:1;
        uint8_t LeftWave:1;
        uint8_t LeftNoise:1;
    });

    uint8_t _raw;

} GBx_OutputTerminalControl;

static_assert(
    sizeof(GBx_OutputTerminalControl) == 1,
    "sizeof(GBx_OutputTerminalControl) != 1"
);

typedef union GBx_APUControl
{
    GBX_PACK(struct {
        uint8_t PlayingTone1:1;
        uint8_t PlayingTone2:1;
        uint8_t PlayingWave:1;
        uint8_t PlayingNoise:1;
        uint8_t :2;
        uint8_t Enabled:1;
    });

    uint8_t _raw;

} GBx_APUControl;

static_assert(
    sizeof(GBx_APUControl) == 1,
    "sizeof(GBx_APUControl) != 1"
);

#define GBX_APUC_WRITE_MASK 0b10000000

#ifdef __cplusplus
}
#endif

#endif // GBX_TYPES_APU_H