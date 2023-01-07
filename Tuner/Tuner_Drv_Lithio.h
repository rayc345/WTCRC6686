#pragma once
#include "Tuner_Interface.h"
#include <initializer_list>

bool devTEF_FM_Tune_To(uint16_t frequency);
bool devTEF_FM_Set_Bandwidth(uint16_t mode, uint16_t bandwidth, uint16_t control_sensitivity, uint16_t low_level_sensitivity);
bool devTEF_FM_Set_RFAGC(uint16_t start);
bool devTEF_FM_Set_MphSuppression(uint16_t mph);
bool devTEF_FM_Set_ChannelEqualizer(uint16_t eq);
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
bool devTEF_FM_Set_Stereo_Min(uint16_t mode);
bool devTEF_FM_Set_RDS(void);
bool devTEF_FM_Set_DR_Options(uint16_t samplerate, uint16_t mode, uint16_t format);

bool devTEF_AM_Tune_To(uint16_t frequency);
bool devTEF_AM_Set_Bandwidth(uint16_t bandwidth);
bool devTEF_AM_Set_DigitalRadio(uint16_t dr);
bool devTEF_AM_Set_DR_Options(uint16_t samplerate, uint16_t mode, uint16_t format);

bool devTEF_Audio_Set_Volume(int16_t volume);
bool devTEF_Audio_Set_Mute(uint16_t mode);

bool devTEF_APPL_Set_OperationMode(uint16_t mode);

bool devTEF_FM_Get_Quality_Status(int16_t *level, uint16_t *usn, uint16_t *wam, int16_t *offset, uint16_t *bandwidth, uint16_t *mod);
bool devTEF_FM_Get_RDS_Data(uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error);
bool devTEF_FM_Get_AGC(uint16_t *input_att, uint16_t *feedback_att);
bool devTEF_FM_Get_Signal_Status(uint16_t *status);
bool devTEF_FM_Get_Interface_Status(uint16_t *samplerate);

bool devTEF_AM_Get_Quality_Status(int16_t *level, uint16_t *noise, uint16_t *co_channel, int16_t *offset, uint16_t *bandwidth, uint16_t *mod);
bool devTEF_AM_Get_Interface_Status(uint16_t *samplerate);

bool devTEF_APPL_Get_Operation_Status(uint8_t *bootstatus);
bool devTEF_APPL_Get_Identification(uint16_t *device, uint16_t *hw_version, uint16_t *sw_version);
bool devTEF_APPL_Get_Interface_Status(uint16_t *samplerate);