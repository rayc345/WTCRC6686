#pragma once
#include "Tuner_Interface.h"
#include <initializer_list>

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
  Cmd_Set_Keycode = 2,
  Cmd_Set_GPIO = 3,
  Cmd_Set_ReferenceClock = 4,
  Cmd_Activate = 5,
  Cmd_Get_Operation_Status = 128,
  Cmd_Get_GPIO_Status = 129,
  Cmd_Get_Identification = 130,
  Cmd_Get_LastWrite = 131,
  // Cmd_Get_Interface_Status = 135,
} TEF_APPL_COMMAND;

bool devTEF_FM_Tune_To(uint16_t frequency);
bool devTEF_FM_Set_Bandwidth(uint16_t mode, uint16_t bandwidth, uint16_t control_sensitivity, uint16_t low_level_sensitivity);
bool devTEF_FM_Set_RFAGC(uint16_t start);
bool devTEF_FM_Set_MphSuppression(uint16_t mph);
bool devTEF_FM_Set_ChannelEqualizer(uint16_t eq);
bool devTEF_FM_Set_NoiseBlanker(uint8_t mode, uint16_t start);
bool devTEF_FM_Set_DigitalRadio(uint16_t dr);
bool devTEF_FM_Set_Deemphasis(uint16_t timeconstant);
bool devTEF_FM_Set_StereoImprovement(uint16_t fmsi);
bool devTEF_FM_Set_LevelOffset(int16_t offset);
bool devTEF_FM_Set_Highcut_Level(uint16_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_Highcut_Noise(uint16_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_Highcut_Mph(uint16_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_Highcut_Max(uint16_t mode, uint16_t limit);
bool devTEF_FM_Set_Stereo_Level(uint16_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_Stereo_Noise(uint16_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_Stereo_Mph(uint16_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_Stereo_Min(uint16_t mode, uint16_t limit);
bool devTEF_FM_Set_StHiBlend_Level(uint8_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_StHiBlend_Noise(uint8_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_StHiBlend_Mph(uint8_t mode, uint16_t start, uint16_t slope);
bool devTEF_FM_Set_StHiBlend_Max(uint8_t mode, uint16_t limit);
bool devTEF_FM_Set_RDS(void);
bool devTEF_FM_Set_DR_Options(uint16_t samplerate, uint16_t mode, uint16_t format);
bool devTEF_FM_Specials(uint16_t audio);
bool devTEF_FM_Set_StereoBandBlend_Time(uint16_t attack, uint16_t decay);
bool devTEF_FM_Set_StereoBandBlend_Gain(uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4);
bool devTEF_FM_Set_StereoBandBlend_Bias(int16_t band1, int16_t band2, int16_t band3, int16_t band4);

bool devTEF_AM_Tune_To(uint16_t frequency);
bool devTEF_AM_Set_Bandwidth(uint16_t bandwidth);
bool devTEF_AM_Set_Antenna(uint16_t attenuation);
bool devTEF_AM_Set_CoChannelDet(uint16_t sensitivity);
bool devTEF_AM_Set_NoiseBlanker(uint8_t mode, uint16_t sensitivity);
bool devTEF_AM_Set_DigitalRadio(uint16_t dr);
bool devTEF_AM_Set_DR_Options(uint16_t samplerate, uint16_t mode, uint16_t format);

bool devTEF_Audio_Set_Volume(int16_t volume);
bool devTEF_Audio_Set_Mute(uint16_t mode);

bool devTEF_APPL_Set_OperationMode(uint16_t mode);
bool devTEF_APPL_Set_Keycode(uint16_t key_high, uint16_t key_low);
bool devTEF_APPL_Set_GPIO(uint16_t pin, uint16_t module, uint16_t feature);
bool devTEF_APPL_Set_ReferenceClock(uint32_t frequency, uint16_t type);
bool devTEF_APPL_Activate(uint16_t mode);

bool devTEF_FM_Get_Quality_Status(int16_t *level, uint16_t *usn, uint16_t *wam, int16_t *offset, uint16_t *bandwidth, uint16_t *mod);
bool devTEF_FM_Get_RDS_Data(uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error);
bool devTEF_FM_Get_AGC(uint16_t *input_att, uint16_t *feedback_att);
bool devTEF_FM_Get_Signal_Status(uint16_t *status);
bool devTEF_FM_Get_Processing_Status(uint16_t *softmute, uint16_t *highcut, uint16_t *stereo, uint16_t *sthiblend, uint8_t *stband_1, uint8_t *stband_2, uint8_t *stband_3, uint8_t *stband_4);
bool devTEF_FM_Get_Interface_Status(uint16_t *samplerate);

bool devTEF_AM_Get_Quality_Status(int16_t *level, uint16_t *noise, uint16_t *co_channel, int16_t *offset, uint16_t *bandwidth, uint16_t *mod);
bool devTEF_AM_Get_Interface_Status(uint16_t *samplerate);

bool devTEF_APPL_Get_Operation_Status(uint8_t *bootstatus);
bool devTEF_APPL_Get_Identification(uint16_t *device, uint16_t *hw_version, uint16_t *sw_version);
bool devTEF_APPL_Get_Interface_Status(uint16_t *samplerate);