#ifndef APU_H
#define APU_H

#include <stdbool.h>
#include <stdint.h>

typedef union
{
    struct {
        unsigned Shift:3;
        bool     Negate:1;
        unsigned SweepPeriod:3;
        unsigned :1;
        unsigned LengthLoad:6;
        unsigned Duty:2;
        unsigned Period:3;
        bool     EnvelopeAddMode:1;
        unsigned Volume:4;
        unsigned Frequency:11;
        unsigned :3;
        bool     LengthEnable:1;
        bool     Trigger:1;
    };
    uint8_t raw[5];

} tone_t;

#define TONE_READ_MASK0 (0b11111111)
#define TONE_READ_MASK1 (0b00000011)
#define TONE_READ_MASK2 (0b11111111)
#define TONE_READ_MASK3 (0b00000000)
#define TONE_READ_MASK4 (0b00000010)

typedef union
{
    struct {
        unsigned :7;
        bool     DACPower:1;
        unsigned LengthLoad;
        unsigned :5;
        unsigned VolumeCode:2;
        unsigned :1;
        unsigned Frequency:11;
        unsigned :3;
        bool     LengthEnable:1;
        bool     Trigger:1;
    };
    uint8_t raw[5];

} wave_t;

#define WAVE_READ_MASK0 (0b00000001)
#define WAVE_READ_MASK1 (0b11111111)
#define WAVE_READ_MASK2 (0b00000110)
#define WAVE_READ_MASK3 (0b00000000)
#define WAVE_READ_MASK4 (0b00000010)

typedef union
{
    struct {
        unsigned LengthLoad:6;
        unsigned :2;
        unsigned Period:3;
        bool     EnvelopeAddMode:1;
        unsigned Volume:4;
        unsigned DivisorCode:3;
        bool     LFSRWidthMode:1;
        unsigned ClockShift:4;
        unsigned :6;
        bool     LengthEnable:1;
        bool     Trigger:1;
    };
    uint8_t raw[4];

} noise_t;

#define NOISE_READ_MASK0 (0b1111111)
#define NOISE_READ_MASK1 (0b1111111)
#define NOISE_READ_MASK2 (0b1111111)
#define NOISE_READ_MASK3 (0b0000010)

typedef union
{
    struct {
        unsigned RightVolume:3;
        bool     RightVinEnable:1;
        unsigned LeftVolume:3;
        bool     LeftVinEnable:1;
    };
    uint8_t raw;

} volume_control_t;

typedef union 
{
    struct {
        bool Tone1ToSO1;
        bool Tone2ToSO1;
        bool WaveToSO1;
        bool NoiseToSO1;
        bool Tone1ToSO2;
        bool Tone2ToSO2;
        bool WaveToSO2;
        bool NoiseToSO2;
    };
    uint8_t raw;

} sound_output_terminal_t;

typedef union
{
    struct {
        bool     Tone1Playing;
        bool     Tone2Playing;
        bool     WavePlaying;
        bool     NoisePlaying;
        unsigned :2;
        bool     SoundEnable;
    };
    uint8_t raw;

} sound_control_t;

#define SOUND_CONTROL_WRITE_MASK (0b0000001)

// FF10-FF14

extern tone_t Tone1;

extern int Tone1Timer;
extern int Tone1DutyIndex;

// FF16-FF19

extern tone_t Tone2;

extern int Tone2Timer;
extern int Tone2DutyIndex;

// FF1A-FF1E

extern wave_t Wave;

extern int WaveTimer;

// FF30-FF3F

extern uint8_t WaveRAM[0x10];

// FF20-FF23

extern noise_t Noise;

extern int NoiseTimer;

// FF24

extern volume_control_t VolumeControl;

// FF25

extern sound_output_terminal_t SoundOutputTerminal;

// FF26

extern sound_control_t SoundControl;

extern uint8_t DutyCycles[4][8];


void apuInit();
void apuTerm();

void apuTick(unsigned cycles);

void apuPause();
void apuPlay();

void printTone1();
void printTone2();
void printWave();
void printNoise();
void printVolumeControl();
void printWaveRAM();

#endif // APU_H