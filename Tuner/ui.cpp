#include "ui.h"
#include "Tuner_Drv_Lithio.h"
#include "Tuner_Interface.h"
#include "nv_memory.h"
#include "tuner.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

const char *pszTuneTypes[] = {"FS", "SK", "CH", "SC", "AN", "SS"};
const char *pszBands[] = {"LW", "MW", "SW", "FL", "FM"}; // Band name to display

uint8_t bExitMenu;

// Special menu strings
const char MT_RET[] = "RET";
const char MT_EXIT[] = "EXIT";

const char MT_RADI[] = "RADI";
const char MT_AUDI[] = "AUDI";
const char MT_APPL[] = "APPL";

struct M_ITEM M_Option[] =
    {
        {MID_RADI, MT_RADI},
        {MID_AUDI, MT_AUDI},
        {MID_APPL, MT_APPL},
        {MID_EXIT, MT_EXIT}};

// Menu Option(Left encoder)
const char MT_SQU1[] = "Mute Threshold";
const char MT_SQU2[] = "Search Threshold";
const char MT_LSIG[] = "Low Signal Thre";  // Normal/reduced signal quality for seek/scan/any
const char MT_FMCE[] = "FM ChannelEqu";    // FM channel equalizer
const char MT_FMMP[] = "FM MultpathImprv"; // FM enhanced multipath suppression
const char MT_DIRA[] = "Digital Radio";    // Digital Radio Option
const char MT_DEEM[] = "Deemphasize";      // FM de-emphasis
const char MT_BKLT[] = "BackLight";
const char MT_TSCN[] = "Time Scan";
const char MT_TANY[] = "Time Any";
const char MT_TIME[] = "Time Set";

struct M_ITEM M_Radio[] =
    {
        {MID_SQUELCH1, MT_SQU1},
        {MID_SQUELCH2, MT_SQU2},
        {MID_LSIG, MT_LSIG},
        {MID_FMCE, MT_FMCE},
        {MID_FMMP, MT_FMMP},
        {MID_DIRA, MT_DIRA},
        {MID_DEEM, MT_DEEM}};

struct M_ITEM M_Audio[] =
    {
        {MID_RET, MT_RET}};

struct M_ITEM M_Appli[] =
    {
        {MID_BKLT, MT_BKLT},
        {MID_TSCN, MT_TSCN},
        {MID_TANY, MT_TANY},
        {MID_TIME, MT_TIME},
        {MID_RET, MT_RET}};

// Menu Option->LSIG
const char MT_LSIGNORM[] = "NORM";
const char MT_LSIGLOW[] = "LOW";

struct M_ITEM M_LSIG[] =
    {
        {MID_LSIGNORM, MT_LSIGNORM},
        {MID_LSIGLOW, MT_LSIGLOW}};

// Menu Option->FMCE
const char MT_FMCEOFF[] = "OFF";
const char MT_FMCEON[] = "ON";

struct M_ITEM M_FMCE[] =
    {
        {MID_FMCEOFF, MT_FMCEOFF},
        {MID_FMCEON, MT_FMCEON}};

// Menu Option->FMMP
const char MT_FMMPOFF[] = "OFF";
const char MT_FMMPON[] = "ON";

struct M_ITEM M_FMMP[] =
    {
        {MID_FMMPOFF, MT_FMMPOFF},
        {MID_FMMPON, MT_FMMPON}};

// Menu Option->DIRA
const char MT_DIRAOFF[] = "OFF";
const char MT_DIRAON[] = "ON";

struct M_ITEM M_DIRA[] =
    {
        {MID_DIRAOFF, MT_DIRAOFF},
        {MID_DIRAON, MT_DIRAON}};

// Menu Option->DEEM
const char MT_DEEM0[] = "OFF";
const char MT_DEEM50[] = "50US";
const char MT_DEEM75[] = "75US";

struct M_ITEM M_Deem[] =
    {
        {MID_DEEM0, MT_DEEM0},
        {MID_DEEM50, MT_DEEM50},
        {MID_DEEM75, MT_DEEM75}};

// Menu Option->BKLT
const char MT_BKKEEP[] = "KEEP";
const char MT_BKADJ[] = "ADJ";

struct M_ITEM M_BkLt[] =
    {
        {MID_BKKEEP, MT_BKKEEP},
        {MID_BKADJ, MT_BKADJ},
        {MID_RET, MT_RET}};

// Menu Frequency(Right encoder)
const char MT_STAT[] = "Signal Quality";
const char MT_TUNE[] = "Tune Type";
const char MT_BAND[] = "Band Select";
const char MT_FILT[] = "IF Filter Select";
const char MT_HELP[] = "Usage & Help";

struct M_ITEM M_Frequency[] =
    {
        {MID_STAT, MT_STAT},
        {MID_TUNE, MT_TUNE},
        {MID_BAND, MT_BAND},
        {MID_HELP, MT_HELP},
        {MID_EXIT, MT_EXIT}};

// Menu Frequency->TUNE
const char MT_FREQ[] = "FREQ";
const char MT_SEEK[] = "SEEK";
const char MT_CH[] = "CH";
const char MT_SCAN[] = "SCAN";
const char MT_ANY[] = "ANY";
const char MT_SCSV[] = "S&S";

struct M_ITEM M_Tune[] =
    {
        {MID_FREQ, MT_FREQ},
        {MID_SEEK, MT_SEEK},
        {MID_CH, MT_CH},
        {MID_SCAN, MT_SCAN},
        {MID_ANY, MT_ANY},
        {MID_SCSV, MT_SCSV}};

// Menu Frequency->BAND
const char MT_LW[] = "LW";
const char MT_MW[] = "MW";
const char MT_SW[] = "SW";
const char MT_FL[] = "FM-L";
const char MT_FM[] = "FM";

struct M_ITEM M_Band[] =
    {
        {MID_LW, MT_LW},
        {MID_MW, MT_MW},
        {MID_SW, MT_SW},
        {MID_FL, MT_FL},
        {MID_FM, MT_FM},
};

struct M_SUBMENU SM_List[] =
    {
        {MID_OPTION, M_Option, sizeof(M_Option) / sizeof(struct M_ITEM)},          // Menu Option
        {MID_FREQUENCY, M_Frequency, sizeof(M_Frequency) / sizeof(struct M_ITEM)}, // Menu Frequency
        {MID_BKLT, M_BkLt, sizeof(M_BkLt) / sizeof(struct M_ITEM)},                // Menu Option->BKLT
        {MID_RADI, M_Radio, sizeof(M_Radio) / sizeof(struct M_ITEM)},              // Menu Option->RADI
        {MID_AUDI, M_Audio, sizeof(M_Audio) / sizeof(struct M_ITEM)},              // Menu Option->AUDI
        {MID_APPL, M_Appli, sizeof(M_Appli) / sizeof(struct M_ITEM)},              // Menu Option->APPL
        {MID_LSIG, M_LSIG, sizeof(M_LSIG) / sizeof(struct M_ITEM)},                // Menu Option->LSIG
        {MID_FMCE, M_FMCE, sizeof(M_FMCE) / sizeof(struct M_ITEM)},                // Menu Option->FMCE
        {MID_FMMP, M_FMMP, sizeof(M_FMMP) / sizeof(struct M_ITEM)},                // Menu Option->FMMP
        {MID_DIRA, M_DIRA, sizeof(M_DIRA) / sizeof(struct M_ITEM)},                // Menu Option->DIRA
        {MID_DEEM, M_Deem, sizeof(M_Deem) / sizeof(struct M_ITEM)},                // Menu Option->DEEM
        {MID_TUNE, M_Tune, sizeof(M_Tune) / sizeof(struct M_ITEM)},                // Menu Frequency->TUNE
        {MID_BAND, M_Band, sizeof(M_Band) / sizeof(struct M_ITEM)},                // Menu Frequency->BAND
};

////////////////////////////////////////////////////////////
// LCD utility

void Delay(const uint16_t time) {
  TIM4->CNT = 0;
  while (TIM4->CNT < time * 10) {
  }
}

void LCDSetBackLight(const uint8_t Data) {
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Data);
}

void LCDEN(void) // Generate LCD EN pulse
{
  HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
  Delay(2);
  HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
}

void LCDCmd(const uint8_t Cmd) // Write LCD command
{
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (Cmd & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (Cmd & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (Cmd & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (Cmd & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  LCDEN();
  HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (Cmd & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (Cmd & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (Cmd & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (Cmd & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  LCDEN();
  Delay(50);
}

void LCDData(const uint8_t Data) // Write LCD data
{
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (Data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (Data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (Data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (Data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  LCDEN();
  HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (Data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (Data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (Data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (Data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  LCDEN();
  Delay(50);
}

void LCDClr(void) {
  LCDCmd(0x01); // Clear LCD
  HAL_Delay(5);
}

void LCDClr1(void) {
  LCDXYStr(0, 1, "                "); // Clear LCD's 2nd row
}

void LCDOn(void) {
  if (nBacklightKeep && bLCDOff) {
    LCDSetBackLight(nBacklightAdj); // Set LCD backlight PWM
    bLCDOff = false;
  }
  nBacklightTimer = HAL_GetTick();
}

void LCDInit(void) {
  LCDCmd(0x28); // 4 bits interface
  HAL_Delay(5);
  LCDSetBackLight(255);
  LCDCmd(0x28);
  HAL_Delay(5);

  LCDCmd(0x28);
  HAL_Delay(5);

  LCDCmd(0x08); // LCD off
  Delay(50);
  LCDClr();     // Clear LCD
  LCDCmd(0x06); // Auto increment
  Delay(50);
  LCDCmd(0x0c); // LCD on
  HAL_Delay(15);
}

void LCDXY(const uint8_t x, const uint8_t y) // x:0-15, y:0-1
{
  LCDCmd(0x80 + y * 0x40 + x);
}

void LCDXYChar(const uint8_t x, const uint8_t y, const char c) // Display char at x:0-15, y:0-1
{
  LCDXY(x, y);
  LCDData(c);
}

void LCDXYStr(const uint8_t x, const uint8_t y, const char *str) // Display string at x:0-15, y:0-1
{
  const char *p;
  char c;

  LCDXY(x, y);
  p = str;
  while ((c = *p++) != false)
    LCDData(c);
}

void LCDFullStr(const char *str) // Display string to full(2x16 chars) LCD, fill with blank if string is less than 32 chars
{
  const char *p;
  char c;
  uint8_t x, y;
  uint8_t bInStr;

  p = str;
  bInStr = 1;
  for (y = 0; y <= 1; y++) {
    for (x = 0; x <= 15; x++) {
      if (!x)
        LCDXY(0, y);

      if (bInStr) {
        c = *(p++);
        if (!c) {
          c = ' ';
          bInStr = 0;
        }
      } else
        c = ' ';

      LCDData(c);
    }
  }
}

// Display string at x:0-15, y:0-1
void LCDXYStrLen(const uint8_t x, const uint8_t y, const char *str, const uint8_t nLen, const bool bLeftAlign) {
  char *p, c;
  uint8_t i, n;

  LCDXY(x, y);
  p = (char *)str;
  n = nLen - strlen(str);
  if (!bLeftAlign) {
    for (i = 0; i < n; i++)
      LCDData(' ');
  }

  while ((c = *p++) != false)
    LCDData(c);

  if (bLeftAlign) {
    for (i = 0; i < n; i++)
      LCDData(' ');
  }
} // void LCDXYStrLen(uint8_t x, uint8_t y, char *str, uint8_t nLen, bool bLeftAlign)

// Display signed integer at x:0-15, y:0-1. Righ aligned, spaces filled at left. Too long integers are not allowed.
void LCDXYIntLen(const uint8_t x, const uint8_t y, const int32_t n, const uint8_t nLen) {
  int8_t i;
  int32_t r;

  if (n >= 0)
    r = n;
  else
    r = -n;

  i = nLen - 1;
  do {
    LCDXYChar(x + i--, y, '0' + (r % 10));
    r = r / 10;
  } while (r);

  if (n < 0)
    LCDXYChar(x + i--, y, '-');

  while (i >= 0)
    LCDXYChar(x + i--, y, ' ');
}

// Display unsigned integer at x:0-15, y:0-1. Righ aligned, '0' filled at left. Too long unsigned integers are not allowed.
void LCDXYUIntLenZP(const uint8_t x, const uint8_t y, const uint32_t n, const uint8_t nLen) {
  int8_t i;
  uint32_t r;
  r = n;
  i = nLen - 1;
  do {
    LCDXYChar(x + i--, y, '0' + (r % 10));
    r = r / 10;
  } while (r);

  while (i >= 0)
    LCDXYChar(x + i--, y, '0');
}

////////////////////////////////////////////////////////////
// Rotary encoder & key utility

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == SH1A_Pin) {
    if (HAL_GPIO_ReadPin(SH1A_GPIO_Port, SH1A_Pin) == HAL_GPIO_ReadPin(SH1B_GPIO_Port, SH1B_Pin))
      ++nLRot;
    else
      --nLRot;
  } else if (GPIO_Pin == SH2A_Pin) {
    if (HAL_GPIO_ReadPin(SH2A_GPIO_Port, SH2A_Pin) == HAL_GPIO_ReadPin(SH2B_GPIO_Port, SH2B_Pin))
      ++nRRot;
    else
      --nRRot;
  }
}

int8_t GetLRot(void) {
  int8_t n;

  if (nLRot) {
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    n = nLRot;
    nLRot = 0;
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    LCDOn();
    return n;
  }
  return 0;
}

int8_t GetRRot(void) {
  int8_t n;

  if (nRRot) {
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    n = nRRot;
    nRRot = 0;
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    LCDOn();
    return n;
  }
  return 0;
}

uint8_t PeekKey(void) {
  int8_t i;
  uint8_t nKey0 = 0xff, nKey;

  for (i = 0; i < 30;) {

    if (HAL_GPIO_ReadPin(KS0_GPIO_Port, KS0_Pin) == GPIO_PIN_RESET)
      nKey = KEY_LROT;
    else if (HAL_GPIO_ReadPin(KS1_GPIO_Port, KS1_Pin) == GPIO_PIN_RESET)
      nKey = KEY_RROT;
    else if (HAL_GPIO_ReadPin(KS2_GPIO_Port, KS2_Pin) == GPIO_PIN_RESET)
      nKey = KEY_TUNE;
    else if (HAL_GPIO_ReadPin(KS3_GPIO_Port, KS3_Pin) == GPIO_PIN_RESET)
      nKey = KEY_STEP;
    else if (HAL_GPIO_ReadPin(KS4_GPIO_Port, KS4_Pin) == GPIO_PIN_RESET)
      nKey = KEY_BAND;
    else if (HAL_GPIO_ReadPin(KS5_GPIO_Port, KS5_Pin) == GPIO_PIN_RESET)
      nKey = KEY_FILTER;
    else
      nKey = 0;
    if (nKey == nKey0)
      ++i;
    else {
      i = 0;
      nKey0 = nKey;
    }
  }
  return nKey;
}

uint8_t GetKey(void) {
  uint8_t nKey0, nKey;
  uint32_t timer1; // Long press timer
  static uint8_t nKeyWaitUp = 0;
  static uint32_t tKeyWaitUp;

  if (nKeyWaitUp && (HAL_GetTick() - tKeyWaitUp) < TIMER_LAST_LP) {
    while ((nKey0 = PeekKey()) == nKeyWaitUp) {
    };
  } else
    nKey0 = PeekKey();

  nKeyWaitUp = 0;
  if (!nKey0)
    return 0;

  timer1 = HAL_GetTick();
  while ((HAL_GetTick() - timer1) < TIMER_LONGPRESS) {
    if ((nKey = PeekKey()) == 0) {
      LCDOn();
      return nKey0; // Key up
    }

    if (nKey != nKey0) { // Another key pressed
      nKey0 = nKey;
      timer1 = HAL_GetTick(); // Reset long press timer for new key
      continue;
    }
  }

  tKeyWaitUp = HAL_GetTick();
  nKeyWaitUp = nKey0;
  LCDOn();
  return nKey0 | KEY_LONGPRESS;
}

void TestRotKey(void) // Show Rotary encoder and key data on LCD
{
  uint8_t uLRot = 0;
  int8_t iRRot = 0, i, iTmp;

  LCDClr();
  LCDXYStr(6, 0, "TEST");
  nLRot = 0;
  nRRot = 0;

  for (i = 0;; i++) {
    iTmp = GetLRot();
    if (iTmp && PeekKey() == KEY_RROT) {
      // Right rotary encoder pressed while rotate left rotary encoder
      if (iTmp < 0) { // Left encoder: CCW
        if (uLRot == 67 && iRRot == 97)
          NVMInitSetting();
      }
      return;
    }
    uLRot += iTmp;
    LCDXYIntLen(0, 0, uLRot, 4);

    iRRot += GetRRot();
    LCDXYIntLen(12, 0, iRRot, 4);
  }
}

void LCDUpdate(void) {
  int8_t i8;
  uint16_t bw;

  // Update band: LW/MW/SW/FL/FM, FL=FM Low
  LCDXYStr(BAND_X, BAND_Y, (char *)pszBands[nBand]);

  // Update tune type: FS/SK/CH/SC/AN/SS
  LCDXYStr(TYPE_X, TYPE_Y, (char *)pszTuneTypes[nTuneType]);

  if ((nTuneType == TYPE_CH) || (nTuneType == TYPE_SCSV)) // CH/SS type
    LCDXYUIntLenZP(STEP_X, STEP_Y, nBandCh[nBand], 3);
  else { // None ch/ss type
    // Update step: 1K/5K/9K/10K/25K/45K/50K/90K/100/500
    if (nBandStep[nBand][nStepIdx] < 100)
      i8 = 1;
    else
      i8 = 0;
    LCDXYIntLen(STEP_X, STEP_Y, nBandStep[nBand][nStepIdx], 3 - i8);
    if (i8)
      LCDXYChar(STEP_X + 2, STEP_Y, 'K');
  }

  // Update IF filter bandwidth
  if (nRFMode == RFMODE_FM)
    bw = M_FMFilter[nFMFilter];
  else
    bw = M_AMFilter[nAMFilter];
  if (bw == 0 && nRFMode == RFMODE_FM)
    LCDXYStr(FILTER_X, FILTER_Y, "AUTO");
  else {
    LCDXYIntLen(FILTER_X, FILTER_Y, bw, 3);
    LCDXYChar(FILTER_X + 3, FILTER_Y, 'K');
  }
}

void CheckUpdateSig(void) {
  char c;
  float fv;

  GetRFStatReg();

  // FM stereo indicator. 'S' for FM stereo, ' ' for FM mono or AM mode,  'M' for FM forced mono
  c = ' '; // AM mode or FM mono
  if (nRFMode == RFMODE_FM) {
    if (bSTIN)
      c = 'S'; // Stereo
  }

  LCDXYIntLen(RSSI_X, RSSI_Y, constrain(nRSSI, -9, 99), 2); // Update RF signal level in dBuv
  LCDXYChar(STEREO_X, STEREO_Y, c);                         // Update FM stereo indicator
                                                            // LCDXYIntLen(SN_X, SN_Y, **, 3);						   // Update FM  reg
}

void ShowMisc(void) {
  char s[] = "3e- A";
  const char ds[] = "-57";
  const char cmode[] = "FAXX";

  if (nRFMode == RFMODE_FM) {
    if (nFMCEQ)
      s[1] = 'E';           // FM channel equalizer on
    s[2] = ds[nDeemphasis]; // Deemphasis, '-' for none, '5' for 50us, '7' for 75us
  }

  // Update mode: F/A/X, F=FM, A=AM, X=AUX
  s[4] = cmode[nRFMode];

  LCDXYStr(ALT_X, ALT_Y, s);
}

void ShowTime(void) {
  uint32_t nMinutes;

  nMinutes = (HAL_GetTick() / 1000 + nSecondsOffset) / 60;
  LCDXYUIntLenZP(ALT_X, ALT_Y, (nMinutes / 60) % 24, 2); // Hour
  LCDXYChar(ALT_X + 2, ALT_Y, ':');
  LCDXYUIntLenZP(ALT_X + 3, ALT_Y, nMinutes % 60, 2); // Minute
}

void ShowVol(void) {
  LCDXYStr(ALT_X, ALT_Y, "VOL");
  LCDXYIntLen(ALT_X + 3, ALT_Y, nVolume, 2); // Volume
}

void CheckUpdateAlt(int8_t nShow) // Check and update ALT area
{
  static int8_t nShowLast = ALT_MISC;
  static uint32_t nTimerAlt = 0;

  if (nShow != ALT_AUTO) {
    nTimerAlt = HAL_GetTick();
    nShowLast = nShow;
  }

  if ((HAL_GetTick() - nTimerAlt) < TIMER_ALTDISP) { // Not timeout
    switch (nShowLast) {
    case ALT_MISC:
      ShowMisc();
      break;

    case ALT_TIME:
      ShowTime();
      break;

    case ALT_VOL:
      ShowVol();
      break;
    }
  } else { // Timeout, swap beteen ALT_MISC/ALT_TIME
    if (nShowLast != ALT_MISC) {
      nShowLast = ALT_MISC;
      ShowMisc();
    } else {
      nShowLast = ALT_TIME;
      ShowTime();
    }
    nTimerAlt = HAL_GetTick();
  }
}

void Menu_Squelch(uint8_t nIdx) {
  int16_t i16 = nSquelch[nIdx];
  uint8_t nKey, lp;

  // 0123456789012345
  LCDXYStr(0, 1, "SQUELCH1:       ");
  if (nIdx)
    LCDXYChar(7, 1, '2');

  for (lp = 0;; lp++) {
    i16 += GetLRot() + GetRRot();
    i16 = constrain(i16, -99, 99);
    LCDXYIntLen(10, 1, i16, 3);

    if ((nKey = GetKey()) != false) {
      nSquelch[nIdx] = (int8_t)i16;
      LCDClr1();
      if (!(nKey & (KEY_LROT | KEY_RROT)))
        bExitMenu = true;
      eeprom_write_byte(NVMADDR_SQU1 + nIdx, nSquelch[nIdx]);
      return;
    }

    if (!(lp % 16))
      CheckUpdateSig();
    HAL_Delay(64);
  }
}

void Menu_BacklightAdj(void) {
  int16_t i16 = nBacklightAdj;
  uint8_t nKey, lp;

  // 0123456789012345
  LCDXYStr(0, 1, ("BACKLT ADJ:     "));

  for (lp = 0;; lp++) {
    i16 = (i16 + GetLRot() + GetRRot() + 256) % 256;
    LCDXYIntLen(12, 1, i16, 3);
    LCDSetBackLight(i16);
    if ((nKey = GetKey()) != false) {
      nBacklightAdj = (uint8_t)i16;
      LCDClr1();
      if (!(nKey & (KEY_LROT | KEY_RROT)))
        bExitMenu = true;
      eeprom_write_byte(NVMADDR_BKADJ, nBacklightAdj);
      return;
    }

    if (!(lp % 16))
      CheckUpdateSig();
    HAL_Delay(64);
  }
}

void Menu_BacklightKeep(void) {
  int16_t i16 = nBacklightKeep;
  uint8_t nKey, lp;

  // 0123456789012345
  LCDXYStr(0, 1, ("BACKLT KEEP:    "));

  for (lp = 0;; lp++) {
    i16 = (i16 + GetLRot() + GetRRot() + 256) % 256;
    LCDXYIntLen(13, 1, i16, 3);

    if ((nKey = GetKey()) != false) {
      nBacklightKeep = (uint8_t)i16;
      LCDClr1();
      if (!(nKey & (KEY_LROT | KEY_RROT)))
        bExitMenu = true;
      eeprom_write_byte(NVMADDR_BKKEEP, nBacklightKeep);
      return;
    }

    if (!(lp % 16))
      CheckUpdateSig();
    HAL_Delay(64);
  }
}

void Menu_ScanStayTime(void) {
  int16_t i16 = nScanStayTime;
  uint8_t nKey, lp;

  // 0123456789012345
  LCDXYStr(0, 1, ("TIME SCAN:     S"));

  for (lp = 0;; lp++) {
    i16 += GetLRot() + GetRRot();
    i16 = constrain(i16, 0, 255);
    LCDXYIntLen(12, 1, i16, 3);

    if ((nKey = GetKey()) != false) {
      nScanStayTime = (uint8_t)i16;
      LCDClr1();
      if (!(nKey & (KEY_LROT | KEY_RROT)))
        bExitMenu = true;
      eeprom_write_byte(NVMADDR_SCSTAY, nScanStayTime);
      return;
    }

    if (!(lp % 16))
      CheckUpdateSig();
    HAL_Delay(64);
  }
}

void Menu_AnyHoldTime(void) {
  int16_t i16 = nAnyHoldTime;
  uint8_t nKey, lp;

  // 0123456789012345
  LCDXYStr(0, 1, ("TIME ANY:     S"));

  for (lp = 0;; lp++) {
    i16 += GetLRot() + GetRRot();
    i16 = constrain(i16, 0, 255);
    LCDXYIntLen(11, 1, i16, 3);

    if ((nKey = GetKey()) != false) {
      nAnyHoldTime = (uint8_t)i16;
      LCDClr1();
      if (!(nKey & (KEY_LROT | KEY_RROT)))
        bExitMenu = true;
      eeprom_write_byte(NVMADDR_ANYHOLD, nAnyHoldTime);
      return;
    }

    if (!(lp % 16))
      CheckUpdateSig();
    HAL_Delay(64);
  }
}

void Menu_Time(void) {
  int32_t nSeconds;
  uint8_t u8, lp;

  // 0123456789012345
  LCDXYStr(0, 1, ("TIME    :  :    "));

  for (lp = 0;; lp++) {
    nSecondsOffset += (int16_t)GetLRot() * 3600 + (int16_t)GetRRot() * 60;
    nSeconds = HAL_GetTick() / 1000 + nSecondsOffset;
    if (nSeconds < 0) {
      nSeconds = 0;
      nSecondsOffset = 3600L * 24 * 15 - HAL_GetTick() / 1000;
    }

    LCDXYUIntLenZP(6, 1, (nSeconds / 3600) % 24, 2); // Hours
    LCDXYUIntLenZP(9, 1, (nSeconds / 60) % 60, 2);   // Minutes
    LCDXYUIntLenZP(12, 1, nSeconds % 60, 2);         // Seconds

    if ((u8 = GetKey()) != false) {
      nSeconds = HAL_GetTick() / 1000 + nSecondsOffset;
      if (u8 == (KEY_FILTER | KEY_LONGPRESS)) { // Adjust to on the minute, i.e. hh:mm:00
        u8 = nSeconds % 60;
        if (u8 < 30)
          nSeconds -= u8;
        else
          nSeconds += 60 - u8;
      }
      nSecondsOffset = nSeconds - HAL_GetTick() / 1000;
      LCDClr1();
      if (!(u8 & (KEY_LROT | KEY_RROT)))
        bExitMenu = true;
      return;
    }

    if (!(lp % 16))
      CheckUpdateSig();
    HAL_Delay(64);
  }
}

void sprhex2(char *str, uint8_t v) {
  uint8_t n;

  n = v >> 4;
  *str = ((n < 10) ? '0' : ('A' - 10)) + n;
  n = v & 0x0f;
  *(str + 1) = ((n < 10) ? '0' : ('A' - 10)) + n;
}

void Menu_Stat_FM(void) {
  uint8_t nKey, u8, lp = 0;
  int8_t i8, nItem = 0, nItemOld = -1, nItems;
  // 0123456789012345
  const char *str[] =
      {
          ("FM US NOISE:"), // 1st item
          ("FM MULTIPATH:"),
          ("RF OFFSET:     K"),
          ("IF FILTER:"),
          ("MODULATION:    %"),
      };

  nItems = sizeof(str) / sizeof(const char *);
  for (;;) {
    if ((nKey = GetKey()) != false) {
      if (!(nKey & (KEY_LROT | KEY_RROT)))
        bExitMenu = true;
      LCDClr1();
      return;
    }

    if ((i8 = GetLRot() + GetRRot()) != false)
      nItem = (nItems + nItem + i8) % nItems;

    if (nItem != nItemOld) {
      if (nItem == 0 && nRFMode == RFMODE_AM)
        u8 = nItems;
      else
        u8 = nItem;
      LCDClr1();
      LCDXYStr(0, 1, str[u8]);
      nItemOld = nItem;
      lp = 0;
    } else if (++lp % 16) {
      HAL_Delay(64);
      continue;
    }

    CheckUpdateSig();

    uint16_t usn, wam, bandwidth, mod;
    int16_t offset;

    devTEF_FM_Get_Quality_Status(nullptr, &usn, &wam, &offset, &bandwidth, &mod);

    switch (nItem) {
    case 0:
      LCDXYIntLen(13, 1, usn, 3); // FM ultrasonic noise detector & AM high frequency noise detector
      break;

    case 1:
      LCDXYIntLen(13, 1, wam, 3); // FM multipath
      break;

    case 2:
      LCDXYIntLen(11, 1, offset, 4); // Frequency offset
      break;

    case 3:
      LCDXYIntLen(12, 1, bandwidth, 3); // IF filter bandwidth
      LCDXYChar(15, 1, 'K');
      break;

    case 4:
      LCDXYIntLen(12, 1, mod, 3); // Modulation index
      break;
    }
  }
}

void Menu_Stat_AM(void) {
  uint8_t nKey, u8, lp = 0;
  int8_t i8, nItem = 0, nItemOld = -1, nItems;
  // 0123456789012345
  const char *str[] =
      {
          ("AM NOISE:"), // 1st item
          ("Co-Channel:"),
          ("RF OFFSET:     K"),
          ("IF FILTER:"),
          ("MODULATION:    %"),
      };

  nItems = sizeof(str) / sizeof(const char *);
  for (;;) {
    if ((nKey = GetKey()) != false) {
      if (!(nKey & (KEY_LROT | KEY_RROT)))
        bExitMenu = true;
      LCDClr1();
      return;
    }

    if ((i8 = GetLRot() + GetRRot()) != false)
      nItem = (nItems + nItem + i8) % nItems;

    if (nItem != nItemOld) {
      if (nItem == 0 && nRFMode == RFMODE_AM)
        u8 = nItems;
      else
        u8 = nItem;
      LCDClr1();
      LCDXYStr(0, 1, str[u8]);
      nItemOld = nItem;
      lp = 0;
    } else if (++lp % 16) {
      HAL_Delay(64);
      continue;
    }

    CheckUpdateSig();
    uint16_t noise, cochannel, bandwidth, mod;
    int16_t offset;

    devTEF_AM_Get_Quality_Status(nullptr, &noise, &cochannel, &offset, &bandwidth, &mod);
    switch (nItem) {
    case 0:
      LCDXYIntLen(13, 1, noise, 3); // AM high frequency noise detector
      break;

    case 1:
      LCDXYIntLen(13, 1, cochannel, 3); // FM multipath & AM co-channel detector
      break;

    case 2:
      LCDXYIntLen(11, 1, offset, 4); // Frequency offset
      break;

    case 3:
      LCDXYIntLen(12, 1, bandwidth, 3); // IF filter bandwidth
      LCDXYChar(15, 1, 'K');
      break;

    case 4:
      LCDXYIntLen(12, 1, mod, 3); // Modulation index
      break;
    }
  }
}

bool YesNo(bool bChkSig) {
  uint8_t u8, lp;

  LCDXYStr(13, 1, ("Y/N"));
  GetKey(); // Clear key

  for (lp = 0;; lp++) {
    if ((u8 = GetKey()) != false) {
      if (u8 == KEY_LROT)
        return true;
      else if (u8 == KEY_RROT)
        return false;
    }

    if (bChkSig && !(lp % 16))
      CheckUpdateSig();
    HAL_Delay(64);
  }
}

void Menu_SCSV(void) {
  uint16_t i;
  uint8_t j;
  bool bReturn = false;

  // 0123456789012345
  LCDXYStr(0, 1, ("Overwrite?   ")); // Confirm overwrite currrent band ch data
  if (!YesNo(true))
    return;

  nTuneType = TYPE_SCSV;
  SetVolume(0); // Mute
  if (nRFMode == RFMODE_FM) {
    if (!nFMFilter || nFMFilter > SEEK_FM_FILTER)
      dsp_set_filter(SEEK_FM_FILTER);
  } else { // AM
    if (!nAMFilter || nAMFilter > SEEK_AM_FILTER)
      dsp_set_filter(SEEK_AM_FILTER);
  }

  nBandCh[nBand] = 0;
  LCDClr1();
  LCDUpdate();
  for (i = 0; i <= ((nBandFMax[nBand] - nBandFMin[nBand]) / nBandStep[nBand][0]); i++) {
    nBandFreq[nBand] = nBandFMin[nBand] + (int32_t)nBandStep[nBand][0] * i;
    AdjFreq(false);
    TuneFreqDisp();
    HAL_Delay(5);

    for (j = 0; j < 10; j++) {
      HAL_Delay(5);
      GetRFStatReg();
      if (IsSigOK()) { // Find one signal
        WriteChFreq(true);
        LCDUpdate();
        if (++nBandCh[nBand] >= nBandChs[nBand]) // NV memory full for current band
          bReturn = true;
        break;
      }
    }

    CheckUpdateSig();
    CheckUpdateAlt(ALT_AUTO);

    if (bReturn || GetKey())
      break;
  }

  for (; nBandCh[nBand] < nBandChs[nBand]; ++nBandCh[nBand])
    WriteChFreq(false); // Delete extra band chs

  SetFilter(false);
  nTuneType = TYPE_CH;
  nBandCh[nBand] = 0;
  SeekCh(0);
  AddSyncBits(NEEDSYNC_TUNE);
  SetVolume(nVolume); // Unmute
}

void AddDelCh(void) {
  uint8_t u8, lp;
  int8_t i8;
  bool bReDISP = true;
  bool bReturn;
  uint32_t u32 = 0;
  char s[4] = "A/D";

  // 0123456789012345
  LCDXYStr(0, 1, ("CH           "));
  for (bReturn = false, lp = 0; !bReturn; lp++) {
    if ((i8 = GetLRot()) != false) {
      nBandCh[nBand] = (nBandCh[nBand] + i8 + nBandChs[nBand]) % nBandChs[nBand];
      bReDISP = true;
    }

    if ((i8 = GetRRot()) != false) {
      nBandFreq[nBand] += i8 * nBandStep[nBand][nStepIdx];
      AdjFreq(true);
      TuneFreqDisp();
    }

    if (bReDISP) {
      u32 = ReadChFreq();
      LCDXYUIntLenZP(2, 1, nBandCh[nBand], 3);
      LCDXYIntLen(6, 1, u32, 6);
      if (u32)      // Has frequency
        s[2] = 'D'; // Not empty ch, can be deleted
      else
        s[2] = 'd'; // Empty ch
      LCDXYStr(13, 1, s);
      bReDISP = false;
    }

    if ((u8 = GetKey()) != false) {
      switch (u8) {
      case KEY_LROT:         // Add ch
        if (YesNo(true)) {   // Confirmed
          WriteChFreq(true); // Add current frequency to this ch
          u32 = nBandFreq[nBand];
        }
        bReDISP = true;
        break;

      case KEY_RROT:            // Del ch
        if (u32) {              // Not empty ch, can be deleted
          if (YesNo(true)) {    // Confirmed
            WriteChFreq(false); // Delete this ch
            u32 = 0;
          }
          bReDISP = true;
        }
        break;

      case KEY_STEP:
        nStepIdx = (nStepIdx + 1) % NUM_STEPS;
        break;

      case KEY_STEP | KEY_LONGPRESS: // Default step
        nStepIdx = 0;
        break;

      default:
        bReturn = true;
        break;
      }
    }

    if (!(lp % 16))
      CheckUpdateSig();
    HAL_Delay(64);
  }

  LCDClr1();
  LCDUpdate();
  CheckUpdateAlt(ALT_AUTO);
}

void Menu_Help(void) {
  int8_t nItem, nItems, nLine, nLines;
  int8_t i8;
  uint8_t nKey;
  // Line           12              23
  // 012345678901234501234567890123450123456789012345
  const char *s[] =
      {("SQU1:SQUELCH FORLISTEN TO RADIO"),
          ("SQU2:SQUELCH FORSEEK/SCAN/ANY"),
          ("LSIG:LOWER SIG- NAL QUALITY FOR SEEK/SCAN/ANY"),
          ("FMST:FM STEREO  0=FORCE MONO    5=DEFAULT       9=STRONGEST"),
          ("FMCE:FM CHANNEL EQUALIZER"),
          ("FMMP:FM ENHANCEDMULTIPATH SUPP- RESSION"),
          ("DEEM:FM DEEMPHA-SIS CONSTANT"),
          ("BKLT:ADJUST LCD BACKLIGHT"),
          ("KEEP:SECONDS OF LCD KEEP ON"),
          ("ADJ:ADJUST LCD  BRIGHTNESS"),
          ("TSCN:TIME FOR   SCAN STATION"),
          ("TANY:TIME FOR   ANY STATION"),
          ("TIME:ADJUST TIMEOF CLOCK"),
          ("STAT:SHOW OTHER RF STATUS"),
          ("TUNE:SET TUNING METHODS"),
          ("FILT:SET FILTER BANDWIDTH"),
          ("FREQ:TUNING BY  FREQUENCY STEP"),
          ("SEEK:TUNING TO  NEXT STATION"),
          ("CH:TUNING BY    CHANNEL No."),
          ("SCAN:TUNING TO  EVERY STATION & KEEP A WHILE"),
          ("ANY:TUNING TO   ANY STATION.KEEPTILL LOST SIGNAL"),
          ("S&S:SCAN & SAVE STATIONS IN BAND"),
          ("FM-L:BAND BELOW FM BAND"),
          ("SLP:SLEEP MCU"),
          ("YACRC6686 V2 Build 1 2023.1.9")};

  nItems = sizeof(s) / sizeof(const char *);
  // 01234567890123450123456789012345
  LCDFullStr(("TURN ENCODERS TOSHOW HELP INFO"));

  for (nItem = -1;;) {
    if ((nKey = GetKey()) != false) {
      if (!(nKey & (KEY_LROT | KEY_RROT)))
        bExitMenu = 1;
      break;
    }

    if ((i8 = GetRRot()) != false) {
      if (nItem == -1 && i8 < 0)
        nItem = 0;
      nItem = (nItem + i8 + nItems) % nItems;
      LCDFullStr(s[nItem]);
      nLines = (strlen(s[nItem]) - 1) / 16 + 1;
      nLine = 1;
    }

    if ((i8 = GetLRot()) != false) {
      if (nItem == -1) {
        nItem = 0;
        nLines = (strlen(s[0]) - 1) / 16 + 1;
        nLine = 1;
      }

      nLine += i8;
      if (nLine < 1 || nLines <= 2)
        nLine = 1;
      else if (nLine > (nLines - 1))
        nLine = nLines - 1;

      LCDFullStr(s[nItem] + ((nLine - 1) << 4));
    }
  }

  LCDClr();
  TuneFreqDisp();
  LCDUpdate();
}

////////////////////////////////////////////////////////////
// Menu
////////////////////////////////////////////////////////////

bool IsMenuVisible(uint8_t nMenuID) {
  if (nMenuID == MID_FMCE || nMenuID == MID_FMMP || nMenuID == MID_DEEM) {
    if (nRFMode == RFMODE_AM)
      return false;
    uint16_t uDevice;
    // if (nMenuID == MID_FMCE)
    // {
    // 	Get_Cmd(64, 130, &uDevice, 1);
    // 	if ((uDevice & 0xF0) >> 8 != 9)
    // 		return false;
    // }
  }
  return true;
}

void ProcSubMenu(struct M_SUBMENU *pSubMenu) {
  int8_t i8;
  uint8_t u8, lp, nHit;
  int8_t nFirst = 0;

  while (!IsMenuVisible((pSubMenu->pMItem + (nFirst % pSubMenu->nItemCount))->nMID))
    nFirst++;

  for (;;) {
    // Show max 3 sub menu items
    LCDClr1();
    if (pSubMenu->nMID < MID_MIN_AUTORET && pSubMenu->nMID > MID_OPTION) {
      uint8_t textlen = strlen((pSubMenu->pMItem + (nFirst % pSubMenu->nItemCount))->pszMTxt);
      LCDClr1();
      LCDXYStrLen(8 - textlen / 2, 1, (pSubMenu->pMItem + (nFirst % pSubMenu->nItemCount))->pszMTxt, textlen, 1);
    } else {
      for (i8 = 0; i8 < ((pSubMenu->nItemCount < 3) ? pSubMenu->nItemCount : 3); i8++)
        LCDXYStrLen(1 + i8 * 5, 1, (pSubMenu->pMItem + ((nFirst + i8) % pSubMenu->nItemCount))->pszMTxt, 4, 1);
    }

    // Display special indicator char for selected item
    switch (pSubMenu->nMID) {
    case MID_LSIG:
      nHit = nLowerSig;
      break;

    case MID_FMCE:
      nHit = nFMCEQ;
      break;

    case MID_FMMP:
      nHit = nFMEMS;
      break;

    case MID_DIRA:
      nHit = nDIGRA;
      break;

    case MID_DEEM:
      nHit = nDeemphasis;
      break;

    case MID_TUNE:
      nHit = nTuneType;
      break;

    case MID_BAND:
      nHit = nBand;
      break;

    default:
      nHit = MID_NONE; // Magic number, no item selected
      break;
    }

    if (nHit != MID_NONE) {
      nHit = (nHit - nFirst + pSubMenu->nItemCount) % pSubMenu->nItemCount;
      if (nHit <= 2)
        LCDXYChar(nHit * 5, 1, CHAR_SEL);
    }

    for (lp = 0;; lp++) {
      // Process key
      if ((i8 = GetKey()) != false) {
        if (i8 & (KEY_LROT | KEY_RROT)) { // Item selected
          if (pSubMenu->nMID < MID_MIN_AUTORET && pSubMenu->nMID > MID_OPTION)
            u8 = (pSubMenu->pMItem + (nFirst % pSubMenu->nItemCount))->nMID;
          else
            u8 = (pSubMenu->pMItem + ((nFirst + 1) % pSubMenu->nItemCount))->nMID;
          if (u8 == MID_RET)
            return;

          ProcMenuItem(u8);

          if ((pSubMenu->nMID >= MID_MIN_AUTORET) || bExitMenu)
            return; // Auto return sub menu, or none left/right key pressed in sub menu
          else
            break; // Redraw sub menu items
        } else {
          bExitMenu = 1; // None left/right key pressed, exit
          return;
        }
      }

      // Process rotary encoder, adjust nFirst
      if ((i8 = (GetLRot() + GetRRot())) != false) {
        nFirst = (nFirst + i8 + pSubMenu->nItemCount) % pSubMenu->nItemCount;
        if (i8 > 0)
          while (!IsMenuVisible((pSubMenu->pMItem + (nFirst % pSubMenu->nItemCount))->nMID))
            nFirst++;
        else
          while (!IsMenuVisible((pSubMenu->pMItem + (nFirst % pSubMenu->nItemCount))->nMID))
            nFirst--;
        nFirst = (nFirst + pSubMenu->nItemCount) % pSubMenu->nItemCount;
        break;
      }

      // Update sig
      if (!(lp % 16))
        CheckUpdateSig();
      HAL_Delay(64);
    }
  }
}

void ProcMenuItem(uint8_t nMenuID) {
  if (bExitMenu)
    return;

  // Sub menu items
  if (nMenuID <= MID_MAX_SUB) {
    ProcSubMenu(&SM_List[nMenuID]);
    return;
  }

  if ((nMenuID >= MID_FREQ) && (nMenuID <= MID_ANY)) {
    nTuneType = TYPE_FREQ + (nMenuID - MID_FREQ); // Set tune type
    AddSyncBits(NEEDSYNC_TUNE);
    return;
  }

  if ((nMenuID >= MID_LSIGNORM) && (nMenuID <= MID_LSIGLOW)) {
    nLowerSig = nMenuID - MID_LSIGNORM; // Normal/reduced signal quality for seek/scan/any, 0=normal, 1=lower
    AddSyncBits(NEEDSYNC_MISC1);
    return;
  }

  if ((nMenuID >= MID_FMCEOFF) && (nMenuID <= MID_FMCEON)) {
    nFMCEQ = nMenuID - MID_FMCEOFF; // FM channel equalizer, 0=off, 1=on
    SetRFCtrlReg();
    AddSyncBits(NEEDSYNC_MISC2);
    return;
  }

  if ((nMenuID >= MID_FMMPOFF) && (nMenuID <= MID_FMMPON)) {
    nFMEMS = nMenuID - MID_FMMPOFF; // FM enhanced multipath suppression, 0=off, 1=on
    SetRFCtrlReg();
    AddSyncBits(NEEDSYNC_MISC2);
    return;
  }

  if ((nMenuID >= MID_DIRAOFF) && (nMenuID <= MID_DIRAON)) {
    nDIGRA = nMenuID - MID_DIRAOFF; // FM/AM Digital radio, 0=off, 1=on
    SetDigiRadio();
    AddSyncBits(NEEDSYNC_MISC1);
    return;
  }

  if ((nMenuID >= MID_DEEM0) && (nMenuID <= MID_DEEM75)) {
    nDeemphasis = DEEMPHOFF + (nMenuID - MID_DEEM0); // FM de-emphasis, 0=off, 1=50us, 2=75us
    SetRFCtrlReg();
    AddSyncBits(NEEDSYNC_MISC1);
    return;
  }

  // Misc leaf menu items(w/o sub menu)
  switch (nMenuID) {
  case MID_EXIT:
    bExitMenu = 1;

  case MID_RET:
    return;

  case MID_SQUELCH1:
    Menu_Squelch(0);
    break;

  case MID_SQUELCH2:
    Menu_Squelch(1);
    break;

  case MID_BKKEEP:
    Menu_BacklightKeep();
    break;

  case MID_BKADJ:
    Menu_BacklightAdj();
    break;

  case MID_TSCN:
    Menu_ScanStayTime();
    break;

  case MID_TANY:
    Menu_AnyHoldTime();
    break;

  case MID_TIME:
    Menu_Time(); // Adjust time
    break;

  case MID_STAT:
    if (nRFMode == RFMODE_FM)
      Menu_Stat_FM();
    else
      Menu_Stat_AM();
    break;

  case MID_SCSV:
    Menu_SCSV();
    bExitMenu = 1;
    break;

  case MID_HELP: // Show help
    Menu_Help();
    break;
  }
}

void Menu(uint8_t nMenuID) {
  bExitMenu = 0;
  ProcMenuItem(nMenuID);
  LCDClr1();
  LCDUpdate();
}