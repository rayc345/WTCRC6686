#include "Tuner_Interface.h"
#include "Tuner_Patch_Lithio_V101_p120.h"
#include "Tuner_Patch_Lithio_V102_p224.h"
#include "Tuner_Patch_Lithio_V205_p512.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

static const uint8_t tuner_init_tab[] = {
    7, 0x20, 0x0B, 0x01, 0x03, 0x98, 0x00, 0x00,                          // FM_Set_RFAGC(1,920,0)
    5, 0x20, 0x14, 0x01, 0x00, 0x00,                                      // FM_Set_MphSuppression(1, 0)
    5, 0x20, 0x16, 0x01, 0x00, 0x00,                                      // FM_Set_ChannelEqualizer(1, 0)
    7, 0x20, 0x17, 0x01, 0x00, 0x00, 0x03, 0xE8,                          // FM_Set_NoiseBlanker(1,0,1000)
    5, 0x20, 0x20, 0x01, 0x00, 0x01,                                      // FM_Set_StereoImprovement(1,1)
    5, 0x20, 0x1F, 0x01, 0x01, 0xF4,                                      // FM_Set_Deemphasis(1,500)
    9, 0x20, 0x2A, 0x01, 0x00, 0x00, 0x01, 0xF4, 0x00, 0xDC,              // FM_Set_Softmute_Level(1,0,500,220)
    9, 0x20, 0x2B, 0x01, 0x00, 0x00, 0x01, 0xF4, 0x03, 0xE8,              // FM_Set_Softmute_Noise(1,0,500,1000)
    9, 0x20, 0x2C, 0x01, 0x00, 0x00, 0x01, 0xF4, 0x03, 0xE8,              // FM_Set_Softmute_Mph(1,0,500,1000)
    7, 0x20, 0x2D, 0x01, 0x00, 0x00, 0x00, 0xC8,                          // FM_Set_Softmute_Max(1,0,)
    11, 0x20, 0x32, 0x01, 0x00, 0x3C, 0x00, 0x78, 0x00, 0xC8, 0x00, 0xC8, // FM_Set_Highcut_Time(1,60,120,200,200)
    11, 0x20, 0x33, 0x01, 0x00, 0x00, 0x00, 0xFA, 0x00, 0x82, 0x01, 0xF4, // FM_Set_Highcut_Mod(1,0,250,130,500)
    9, 0x20, 0x36, 0x01, 0x00, 0x00, 0x01, 0x68, 0x01, 0x2C,              // FM_Set_Highcut_Mph(1,0,360,300)
    7, 0x20, 0x37, 0x01, 0x00, 0x00, 0x0F, 0xA0,                          // FM_Set_Highcut_Max(1,0,4000)
    7, 0x20, 0x39, 0x01, 0x00, 0x00, 0x00, 0x64,                          // FM_Set_Lowcut_Max(1,0,100)
    7, 0x20, 0x3A, 0x01, 0x00, 0x00, 0x00, 0xAA,                          // FM_Set_Lowcut_Min(1,0,170)
    5, 0x20, 0x3B, 0x01, 0x00, 0x01,                                      // FM_Set_Highcut_Options(1,1)
    11, 0x20, 0x3C, 0x01, 0x00, 0x3C, 0x00, 0x78, 0x00, 0x64, 0x00, 0xC8, // FM_Set_Stereo_Time(1,60,120,100,200)
    11, 0x20, 0x3D, 0x01, 0x00, 0x00, 0x00, 0xD2, 0x00, 0x5A, 0x01, 0xF4, // FM_Set_Stereo_Mod(1,0,210,90,500)
    11, 0x20, 0x46, 0x01, 0x01, 0xF4, 0x07, 0xD0, 0x00, 0xC8, 0x00, 0xC8, // FM_Set_StHiBlend_Time(1,500,2000,200,200)
    11, 0x20, 0x47, 0x01, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x78, 0x02, 0x9E, // FM_Set_StHiBlend_Mod(1,0,240,120,670)
    9, 0x20, 0x48, 0x01, 0x00, 0x00, 0x02, 0x58, 0x00, 0xF0,              // FM_Set_StHiBlend_Level(1,0,600,240)
    9, 0x20, 0x49, 0x01, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x8C,              // FM_Set_StHiBlend_Noise(1,0,160,140)
    9, 0x20, 0x4A, 0x01, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x8C,              // FM_Set_StHiBlend_Mph(1,0,160,140)
    7, 0x20, 0x4B, 0x01, 0x00, 0x00, 0x0F, 0xA0,                          // FM_Set_StHiBlend_Max(1,0,4000)
    5, 0x20, 0x56, 0x01, 0x03, 0x84,                                      // FM_Set_Bandwidth_Options(1,900)
    7, 0x30, 0x15, 0x01, 0x00, 0x80, 0x00, 0x01,                          // AUDIO_Set_Ana_Out(1,128,1)
    5, 0x21, 0x27, 0x01, 0xFF, 0x06                                       // AM_Set_LevelOffset(1,-250)
};

bool Tuner_WriteBuffer(uint8_t *buf, uint16_t len) {
  HAL_StatusTypeDef r = HAL_I2C_Master_Transmit(&hi2c1, 0xC8, buf, len, 100);
  return r == HAL_OK;
}

bool Tuner_ReadBuffer(uint8_t *buf, uint16_t len) {
  HAL_StatusTypeDef r = HAL_I2C_Master_Receive(&hi2c1, 0xC8, buf, len, 100);
  return r == HAL_OK;
}

bool Tuner_Patch_Load(const uint8_t *pLutBytes, uint16_t size) {
  uint8_t buf[24 + 1];
  uint16_t i, len;
  bool r;
  buf[0] = 0x1B;

  while (size) {
    len = (size > 24) ? 24 : size;
    size -= len;

    for (i = 0; i < len; i++)
      buf[1 + i] = pLutBytes[i];

    pLutBytes += len;

    if (true != (r = Tuner_WriteBuffer(buf, len + 1))) {
      break;
    }
  }
  return r;
}

bool Tuner_Patch(const uint8_t TEF) {
  uint8_t uData[24];
  uData[0] = 0x1E;
  uData[1] = 0x5A;
  uData[2] = 0x01;
  uData[3] = 0x5A;
  uData[4] = 0x5A;
  if (!Tuner_WriteBuffer(uData, 5))
    return false;
  HAL_Delay(15);
  uData[0] = 0x1C;
  uData[1] = 0x00;
  uData[2] = 0x00;
  if (!Tuner_WriteBuffer(uData, 3))
    return false;
  uData[2] = 0x74;
  if (!Tuner_WriteBuffer(uData, 3))
    return false;
  if (TEF == 101) {
    if (!Tuner_Patch_Load(pPatchBytes101, PatchSize101))
      return false;
  } else if (TEF == 102) {
    if (!Tuner_Patch_Load(pPatchBytes102, PatchSize102))
      return false;
  } else if (TEF == 205) {
    if (!Tuner_Patch_Load(pPatchBytes205, PatchSize205))
      return false;
  }
  uData[0] = 0x1C;
  uData[1] = 0x00;
  uData[2] = 0x00;
  if (!Tuner_WriteBuffer(uData, 3))
    return false;
  uData[2] = 0x75;
  if (!Tuner_WriteBuffer(uData, 3))
    return false;
  if (TEF == 102) {
    if (!Tuner_Patch_Load(pLutBytes102, LutSize102))
      return false;
  } else if (TEF == 205) {
    if (!Tuner_Patch_Load(pLutBytes205, LutSize205))
      return false;
  }
  uData[0] = 0x1C;
  uData[1] = 0x00;
  uData[2] = 0x00;
  if (!Tuner_WriteBuffer(uData, 3))
    return false;
  uData[0] = 0x14;
  uData[1] = 0x00;
  uData[2] = 0x01;
  if (!Tuner_WriteBuffer(uData, 3))
    return false;
  HAL_Delay(50);
  return true;
}

bool Tuner_Table_Write(const uint8_t *tab) {
  if (tab[1] == 0xFF) {
    HAL_Delay(tab[2]);
    return true;
  } else {
    return Tuner_WriteBuffer((uint8_t *)&tab[1], tab[0]);
  }
}

bool Tuner_Init(void) {
  bool r;
  const uint8_t *p = tuner_init_tab;

  for (uint16_t i = 0; i < sizeof(tuner_init_tab); i += (p[i] + 1)) {
    if (true != (r = Tuner_Table_Write(p + i)))
      break;
  }
  return r;
}