#ifndef APU_H
#define APU_H

#include <GBx/Types.h>

typedef union
{
    struct {
        uint Shift:3;
        bool Negate:1;
        uint SweepPeriod:3;
        uint :1;
        uint LengthLoad:6;
        uint Duty:2;
        uint Period:3;
        bool EnvelopeAddMode:1;
        uint Volume:4;
        uint Frequency:11;
        uint :3;
        bool LengthEnabled:1;
        bool Trigger:1;
    };
    byte raw[5];

} tone_control;

#define TONE_READ_MASK0 (0b11111111)
#define TONE_READ_MASK1 (0b00000011)
#define TONE_READ_MASK2 (0b11111111)
#define TONE_READ_MASK3 (0b00000000)
#define TONE_READ_MASK4 (0b00000010)

typedef union
{
    struct {
        uint :7;
        bool DACPower:1;
        uint LengthLoad;
        uint :5;
        uint VolumeCode:2;
        uint :1;
        uint Frequency:11;
        uint :3;
        bool LengthEnabled:1;
        bool Trigger:1;
    };
    byte raw[5];

} wave_control;

#define WAVE_READ_MASK0 (0b00000001)
#define WAVE_READ_MASK1 (0b11111111)
#define WAVE_READ_MASK2 (0b00000110)
#define WAVE_READ_MASK3 (0b00000000)
#define WAVE_READ_MASK4 (0b00000010)

typedef union
{
    struct {
        uint LengthLoad:6;
        uint :2;
        uint Period:3;
        bool EnvelopeAddMode:1;
        uint Volume:4;
        uint DivisorCode:3;
        bool LFSRWidthMode:1;
        uint ClockShift:4;
        uint :6;
        bool LengthEnabled:1;
        bool Trigger:1;
    };
    byte raw[4];

} noise_control;

#define NOISE_READ_MASK0 (0b1111111)
#define NOISE_READ_MASK1 (0b1111111)
#define NOISE_READ_MASK2 (0b1111111)
#define NOISE_READ_MASK3 (0b0000010)

typedef union
{
    struct {
        uint RightVolume:3;
        bool RightVinEnabled:1;
        uint LeftVolume:3;
        bool LeftVinEnabled:1;
    };
    byte raw;

} volume_control;

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
    byte raw;

} sound_output_terminal;

typedef union
{
    struct {
        bool     Tone1Playing;
        bool     Tone2Playing;
        bool     WavePlaying;
        bool     NoisePlaying;
        uint :2;
        bool     Enabled;
    };
    byte raw;

} sound_control;

#define SOUND_CONTROL_WRITE_MASK (0b0000001)

// $FF10-$FF14
extern tone_control Tone1;

extern int Tone1Timer;
extern int Tone1DutyIndex;

// $FF16-$FF19
extern tone_control Tone2;

extern int Tone2Timer;
extern int Tone2DutyIndex;

// $FF1A-$FF1E
extern wave_control Wave;

extern int WaveTimer;

// $FF30-$FF3F
extern byte WaveRAM[0x10];

// $FF20-$FF23
extern noise_control Noise;

extern int NoiseTimer;

// $FF24
extern volume_control VolumeControl;

// $FF25
extern sound_output_terminal SoundOutputTerminal;

// $FF26
extern sound_control APUC;

extern byte DutyCycles[4][8];

void ResetAPU();

void APUInit();
void APUTerm();

void APUTick(uint cycles);

void APUPause();
void APUPlay();

const char * GetVolumeCodeString(uint index);

void PrintAPUC();
void PrintVolumeControl();
void PrintSoundOutputTerminal();
void PrintTone1();
void PrintTone2();
void PrintWave();
void PrintNoise();
void PrintWaveRAM();

#endif // APU_H