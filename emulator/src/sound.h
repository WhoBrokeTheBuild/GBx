#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>
#include <stdbool.h>

typedef union {
    struct {
        uint8_t Shift:3;
        bool Negate:1;
        uint8_t SweepPeriod:3;
        uint8_t _2:1;
        uint8_t LengthLoad:6;
        uint8_t Duty:2;
        uint8_t Period:3;
        bool EnvelopeAddMode:1;
        uint8_t Volume:4;
        uint16_t Frequency:11;
        uint8_t _1:3;
        bool LengthEnable:1;
        bool Trigger:1;
    };
    uint8_t data[5];
} square_wave_t;

extern square_wave_t Wave1;
extern square_wave_t Wave2;

typedef union {
    struct {
        uint8_t _1:7;
        bool DACPower:1;
        uint8_t LengthLoad;
        uint8_t _2:5;
        uint8_t VolumeCode:2;
        uint8_t _3:1;
        uint16_t Frequency:11;
        uint8_t _4:3;
        bool LengthEnable:1;
        bool Trigger:1;
    };
    uint8_t data[5];
} wave_t;

extern wave_t Wave3;

typedef union {
    struct {
        uint8_t LengthLoad:6;
        uint8_t _1:2;
        uint8_t Period:3;
        bool EnvelopeAddMode:1;
        uint8_t Volume:4;
        uint8_t DivisorCode:3;
        bool LFSRWidthMode:1;
        uint8_t ClockShift:4;
        uint8_t _2:6;
        bool LengthEnable:1;
        bool Trigger:1;
    };
    uint8_t data[4];
} noise_t;

extern noise_t Noise;

typedef union {
    struct {
        uint8_t RightVolume:3;
        bool RightVinEnable:1;
        uint8_t LeftVolume:3;
        bool LeftVinEnable:1;
    };
    uint8_t data;
} volume_control_t;

extern volume_control_t VolumeControl;

extern uint8_t WaveRAM[0x10];

void printWave1();
void printWave2();
void printWave3();
void printNoise();
void printVolumeControl();
void printWaveRAM();

#endif // SOUND_H