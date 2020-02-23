#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <stdint.h>

typedef union {
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
    uint8_t data[5];
} tone_t;

extern tone_t Tone1;
extern tone_t Tone2;

typedef union {
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
    uint8_t data[5];
} wave_t;

extern wave_t Wave;

typedef union {
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
    uint8_t data[4];
} noise_t;

extern noise_t Noise;

typedef union {
    struct {
        uint8_t RightVolume:3;
        bool    RightVinEnable:1;
        uint8_t LeftVolume:3;
        bool    LeftVinEnable:1;
    };
    uint8_t data;
} volume_control_t;

extern volume_control_t VolumeControl;


extern uint8_t WaveRAM[0x10];

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

#endif // SOUND_H