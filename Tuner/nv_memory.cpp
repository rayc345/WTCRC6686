#include "nv_memory.h"
#include "ui.h"

// NV memory initialization data
const uint8_t NVM_INIT[] =
    {
        2, NVMSIG0, NVMSIG1,
        0x80 + 5, 0,             // Band ch No.
        0x80 + 10, 0xff,         // Band frequency
        7, 4, 10, 0, 0, 0, 0, 0, // Band, Vol, RFMode, TuneType, StepIdx, FMFilter, AMFilter
        6, 15, 30, 5, 3, 255, 0, // Squ1, Squ2, TScan, TAny, BkAdj, BkKeep,
        5, 0x7D, 0, 0, 0, 0,     // Misc1, Misc2, Misc3, Misc4, Misc5
        0                        // End
};

void NVMInitSetting(void) {
  printf("NVMInitSettings: %4X", eeprom_read_word(NVMADDR_SIG));
  LCDXYStr(0, 1, "Init Setting ...");
  NVMUnpkWrData(NVM_INIT); // Initialize NV memory
}

void NVMGetArgs(void) {
  uint8_t u8, i;
  uint16_t u16;

  if (eeprom_read_word(NVMADDR_SIG) != NVMSIG)
    NVMInitSetting();

  nBand = eeprom_read_byte(NVMADDR_BAND);
  if (nBand >= NUM_BANDS)
    nBand = BAND_FM;

  nVolume = eeprom_read_byte(NVMADDR_VOL);
  if (nVolume > MAX_VOL)
    nVolume = 10;

  nRFMode = eeprom_read_byte(NVMADDR_RFMODE);
  if (nRFMode >= NUM_RFMODES)
    nRFMode = RFMODE_FM;

  nTuneType = eeprom_read_byte(NVMADDR_TUNE);
  if (nTuneType >= NUM_TYPES)
    nTuneType = TYPE_FREQ;

  nStepIdx = eeprom_read_byte(NVMADDR_STEPIDX);
  if (nStepIdx >= NUM_STEPS)
    nStepIdx = 0;

  nFMFilter = eeprom_read_byte(NVMADDR_FMFILTER);
  if (nFMFilter > NUM_FM_FILTERS)
    nFMFilter = DEF_FM_FILTER;

  nAMFilter = eeprom_read_byte(NVMADDR_AMFILTER);
  if (nAMFilter > NUM_AM_FILTERS)
    nAMFilter = DEF_AM_FILTER;

  u8 = eeprom_read_byte(NVMADDR_MISC1);
  nLowerSig = (u8 >> 7) & 1; // Reduce signal quality for seek/scan/any, 0=normal, 1=lower
  nFMCEQ = (u8 >> 6) & 1;    // FM channel equalizer, 0=off, 1=on
  nFMEMS = (u8 >> 5) & 1;    // FM enhanced multipath suppression, 0=off, 1=on
  nFMSI = (u8 >> 4) & 1;     // FM Stereo Improvement, 0=off, 1=on
  nDIGRA = (u8 >> 3) & 1;    // FM/AM Digital radio, 0=off, 1=on
  // nFMPD = (u8 >> 2) & 1;    // FM Phase diversity, 0=off, 1=on
  nDeemphasis = (u8)&3; // FM de-emphasis, 0=off, 1=50us, 2=75us
  if (nDeemphasis > 2)
    nDeemphasis = DEEMPH50;

  nSquelch[0] = constrain(eeprom_read_byte(NVMADDR_SQU1), -20, 99);
  nSquelch[1] = constrain(eeprom_read_byte(NVMADDR_SQU2), -20, 99);
  nScanStayTime = eeprom_read_byte(NVMADDR_SCSTAY);
  nAnyHoldTime = eeprom_read_byte(NVMADDR_ANYHOLD);
  nBacklightAdj = eeprom_read_byte(NVMADDR_BKADJ);
  nBacklightKeep = eeprom_read_byte(NVMADDR_BKKEEP);

  for (i = 0; i < NUM_BANDS; i++) {
    nBandCh[i] = eeprom_read_byte(NVMADDR_BANDCH + i);
    if (nBandCh[i] < 0 || nBandCh[i] >= nBandChs[i])
      nBandCh[i] = 0;

    u16 = eeprom_read_word(NVMADDR_BANDFREQ + i);
    if (u16 == 0xffff) // Blank ch
      nBandFreq[i] = nBandFMin[i];
    else if (i <= BAND_SW)
      nBandFreq[i] = nBandFMin[i] + u16;
    else
      nBandFreq[i] = nBandFMin[i] + (int32_t)u16 * 5;
    nBandFreq[i] = constrain(nBandFreq[i], nBandFMin[i], nBandFMax[i]);
  }
}

void NVMUnpkWrData(const uint8_t *pAddr) {
  const uint8_t *pF = pAddr;
  uint16_t pNVM = 0;
  uint8_t len, i, val;

  for (;;) {
    len = *(pF++);
    if (!len)
      break;
    if (len & 0x80) { // Dup
      val = *(pF++);
      for (i = 0; i < (len & 0x7f); i++)
        eeprom_write_byte(pNVM++, val);
    } else { // len bytes data
      for (i = 0; i < len; i++)
        eeprom_write_byte(pNVM++, *(pF++));
    }
  }
}

int constrain(int val, int min, int max) {
  if (val >= max)
    return max;
  else if (val <= min)
    return min;
  else
    return val;
}