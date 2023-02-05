#pragma once

#ifndef __NVMEM_H
#define __NVMEM_H

#include "eeprom.h"
#include "global.h"

#define NVMSIG0 0x5C
#define NVMSIG1 0x4C
#define NVMSIG ((uint16_t)(NVMSIG0) + ((uint16_t)NVMSIG1 << 8))

// NV memory addr
enum NV_Mem {
  NVMADDR_SIG = 0,
  NVMADDR_BANDCH = 2,
  NVMADDR_BANDFREQ = 7,
  NVMADDR_BAND = 17,
  NVMADDR_VOL,
  NVMADDR_RFMODE,
  NVMADDR_TUNE,
  NVMADDR_STEPIDX,
  NVMADDR_FMFILTER,
  NVMADDR_AMFILTER,
  NVMADDR_SQU1,
  NVMADDR_SQU2,
  NVMADDR_SCSTAY,
  NVMADDR_ANYHOLD,
  NVMADDR_BKADJ,
  NVMADDR_BKKEEP,
  NVMADDR_MISC1,
  NVMADDR_MISC2,
  NVMADDR_MISC3,
  NVMADDR_MISC4,
  NVMADDR_MISC5,
};

#define NVMADDR_LW 100
#define NVMADDR_MW 140
#define NVMADDR_SW 240
#define NVMADDR_FL 440
#define NVMADDR_FM 540

void NVMInitSetting(void);
void NVMGetArgs(void);
void NVMUnpkWrData(const uint8_t *pPGMAddr);

int constrain(int val, int min, int max);

#endif