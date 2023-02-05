#include "tuner.h"
#include "Tuner_Drv_Lithio.h"
#include "Tuner_Interface.h"
#include "nv_memory.h"
#include "tuner main.h"
#include "ui.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

bool bMuted = false; // For mute/unmute

const uint8_t nBandMode[NUM_BANDS] =
    {RFMODE_AM, RFMODE_AM, RFMODE_AM, RFMODE_FM, RFMODE_FM}; // Band RF mode

const uint16_t nBandChNVMA[NUM_BANDS] =
    {NVMADDR_LW, NVMADDR_MW, NVMADDR_SW, NVMADDR_FL, NVMADDR_FM};

uint32_t timer = 0; // RSSI, SNR & FM stereo indicator display timer

uint32_t nAutoSyncBits;
uint16_t nAutoSyncChs;
uint16_t nAutoSyncFreqs;
uint32_t nAutoSyncTimer; // Auto sync to NV memory timer

bool bBandChangeOnly = false;

bool bDelayedCheck = false;
uint32_t nDelayedTimer;

void SetVolume(uint8_t v30) {
  static uint8_t vLast = 0;

  if (v30 == vLast)
    return;
  devTEF_Audio_Set_Volume(M_Volume[v30]);

  if (v30 == 0)
    devTEF_Audio_Set_Mute(1);
  else
    devTEF_Audio_Set_Mute(0);

  vLast = v30;
}

void CheckVolume(void) {
  int8_t i8;

  if ((i8 = GetLRot()) != false) {
    if ((i8 < 0) && (-i8 > nVolume))
      nVolume = 0;
    else
      nVolume = constrain(nVolume + i8, MIN_VOL, MAX_VOL);
    SetVolume(nVolume);
    if (nVolume)
      bMuted = false;
    AddSyncBits(NEEDSYNC_VOL);
    CheckUpdateAlt(ALT_VOL); // Show volume for a period
  }
}

void dsp_set_filter(int8_t f) {
  if (nRFMode == RFMODE_FM) {
    if (nFMFilter == 0)
      devTEF_FM_Set_Bandwidth(1, 2360, 1000, 1000);
    else
      devTEF_FM_Set_Bandwidth(0, 10 * M_FMFilter[f], 1000, 1000);
  } else
    devTEF_AM_Set_Bandwidth(10 * M_AMFilter[f]);
}

void NextFilter(void) {
  if (nRFMode == RFMODE_FM)
    nFMFilter = (nFMFilter + 1) % (NUM_FM_FILTERS);
  else // RFMODE_AM
    nAMFilter = (nAMFilter + 1) % (NUM_AM_FILTERS);
}

void SetFilter(bool bNeedSync) {
  if (nRFMode == RFMODE_FM) {
    dsp_set_filter(nFMFilter);
    if (bNeedSync)
      AddSyncBits(NEEDSYNC_FMFILTER);
  } else {
    dsp_set_filter(nAMFilter);
    if (bNeedSync)
      AddSyncBits(NEEDSYNC_AMFILTER);
  }
}

void SetRFCtrlReg(void) {
  SetFilter(false);
  if (nRFMode == RFMODE_FM) {
    devTEF_FM_Set_MphSuppression(nFMEMS);
    devTEF_FM_Set_ChannelEqualizer(nFMCEQ);
    if (nDeemphasis == 0)
      devTEF_FM_Set_Deemphasis(0);
    else if (nDeemphasis == 1)
      devTEF_FM_Set_Deemphasis(500);
    else if (nDeemphasis == 2)
      devTEF_FM_Set_Deemphasis(750);
  }
}

bool TuneFreq(int32_t freq) {
  if (nRFMode == RFMODE_FM)
    devTEF_FM_Tune_To(freq / 10);
  else
    devTEF_AM_Tune_To(freq);

  bDelayedCheck = true;
  nDelayedTimer = HAL_GetTick() + 30;

  if (!bBandChangeOnly) {
    nAutoSyncFreqs |= (1 << nBand);
    nAutoSyncTimer = HAL_GetTick();
  }
  return true;
}

void AdjFreq(bool bCurrStep) {
  int32_t nFreq;
  int16_t nStep;

  nFreq = nBandFreq[nBand];
  if (bCurrStep)
    nStep = nBandStep[nBand][nStepIdx];
  else
    nStep = nBandStep[nBand][0];

  if (nFreq < nBandFMin[nBand])
    nFreq = nBandFMax[nBand];
  else if (nFreq > nBandFMax[nBand])
    nFreq = nBandFMin[nBand];

  // Adjust frequency to integer multiples of step value
  nFreq = ((nFreq + (nStep >> 1)) / nStep) * nStep;

  if (nFreq < nBandFMin[nBand]) // Check min margin
    nFreq += nStep;
  else if (nFreq > nBandFMax[nBand]) // Check max margin
    nFreq -= nStep;

  nBandFreq[nBand] = nFreq;
}

void TuneFreqDisp(void) {
  int32_t f;

  f = nBandFreq[nBand];
  TuneFreq(f);

  if (nBand >= BAND_FL) {
    LCDXYIntLen(FREQ_X, FREQ_Y, f / 1000, 3);
    LCDXYChar(FREQ_X + 3, FREQ_Y, '.');
    LCDXYUIntLenZP(FREQ_X + 4, FREQ_Y, f % 1000, 3);
  } else {
    LCDXYIntLen(FREQ_X, FREQ_Y, f, 6);
    LCDXYChar(FREQ_X + 6, FREQ_Y, 'K');
  }
}

void SetDigiRadio(void) {
  devTEF_FM_Set_DigitalRadio(nDIGRA);
  devTEF_AM_Set_DigitalRadio(nDIGRA);
  if (nDIGRA) {
    devTEF_FM_Set_DR_Options(6500, 34 << 8 | 2, 16);
    devTEF_AM_Set_DR_Options(6500, 34 << 8 | 2, 16);
  }
}

uint32_t ReadChFreq(void) {
  uint16_t nNVMData;
  int32_t nFreq;

  nNVMData = eeprom_read_word((nBandChNVMA[nBand] + nBandCh[nBand] * 2));
  if (nNVMData == 0xffff) // Blank ch
    return 0;
  if (nBand <= BAND_SW)
    nFreq = nBandFMin[nBand] + nNVMData;
  else
    nFreq = nBandFMin[nBand] + (int32_t)nNVMData * 5;
  nFreq = constrain(nFreq, nBandFMin[nBand], nBandFMax[nBand]);

  return nFreq;
}

void WriteChFreq(bool bAdd) {
  uint16_t u16;

  if (bAdd) { // Add ch
    if (nBand <= BAND_SW)
      u16 = (uint16_t)(nBandFreq[nBand] - nBandFMin[nBand]);
    else
      u16 = (uint16_t)((nBandFreq[nBand] - nBandFMin[nBand]) / 5);
  } else // Delete ch
    u16 = 0xffff;

  eeprom_write_word((nBandChNVMA[nBand] + nBandCh[nBand] * 2), u16);
}

void SeekCh(int8_t nDir) {
  uint8_t u8;
  uint32_t u32;
  int8_t nDir1;

  nDir1 = nDir;
  if (!nDir1) { // Try current ch 1st
    if ((u32 = ReadChFreq()) != false) {
      nBandFreq[nBand] = u32;
      TuneFreqDisp();
      LCDUpdate();
      return;
    }
    nDir1 = 1;
  }

  for ((u8 = 0); u8 < nBandChs[nBand]; u8++) {
    nBandCh[nBand] = (nBandCh[nBand] + nDir1 + nBandChs[nBand]) % nBandChs[nBand];
    if ((u32 = ReadChFreq()) != false) {
      nBandFreq[nBand] = u32;
      TuneFreqDisp();
      LCDUpdate();
      nAutoSyncChs |= (1 << nBand);
      nAutoSyncTimer = HAL_GetTick();
      return;
    }
  }

  // No ch find, set ch=0, f=fmin
  nBandCh[nBand] = 0;
  nBandFreq[nBand] = nBandFMin[nBand];
  TuneFreqDisp();
  LCDUpdate();
}

void ProcBand(uint8_t nBd) {
  if (nStepIdx) {
    nStepIdx = 0; // Default band step
    AddSyncBits(NEEDSYNC_STEPIDX);
  }

  nBand = nBd;
  SetRFMode();

  if (nTuneType == TYPE_CH)
    SeekCh(0);
  else {
    bBandChangeOnly = true; // Change band only, don't sync band frequency to NV memory
    TuneFreqDisp();
    bBandChangeOnly = false;
  }
  SetRFCtrlReg(); // Set RF mode related regs

  AddSyncBits(NEEDSYNC_BAND);
}

void ProcStepFilter(uint8_t nKey) {
  switch (nKey) {
  case KEY_STEP:
    nStepIdx = (nStepIdx + 1) % NUM_STEPS;
    AddSyncBits(NEEDSYNC_STEPIDX);
    break;

  case KEY_STEP | KEY_LONGPRESS: // Default step
    if (nStepIdx != 0)
      nStepIdx = 0;
    else
      nStepIdx = NUM_STEPS - 1;
    AddSyncBits(NEEDSYNC_STEPIDX);
    break;

  case KEY_FILTER:
    NextFilter();
    SetFilter(true);
    break;

  case KEY_FILTER | KEY_LONGPRESS: // Default filter
    if (nRFMode == RFMODE_FM) {
      nFMFilter = DEF_FM_FILTER;
    } else {
      nAMFilter = DEF_AM_FILTER;
    }
    SetFilter(true);
    break;
  }
}

void GetRFStatReg(void) {
  uint16_t signalsts;
  int16_t level;

  if (nRFMode == RFMODE_FM) {
    devTEF_FM_Get_Signal_Status(&signalsts);
    bSTIN = (signalsts >> 15) & 1;

    devTEF_FM_Get_Quality_Status(&level, nullptr, nullptr, nullptr, nullptr, nullptr);
  } else {
    devTEF_AM_Get_Quality_Status(&level, nullptr, nullptr, nullptr, nullptr, nullptr);
  }
  nRSSI = level;
}

bool IsSigOK(void) {
  int8_t nLow;

  nLow = nLowerSig * 10;

  if (nRFMode == RFMODE_FM) {
    if (bSTIN)
      return true;
    if (nRSSI >= (nSquelch[1] + 10 - nLow))
      return true;
    HAL_Delay(5);
    GetRFStatReg();
    if (nRSSI >= (nSquelch[1] - nLow))
      return true;
  } else { // AM
    if (nRSSI >= (nSquelch[1] + 15 - nLow))
      return true;
    HAL_Delay(5);
    GetRFStatReg();
    if (nRSSI >= (nSquelch[1] + 5 - nLow))
      return true;
  }

  return false;
}

int8_t Seek(int8_t nDir) {
  int8_t i;
  uint8_t nKey;

  SetVolume(0); // Mute

  if (nRFMode == RFMODE_FM) {
    if (!nFMFilter || nFMFilter > SEEK_FM_FILTER)
      dsp_set_filter(SEEK_FM_FILTER);
  } else {
    if (!nAMFilter || nAMFilter > SEEK_AM_FILTER)
      dsp_set_filter(SEEK_AM_FILTER);
  }

  for (;;) {
    nBandFreq[nBand] += nDir * nBandStep[nBand][nStepIdx];
    AdjFreq(true);
    TuneFreqDisp();

    for (i = 0; i < 6; i++) {
      if ((nKey = GetKey()) != false) {
        if (nKey & (KEY_STEP | KEY_FILTER)) {
          ProcStepFilter(nKey);
          LCDUpdate();
        } else {
          SetFilter(false);
          SetVolume(nVolume); // Unmute
          return nKey;        // Other key pressed, abort seek
        }
      }

      HAL_Delay(5);
      GetRFStatReg();
      CheckUpdateSig();
      if (IsSigOK()) { // Find one signal
        SetFilter(false);
        SetVolume(nVolume); // Unmute
        return 0;
      }
    }

    CheckVolume();
    CheckUpdateAlt(ALT_AUTO);
  }
}

uint8_t ScanAny() {
  uint32_t t1, nTimeout;
  uint8_t nKey, lp;

  if (nTuneType == TYPE_SCAN)
    nTimeout = nScanStayTime * 1000;
  else
    nTimeout = nAnyHoldTime * 1000;
  LCDUpdate();
  eeprom_write_byte(NVMADDR_TUNE, nTuneType);

  for (;;) {
    LCDOn();

    if ((nKey = Seek(+1)) != false) // Key(Other than STEP & FILTER) pressed, user abort
      return nKey;

    // Else we find one signal
    t1 = HAL_GetTick();
    for (lp = 0;; lp++) {
      if (!(lp % 16))
        CheckUpdateSig();
      if (nTuneType == TYPE_ANY && IsSigOK())
        t1 = HAL_GetTick();

      if ((HAL_GetTick() - t1) > nTimeout) // Lost signal for nTimeout ms in ANY mode
        break;                             // or nTimeout ms passed in SCAN mode

      if ((nKey = GetKey()) != false) {
        if (nKey & (KEY_STEP | KEY_FILTER)) {
          ProcStepFilter(nKey);
          LCDUpdate();
        } else
          return nKey; // Key(Other than STEP/FILTER) pressed, user abort
      }

      if (GetRRot()) // Right rotary encoder turned, leave this signal
        break;

      if ((nTuneType == TYPE_ANY) && nBacklightKeep && !bLCDOff) {
        if ((HAL_GetTick() - nBacklightTimer) >= (uint32_t)nBacklightKeep * 1000) {
          LCDSetBackLight(0); // Set LCD backlight off
          bLCDOff = true;
        }
      }

      CheckVolume(); // Check volume adjustment
      CheckUpdateAlt(ALT_AUTO);
      HAL_Delay(64);
    }
  }
}

void SetRFMode(void) {
  if (nBandMode[nBand] == RFMODE_FM)
    nRFMode = RFMODE_FM;
  else
    nRFMode = RFMODE_AM;
  AddSyncBits(NEEDSYNC_RFMODE);
}

void AddSyncBits(uint32_t SyncBit) {
  nAutoSyncBits |= SyncBit;
  nAutoSyncTimer = HAL_GetTick();
}

void mainTuner(void) {
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(30);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start(&htim4);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  LCDInit();
  LCDXYStr(3, 0, "WTCRC6686");
  LCDXYStr(3, 1, "V2 Build 1");
  HAL_Delay(1000);
  if (HAL_GPIO_ReadPin(KS0_GPIO_Port, KS0_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(KS1_GPIO_Port, KS1_Pin) == GPIO_PIN_RESET)
    TestRotKey();
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
  HAL_Delay(100);

  uint8_t sts;

  if (!eeprom_check()) {
    LCDXYStr(0, 0, "EEPROM Error");
    LCDClr1();
    while (1) {
    };
  }
  if (!devTEF_APPL_Get_Operation_Status(&sts)) {
    LCDXYStr(0, 0, "TEF Chip Absent");
    LCDClr1();
    while (1) {
    };
  }
  Tuner_Patch(205);
  Tuner_Init12000();
  Tuner_Init();
  NVMGetArgs();
  LCDClr();

  nLRot = 0;
  nRRot = 0;

  if (nTuneType == TYPE_CH)
    SeekCh(0); // Tune to current ch
  else
    TuneFreqDisp(); // Tune to default frequency and display
  SetRFCtrlReg();   // Set RF mode related regs
  SetDigiRadio();
  SetVolume(nVolume); // Set audio volume
  HAL_Delay(50);

  LCDSetBackLight(nBacklightAdj); // Set LCD backlight PWM
  nBacklightTimer = HAL_GetTick();
  LCDUpdate();

  nAutoSyncBits = 0;
  nAutoSyncChs = 0;
  nAutoSyncFreqs = 0;
  nAutoSyncTimer = 1000L * 3600 * 24; // Auto sync to NV memory timer
  while (1) {
    TunerLoop();
  }
}

void TunerLoop(void) {
  int8_t i8;
  uint8_t nKey;
  uint16_t u16;

  if (bDelayedCheck && HAL_GetTick() >= nDelayedTimer) {
    bDelayedCheck = false;
    CheckUpdateSig(); // Update RSSI, SNR & FM stereo indicator
    timer = HAL_GetTick() - (TIMER_INTERVAL >> 2);
  } else if ((HAL_GetTick() - timer) >= TIMER_INTERVAL) { // Check every TIMER_INTERVAL ms
    CheckUpdateSig();                                     // Update RSSI, SNR & FM stereo indicator
    timer = HAL_GetTick();

    if (!bMuted) {
      if ((nRSSI < nSquelch[0]))
        SetVolume(0);
      else
        SetVolume(nVolume);
    }

    CheckUpdateAlt(ALT_AUTO);

    if (nBacklightKeep && !bLCDOff) {
      if ((timer - nBacklightTimer) >= (uint32_t)nBacklightKeep * 1000) {
        LCDSetBackLight(0); // Set LCD backlight off
        bLCDOff = true;
      }
    }

    if ((timer - nAutoSyncTimer) >= TIMER_AUTOSYNC) {
      if (nAutoSyncBits) {
        if (nAutoSyncBits & NEEDSYNC_BAND)
          eeprom_write_byte(NVMADDR_BAND, nBand);

        if (nAutoSyncBits & NEEDSYNC_VOL)
          eeprom_write_byte(NVMADDR_VOL, nVolume);

        if (nAutoSyncBits & NEEDSYNC_RFMODE)
          eeprom_write_byte(NVMADDR_RFMODE, nRFMode);

        if (nAutoSyncBits & NEEDSYNC_TUNE)
          eeprom_write_byte(NVMADDR_TUNE, nTuneType);

        if (nAutoSyncBits & NEEDSYNC_STEPIDX)
          eeprom_write_byte(NVMADDR_STEPIDX, nStepIdx);

        if (nAutoSyncBits & NEEDSYNC_FMFILTER)
          eeprom_write_byte(NVMADDR_FMFILTER, nFMFilter);

        if (nAutoSyncBits & NEEDSYNC_AMFILTER)
          eeprom_write_byte(NVMADDR_AMFILTER, nAMFilter);

        if (nAutoSyncBits & NEEDSYNC_MISC1)
          eeprom_write_byte(NVMADDR_MISC1, (nDeemphasis << 6) | (nDIGRA << 5) | (nLowerSig << 4));

        if (nAutoSyncBits & NEEDSYNC_MISC2)
          eeprom_write_byte(NVMADDR_MISC2, (nFMEMS << 7) | (nFMCEQ << 5));

        nAutoSyncBits = 0;
      }

      if (nAutoSyncChs) {
        for (i8 = 0; i8 < NUM_BANDS; i8++) {
          if (nAutoSyncChs & (1 << i8))
            eeprom_write_byte(NVMADDR_BANDCH + i8, nBandCh[i8]);
        }
        nAutoSyncChs = 0;
      }

      if (nAutoSyncFreqs) {
        for (i8 = 0; i8 < NUM_BANDS; i8++) {
          if (nAutoSyncFreqs & (1 << i8)) {
            if (i8 <= BAND_SW)
              u16 = (uint16_t)(nBandFreq[i8] - nBandFMin[i8]);
            else
              u16 = (uint16_t)((nBandFreq[i8] - nBandFMin[i8]) / 5);
            eeprom_write_word(NVMADDR_BANDFREQ + i8, u16);
          }
        }
        nAutoSyncFreqs = 0;
      }

      nAutoSyncTimer = timer + 1000L * 3600 * 24;
    }
  }

  CheckVolume();

  if ((i8 = GetRRot()) != false) {
    switch (nTuneType) {
    case TYPE_FREQ:
      nBandFreq[nBand] += i8 * nBandStep[nBand][nStepIdx];
      AdjFreq(true);
      TuneFreqDisp();
      break;

    case TYPE_SEEK:
      Seek(constrain(i8, -1, 1));
      break;

    case TYPE_CH:
      if (i8 < 0)
        SeekCh(-1);
      else
        SeekCh(1);
      break;
    }
  }

  if ((nKey = GetKey()) != false) {
    switch (nKey) {
    case KEY_LROT:
      Menu(MID_OPTION);
      break;

    case KEY_LROT | KEY_LONGPRESS: // Toggle mute/unmute
      if (bMuted)
        SetVolume(nVolume); // Unmute
      else
        SetVolume(0); // Mute
      bMuted = !bMuted;
      break;

    case KEY_RROT:
      Menu(MID_FREQUENCY);
      break;

    case KEY_RROT | KEY_LONGPRESS:
      AddDelCh();
      LCDUpdate();
      break;

    case KEY_TUNE:
      nTuneType = (nTuneType + 1) % NUM_TYPES;
      if (nTuneType == TYPE_CH)
        SeekCh(0);
      AddSyncBits(NEEDSYNC_TUNE);
      break;

    case KEY_TUNE | KEY_LONGPRESS:
      if (nTuneType != TYPE_FREQ)
        nTuneType = TYPE_FREQ;
      else {
        nTuneType = TYPE_CH;
        SeekCh(0);
      }
      AddSyncBits(NEEDSYNC_TUNE);
      break;

    case KEY_BAND:
      ProcBand((nBand + 1) % NUM_BANDS);
      break;

    case KEY_BAND | KEY_LONGPRESS:
      if (nBand != BAND_FM)
        ProcBand(BAND_FM);
      else
        ProcBand(BAND_MW);
      break;

    default: // Include KEY_STEP, KEY_STEP | KEY_LONGPRESS, KEY_FILTER, KEY_FILTER | KEY_LONGPRESS
      ProcStepFilter(nKey);
      break;
    }
    LCDUpdate();
  }

  if (nTuneType == TYPE_SCAN) {
    if (ScanAny() == KEY_TUNE)
      nTuneType = TYPE_ANY;
    else
      nTuneType = TYPE_FREQ;
    LCDUpdate();
    AddSyncBits(NEEDSYNC_TUNE);
  }

  if (nTuneType == TYPE_ANY) {
    ScanAny();
    nTuneType = TYPE_FREQ;
    LCDUpdate();
    AddSyncBits(NEEDSYNC_TUNE);
  }
}