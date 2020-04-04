#ifndef APU_H
#define APU_H

#include <GBx/Types.h>
#include <GBx/Macros.h>

typedef union
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

} tone_control_t;

#define TONE_READ_MASK0 (0b11111111)
#define TONE_READ_MASK1 (0b00000011)
#define TONE_READ_MASK2 (0b11111111)
#define TONE_READ_MASK3 (0b00000000)
#define TONE_READ_MASK4 (0b00000010)

typedef union
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

} wave_control_t;

#define WAVE_READ_MASK0 (0b00000001)
#define WAVE_READ_MASK1 (0b11111111)
#define WAVE_READ_MASK2 (0b00000110)
#define WAVE_READ_MASK3 (0b00000000)
#define WAVE_READ_MASK4 (0b00000010)

typedef union
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

} noise_control_t;

#define NOISE_READ_MASK0 (0b1111111)
#define NOISE_READ_MASK1 (0b1111111)
#define NOISE_READ_MASK2 (0b1111111)
#define NOISE_READ_MASK3 (0b0000010)

typedef union
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

} volume_control_t;

typedef union 
{
    // clang-format on

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

    // clang-format off

    uint8_t raw;

} sound_output_terminal_t;

typedef union
{
    // clang-format on

    GBX_PACK(struct
    {
        bool Tone1Playing;
        bool Tone2Playing;
        bool WavePlaying;
        bool NoisePlaying;
        uint8_t :2;
        bool Enabled;
    });

    // clang-format off

    uint8_t raw;

} sound_control_t;

#define SOUND_CONTROL_WRITE_MASK (0b0000001)

// $FF10-$FF14
extern tone_control_t Tone1;

extern int Tone1Timer;
extern int Tone1DutyIndex;

// $FF16-$FF19
extern tone_control_t Tone2;

extern int Tone2Timer;
extern int Tone2DutyIndex;

// $FF1A-$FF1E
extern wave_control_t Wave;

extern int WaveTimer;

// $FF30-$FF3F
extern uint8_t WaveRAM[0x10];

// $FF20-$FF23
extern noise_control_t Noise;

extern int NoiseTimer;

// $FF24
extern volume_control_t VolumeControl;

// $FF25
extern sound_output_terminal_t SoundOutputTerminal;

// $FF26
extern sound_control_t APUC;

extern uint8_t DutyCycles[4][8];

void ResetAPU();

void APUInit();
void APUTerm();

void APUTick(unsigned cycles);

void APUPause();
void APUPlay();

const char * GetVolumeCodeString(unsigned index);

void PrintAPUC();
void PrintVolumeControl();
void PrintSoundOutputTerminal();
void PrintTone1();
void PrintTone2();
void PrintWave();
void PrintNoise();
void PrintWaveRAM();

#endif // APU_H