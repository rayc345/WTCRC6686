#pragma once

#include "main.h"
#include <cmath>
#include <cstdbool>
#include <cstdio>
#include <cstring>

#ifndef __GLOBAL_H
#define __GLOBAL_H

#define DSP_I2C 0xC8

// RF Modes
#define NUM_RFMODES 2
#define RFMODE_FM 0
#define RFMODE_AM 1

// Volume
#define MIN_VOL 0
#define MAX_VOL 29

// Tune types
#define NUM_TYPES 5
#define TYPE_FREQ 0
#define TYPE_SEEK 1
#define TYPE_CH 2
#define TYPE_SCAN 3
#define TYPE_ANY 4
#define TYPE_SCSV 5 // Special tune type for scan & save

// Bands
#define NUM_BANDS 5
#define BAND_LW 0
#define BAND_MW 1
#define BAND_SW 2
#define BAND_FL 3
#define BAND_FM 4

// Channels in band
#define CHS_LW 20
#define CHS_MW 50
#define CHS_SW 100
#define CHS_FL 50
#define CHS_FM 200

// Steps
#define NUM_STEPS 3 // Step values for each band

// Deemphasis
#define DEEMPHOFF 0
#define DEEMPH50 1
#define DEEMPH75 2

// Filters
#define NUM_FM_FILTERS 17 // Auto + 16 fixed bandwidth filters
#define NUM_AM_FILTERS 4  // 4 fixed bandwidth filters
#define DEF_FM_FILTER 0
#define DEF_AM_FILTER 1
#define SEEK_AM_FILTER 1
#define SEEK_FM_FILTER 1

// Need auto sync to NV memory, bits definition
#define NEEDSYNC_BAND 0x00000001
#define NEEDSYNC_VOL 0x00000002
#define NEEDSYNC_RFMODE 0x00000008
#define NEEDSYNC_TUNE 0x00000010
#define NEEDSYNC_STEPIDX 0x00000020
#define NEEDSYNC_FMFILTER 0x00000040
#define NEEDSYNC_AMFILTER 0x00000080
#define NEEDSYNC_MISC1 0x00000100
#define NEEDSYNC_MISC2 0x00000200
#define NEEDSYNC_MISC3 0x00000400
#define NEEDSYNC_MISC4 0x00000400
#define NEEDSYNC_MISC5 0x00000800
#define NEEDSYNC_TONE 0x00001000
#define NEEDSYNC_BALFADER 0x00002000

#define TIMER_INTERVAL 1000 // Check signal level, RSSI SNR and FM stereo indicator every TIMER_INTERVAL, ms
#define TIMER_ALTDISP 3000  // Must be bigger than TIMER_INTERVAL, ms
#define TIMER_AUTOSYNC 5000 // Must be bigger than TIMER_INTERVAL, ms

inline constexpr int32_t nBandFMin[NUM_BANDS] =
    {144, 522, 2300, 76000, 87000};
inline constexpr int32_t nBandFMax[NUM_BANDS] =
    {288, 1710, 27000, 86990, 108000};
inline constexpr int16_t nBandStep[NUM_BANDS][NUM_STEPS] =
    {
        {9, 1, 45},
        {9, 1, 90},
        {5, 1, 500},
        {50, 10, 500},
        {100, 10, 500}};
inline constexpr uint8_t nBandChs[NUM_BANDS] =
    {CHS_LW, CHS_MW, CHS_SW, CHS_FL, CHS_FM}; // Band total channels

inline constexpr uint16_t M_FMFilter[] =
    {0, 56, 64, 72, 84, 97, 114, 133, 151, 168, 184, 200, 217, 236, 254, 287, 311};
inline constexpr uint8_t M_AMFilter[] =
    {3, 4, 6, 8};
inline constexpr int16_t M_Volume[] =
    {-300, -250, -200, -150, -120, -110 - 100, -90, -80, -70, -60, -55, -50, -45, -40, -35, -30, -25, -20, -15, -10, -8, -6, -5, -4, -3, -2, -1, 0, 5, 10};

inline volatile int8_t nLRot;
inline volatile int8_t nRRot;

inline uint8_t nBand;     // Band: LW/MW/SW/FL/FM
inline uint8_t nVolume;   // Audio volume control, 0-29
inline uint8_t nRFMode;   // FM/AM
inline uint8_t nTuneType; // Freq_Step/Seek/Ch/Scan/Any/Scan_Save
inline uint8_t nStepIdx;  // Step index for current band
inline uint8_t nFMFilter; // Current FIR filter index for FM
inline uint8_t nAMFilter; // Current FIR filter index for AM

inline uint8_t nDeemphasis; // FM de-emphasis, 0=off, 1=50us, 2=75us
inline uint8_t nDIGRA;      // FM/AM Digital radio, 0=off, 1=on
inline uint8_t nLowerSig;   // Normal/reduced signal quality for seek/scan/any, 0=normal, 1=lower
inline uint8_t nFMEMS;      // FM improved multipath suppression, 0=off, 1=on
inline uint8_t nFMCEQ;      // FM channel equalizer, 0=off, 1=on

inline int8_t nSquelch[2]; // Signal squelch value in dBuv, -20~99
                           // 1st item for auto mute, 2nd item for seek/scan/any

inline uint8_t nScanStayTime; // Seconds to stay at current frequency
inline uint8_t nAnyHoldTime;  // Seconds to hold current frequency after lost signal

inline uint8_t nBacklightAdj;  // LCD backlight value, 0-255
inline uint8_t nBacklightKeep; // LCD backlight auto keep seconds, 0-255, 0 for always on

inline int16_t nBandCh[NUM_BANDS];
inline int32_t nBandFreq[NUM_BANDS]; // Band default frequency

inline bool bLCDOff = false;                     // true for LCD is off
inline uint32_t nBacklightTimer;                 // LCD backlight auto keep timer in ms
inline int32_t nSecondsOffset = 3600L * 24 * 15; // Seconds of real time offset, preset to 15 days

inline int8_t nRSSI; // Received signal strength, in dBuv
inline bool bSTIN;   // FM stereo indicator

#endif