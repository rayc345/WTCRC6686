#pragma once
#include "main.h"
#include <cstdbool>
#include <cstddef>
#include <cstdint>

typedef enum {
  TEF_FM = 32,
  TEF_AM = 33,
  TEF_AUDIO = 48,
  TEF_APPL = 64
} TEF_MODULE;

typedef enum {
  Cmd_Tune_To = 1,
  Cmd_Set_Tune_Options = 2,
  Cmd_Set_Bandwidth = 10,
  Cmd_Set_RFAGC = 11,
  Cmd_Set_Antenna = 12,
  Cmd_Set_CoChannelDet = 14,
  Cmd_Set_MphSuppression = 20,
  Cmd_Set_ChannelEqualizer = 22,
  Cmd_Set_NoiseBlanker = 23,
  Cmd_Set_NoiseBlanker_Options = 24,
  Cmd_Set_NoiseBlanker_Audio = 24,
  Cmd_Set_DigitalRadio = 30,
  Cmd_Set_Deemphasis = 31,
  Cmd_Set_StereoImprovement = 32,
  Cmd_Set_Highcut_Fix = 33,
  Cmd_Set_Lowcut_Fix = 34,
  Cmd_Set_LevelStep = 38,
  Cmd_Set_LevelOffset = 39,

  Cmd_Set_Softmute_Time = 40,
  Cmd_Set_Softmute_Mod = 41,
  Cmd_Set_Softmute_Level = 42,
  Cmd_Set_Softmute_Noise = 43,
  Cmd_Set_Softmute_Mph = 44,
  Cmd_Set_Softmute_Max = 45,

  Cmd_Set_Highcut_Time = 50,
  Cmd_Set_Highcut_Mod = 51,
  Cmd_Set_Highcut_Level = 52,
  Cmd_Set_Highcut_Noise = 53,
  Cmd_Set_Highcut_Mph = 54,
  Cmd_Set_Highcut_Max = 55,
  Cmd_Set_Highcut_Min = 56,
  Cmd_Set_Lowcut_Max = 57,
  Cmd_Set_Lowcut_Min = 58,
  Cmd_Set_Highcut_Options = 59,

  Cmd_Set_Stereo_Time = 60,
  Cmd_Set_Stereo_Mod = 61,
  Cmd_Set_Stereo_Level = 62,
  Cmd_Set_Stereo_Noise = 63,
  Cmd_Set_Stereo_Mph = 64,
  Cmd_Set_Stereo_Max = 65,
  Cmd_Set_Stereo_Min = 66,

  Cmd_Set_StHiBlend_Time = 70,
  Cmd_Set_StHiBlend_Mod = 71,
  Cmd_Set_StHiBlend_Level = 72,
  Cmd_Set_StHiBlend_Noise = 73,
  Cmd_Set_StHiBlend_Mph = 74,
  Cmd_Set_StHiBlend_Max = 75,
  Cmd_Set_StHiBlend_Min = 76,

  Cmd_Set_Scaler = 80,
  Cmd_Set_RDS = 81,
  Cmd_Set_QualityStatus = 82,
  Cmd_Set_DR_Blend = 83,
  Cmd_Set_DR_Options = 84,
  Cmd_Set_Specials = 85,
  Cmd_Set_Bandwidth_Options = 86,

  Cmd_Set_StBandBlend_Time = 90,
  Cmd_Set_StBandBlend_Gain = 91,
  Cmd_Set_StBandBlend_Bias = 92,

  Cmd_Get_Quality_Status = 128,
  Cmd_Get_Quality_Data = 129,
  Cmd_Get_RDS_Status = 130,
  Cmd_Get_RDS_Data = 131,
  Cmd_Get_AGC = 132,
  Cmd_Get_Signal_Status = 133,
  Cmd_Get_Processing_Status = 134,
  Cmd_Get_Interface_Status = 135,
} TEF_RADIO_COMMAND;

typedef enum {
  Cmd_Set_Volume = 10,
  Cmd_Set_Mute = 11,
  Cmd_Set_Input = 12,
  Cmd_Set_Output_Source = 13,
  Cmd_Set_Ana_Out = 21,
  Cmd_Set_Dig_IO = 22,
  Cmd_Set_Input_Scaler = 23,
  Cmd_Set_WaveGen = 24,
} TEF_AUDIO_COMMAND;

typedef enum {
  Cmd_Set_OperationMode = 1,
  Cmd_Set_GPIO = 3,
  Cmd_Set_ReferenceClock = 4,
  Cmd_Activate = 5,
  Cmd_Get_Operation_Status = 128,
  Cmd_Get_GPIO_Status = 129,
  Cmd_Get_Identification = 130,
  Cmd_Get_LastWrite = 131,
  // Cmd_Get_Interface_Status = 135,
} TEF_APPL_COMMAND;

bool Tuner_Patch(const uint8_t TEF);
bool Tuner_Init(void);
bool Tuner_Init4000(void);
bool Tuner_Init9216(void);
bool Tuner_Init12000(void);
bool Tuner_WriteBuffer(uint8_t *buf, uint16_t len);
bool Tuner_ReadBuffer(uint8_t *buf, uint16_t len);