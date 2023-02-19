#pragma once

#ifndef __UI_H__
#define __UI_H__

#include "global.h"

#define KEY_LROT 0x01
#define KEY_RROT 0x02
#define KEY_TUNE 0x04
#define KEY_STEP 0x08
#define KEY_BAND 0x10
#define KEY_FILTER 0x20
#define KEY_LONGPRESS 0x80

#define TIMER_LONGPRESS 1200 // Key long press time, ms
#define TIMER_LAST_LP 400    // Is last long press key? ms

// Char display on LCD for selected item
#define CHAR_SEL 0x7e // Right arrow

// LCD locations
#define BAND_X 0
#define BAND_Y 0

#define FREQ_X 2
#define FREQ_Y 0

#define RSSI_X 10
#define RSSI_Y 0

#define STEREO_X 12
#define STEREO_Y 0

#define SN_X 13
#define SN_Y 0

#define TYPE_X 0
#define TYPE_Y 1

#define STEP_X 2
#define STEP_Y 1

#define FILTER_X 6
#define FILTER_Y 1

#define ALT_X 11
#define ALT_Y 1

// Display contents in ALT area
#define ALT_AUTO 0
#define ALT_MISC 1
#define ALT_TIME 2
#define ALT_VOL 3

// Menu IDs with sub menus, not auto return
#define MID_OPTION 0x00
#define MID_FREQUENCY 0x01
#define MID_BKLT 0x02

#define MID_RADI 0x03
#define MID_AUDI 0x04
#define MID_APPL 0x05

// Menu IDs of leaves
enum MIDS {
  // Menu IDs with sub menus, auto return
  MID_MIN_AUTORET = 0x06, // Min ID number with auto return property
  MID_LSIG = 0x06,        // Normal/reduced signal quality for seek/scan/any
  MID_FMCE,               // FM channel equalizer
  MID_FMMP,               // FM improved multipath suppression
  MID_FMSI,               // FM Stereo Improvement
  MID_DIRA,               // FM/AM Digital radio
  MID_DEEM,               // FM de-emphasis
  MID_TUNE,
  MID_BAND,
  MID_MAX_SUB, // Max ID number with sub menus

  MID_SQUELCH1 = 0x20,
  MID_SQUELCH2,
  MID_LSIGNORM, // Normal signal quality for seek/scan/any
  MID_LSIGLOW,  // Reduced signal quality for seek/scan/any
  MID_FMCEOFF,
  MID_FMCEON,
  MID_FMMPOFF,
  MID_FMMPON,
  MID_FMSIOFF,
  MID_FMSION,
  MID_DIRAOFF,
  MID_DIRAON,
  MID_DEEM0,
  MID_DEEM50,
  MID_DEEM75,
  MID_BKKEEP,
  MID_BKADJ,
  MID_TSCN,
  MID_TANY,
  MID_TIME,
};

#define MID_STAT 0x42

#define MID_FREQ 0x45
#define MID_SEEK 0x46
#define MID_CH 0x47
#define MID_SCAN 0x48
#define MID_ANY 0x49
#define MID_SCSV 0x4a

#define MID_LW 0x50
#define MID_MW 0x51
#define MID_SW 0x52
#define MID_FL 0x53
#define MID_FM 0x54

#define MID_HELP 0xf1

// Special menu IDs
#define MID_RET 0xfe
#define MID_EXIT 0xff
#define MID_NONE 0xf0 // Magic number, no item selected

// Menu item structure
struct M_ITEM {
  uint8_t nMID;
  const char *pszMTxt;
};

// Sub menu structure
struct M_SUBMENU {
  uint8_t nMID;
  struct M_ITEM *pMItem;
  uint8_t nItemCount;
};

void LCDSetBackLight(const uint8_t Data);
void LCDEN(void);
void LCDCmd(const uint8_t Cmd);
void LCDData(const uint8_t Data);
void LCDClr(void);
void LCDClr1(void);
void LCDOn(void);
void LCDInit(void);
void LCDXY(const uint8_t x, const uint8_t y);
void LCDXYChar(const uint8_t x, const uint8_t y, const char c);
void LCDXYStr(const uint8_t x, const uint8_t y, const char *str);
void LCDFullStr(const char *str);
void LCDXYStrLen(const uint8_t x, const uint8_t y, const char *str, const uint8_t nLen, const bool bLeftAlign);
void LCDXYIntLen(const uint8_t x, const uint8_t y, const int32_t n, const uint8_t nLen);
void LCDXYUIntLenZP(const uint8_t x, const uint8_t y, const uint32_t n, const uint8_t nLen);

int8_t GetLRot(void);
int8_t GetRRot(void);
uint8_t PeekKey(void);
uint8_t GetKey(void);
void TestRotKey(void);

void LCDUpdate(void);
void CheckUpdateSig(void);
void ShowMisc(void);
void ShowTime(void);
void ShowVol(void);
void CheckUpdateAlt(int8_t nShow);

void Menu_Squelch(uint8_t nIdx);
void Menu_BacklightAdj(void);
void Menu_BacklightKeep(void);
void Menu_ScanStayTime(void);
void Menu_AnyHoldTime(void);
void Menu_Time(void);
void sprhex2(char *str, uint8_t v);
void Menu_Stat_FM(void);
void Menu_Stat_AM(void);
bool YesNo(bool bChkSig);
void Menu_SCSV(void);
void AddDelCh(void);
void Menu_Help(void);

bool IsMenuVisible(uint8_t nMenuID);
void ProcSubMenu(struct M_SUBMENU *pSubMenu);
void ProcMenuItem(uint8_t nMenuID);
void Menu(uint8_t nMenuID);

#endif