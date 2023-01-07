#pragma once
#include "global.h"

#ifndef __TUNER_H
#define __TUNER_H

void SetVolume(uint8_t);
void CheckVolume(void);
void dsp_set_filter(int8_t f);
void NextFilter(void);
void SetFilter(bool bNeedSync);
void SetRFCtrlReg(void);
bool TuneFreq(int32_t);
void AdjFreq(bool bCurrStep);
void TuneFreqDisp(void);
void SetDigiRadio(void);
uint32_t ReadChFreq(void);
void WriteChFreq(bool bAdd);
void SeekCh(int8_t nDir);
void ProcBand(uint8_t nBd);
void ProcStepFilter(uint8_t nKey);
void GetRFStatReg(void);
bool IsSigOK(void);
int8_t Seek(int8_t nDir);
uint8_t ScanAny(void);
void SetRFMode(void);
void AddSyncBits(uint32_t SyncBit);
void TunerLoop(void);

#ifdef __cplusplus
extern "C" {
#endif

void mainTuner();

#ifdef __cplusplus
}
#endif

#endif