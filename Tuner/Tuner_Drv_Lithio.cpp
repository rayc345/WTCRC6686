#include "Tuner_Drv_Lithio.h"
#include "Tuner_Interface.h"

#define High_16bto8b(a) ((uint8_t)(((a) >> 8) & 0xFF))
#define Low_16bto8b(a) ((uint8_t)(a))
#define Convert8bto16b(a) ((uint16_t)(((uint16_t)(*(a))) << 8 | ((uint16_t)(*(a + 1)))))

bool devTEF_Set_Cmd(TEF_MODULE module, uint8_t cmd, std::initializer_list<int> params) {
  uint16_t txSize = 3, temp;
  uint8_t buf[20];

  buf[0] = module;
  buf[1] = cmd;
  buf[2] = 1;

  for (auto p = params.begin(); p != params.end(); p++) {
    buf[txSize++] = High_16bto8b(*p);
    buf[txSize++] = Low_16bto8b(*p);
  }

  return Tuner_WriteBuffer(buf, txSize);
}

bool devTEF_Get_Cmd(TEF_MODULE module, uint8_t cmd, uint8_t *receive, uint16_t len) {
  uint8_t buf[3];

  buf[0] = module;
  buf[1] = cmd;
  buf[2] = 1;

  Tuner_WriteBuffer(buf, 3);
  return Tuner_ReadBuffer(receive, len);
}

bool devTEF_FM_Tune_To(uint16_t frequency) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, {1, frequency});
}

bool devTEF_FM_Set_Bandwidth(uint16_t mode, uint16_t bandwidth, uint16_t control_sensitivity, uint16_t low_level_sensitivity) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Bandwidth, {mode, bandwidth, control_sensitivity, low_level_sensitivity});
}

bool devTEF_FM_Set_RFAGC(uint16_t start) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_RFAGC, {start, 0});
}

bool devTEF_FM_Set_MphSuppression(uint16_t mph) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_MphSuppression, {mph});
}

bool devTEF_FM_Set_ChannelEqualizer(uint16_t eq) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_ChannelEqualizer, {eq});
}

bool devTEF_FM_Set_NoiseBlanker(uint8_t mode, uint16_t start) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_NoiseBlanker, {mode, start});
}

bool devTEF_FM_Set_DigitalRadio(uint16_t dr) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_DigitalRadio, {dr});
}

bool devTEF_FM_Set_Deemphasis(uint16_t timeconstant) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Deemphasis, {timeconstant});
}

bool devTEF_FM_Set_StereoImprovement(uint16_t fmsi) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_StereoImprovement, {fmsi});
}

bool devTEF_FM_Set_LevelOffset(int16_t offset) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_LevelOffset, {offset * 10});
}

bool devTEF_FM_Set_Highcut_Level(uint16_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Level, {mode, start, slope});
}

bool devTEF_FM_Set_Highcut_Noise(uint16_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Noise, {mode, start, slope});
}

bool devTEF_FM_Set_Highcut_Mph(uint16_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Mph, {mode, start, slope});
}

bool devTEF_FM_Set_Highcut_Max(uint16_t mode, uint16_t limit) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Highcut_Max, {mode, limit});
}

bool devTEF_FM_Set_Stereo_Level(uint16_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Level, {mode, start, slope});
}

bool devTEF_FM_Set_Stereo_Noise(uint16_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Noise, {mode, start, slope});
}

bool devTEF_FM_Set_Stereo_Mph(uint16_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Mph, {mode, start, slope});
}

bool devTEF_FM_Set_Stereo_Min(uint16_t mode, uint16_t limit) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Stereo_Min, {mode, limit});
}

bool devTEF_FM_Set_StHiBlend_Level(uint8_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Level, {mode, start, slope});
}

bool devTEF_FM_Set_StHiBlend_Noise(uint8_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Noise, {mode, start, slope});
}

bool devTEF_FM_Set_StHiBlend_Mph(uint8_t mode, uint16_t start, uint16_t slope) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Mph, {mode, start, slope});
}

bool devTEF_FM_Set_StHiBlend_Max(uint8_t mode, uint16_t limit) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_StHiBlend_Max, {mode, limit});
}

bool devTEF_FM_Set_RDS(void) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_RDS, {1, 2, 0});
}

bool devTEF_FM_Set_DR_Options(uint16_t samplerate, uint16_t mode, uint16_t format) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_DR_Options, {samplerate, mode, format});
}

bool devTEF_FM_Specials(uint16_t audio) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_Specials, {audio});
}

bool devTEF_FM_Set_StereoBandBlend_Time(uint16_t attack, uint16_t decay) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_StBandBlend_Time, {attack, decay});
}

bool devTEF_FM_Set_StereoBandBlend_Gain(uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_StBandBlend_Gain, {band1, band2, band3, band4});
}

bool devTEF_FM_Set_StereoBandBlend_Bias(int16_t band1, int16_t band2, int16_t band3, int16_t band4) {
  return devTEF_Set_Cmd(TEF_FM, Cmd_Set_StBandBlend_Bias, {band1, band2, band3, band4});
}

bool devTEF_AM_Tune_To(uint16_t frequency) {
  return devTEF_Set_Cmd(TEF_AM, Cmd_Tune_To, {1, frequency});
}

bool devTEF_AM_Set_Bandwidth(uint16_t bandwidth) {
  return devTEF_Set_Cmd(TEF_AM, Cmd_Set_Bandwidth, {0, bandwidth});
}

bool devTEF_AM_Set_Antenna(uint16_t attenuation) {
  return devTEF_Set_Cmd(TEF_AM, Cmd_Set_Antenna, {attenuation});
}

bool devTEF_AM_Set_CoChannelDet(uint16_t sensitivity) {
  return devTEF_Set_Cmd(TEF_AM, Cmd_Set_CoChannelDet, {1, 2, sensitivity, 3});
}

bool devTEF_AM_Set_NoiseBlanker(uint8_t mode, uint16_t sensitivity) {
  return devTEF_Set_Cmd(TEF_AM, Cmd_Set_NoiseBlanker, {mode, sensitivity});
}

bool devTEF_AM_Set_DigitalRadio(uint16_t dr) {
  return devTEF_Set_Cmd(TEF_AM, Cmd_Set_DigitalRadio, {dr});
}

bool devTEF_AM_Set_DR_Options(uint16_t samplerate, uint16_t mode, uint16_t format) {
  return devTEF_Set_Cmd(TEF_AM, Cmd_Set_DR_Options, {samplerate, mode, format});
}

bool devTEF_Audio_Set_Volume(int16_t volume) {
  return devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Volume, {volume * 10});
}

bool devTEF_Audio_Set_Mute(uint16_t mode) {
  return devTEF_Set_Cmd(TEF_AUDIO, Cmd_Set_Mute, {mode});
}

bool devTEF_APPL_Set_OperationMode(uint16_t mode) {
  return devTEF_Set_Cmd(TEF_APPL, Cmd_Set_OperationMode, {mode});
}

bool devTEF_FM_Get_Quality_Status(int16_t *level, uint16_t *usn, uint16_t *wam, int16_t *offset, uint16_t *bandwidth, uint16_t *mod) {
  uint8_t buf[14];
  uint16_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_Quality_Data, buf, sizeof(buf));

  if (level) {
    *level = Convert8bto16b(buf + 2) / 10;
  }
  if (usn) {
    *usn = Convert8bto16b(buf + 4) / 10;
  }
  if (wam) {
    *wam = Convert8bto16b(buf + 6) / 10;
  }
  if (offset) {
    *offset = Convert8bto16b(buf + 8) / 10;
  }
  if (bandwidth) {
    *bandwidth = Convert8bto16b(buf + 10) / 10;
  }
  if (mod) {
    *mod = Convert8bto16b(buf + 12) / 10;
  }
  return r;
}

bool devTEF_FM_Get_RDS_Data(uint16_t *status, uint16_t *A_block, uint16_t *B_block, uint16_t *C_block, uint16_t *D_block, uint16_t *dec_error) {
  uint8_t buf[12];
  uint8_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_RDS_Data, buf, sizeof(buf));

  if (status) {
    *status = Convert8bto16b(buf);
  }
  if (A_block) {
    *A_block = Convert8bto16b(buf + 2);
  }
  if (B_block) {
    *B_block = Convert8bto16b(buf + 4);
  }
  if (C_block) {
    *C_block = Convert8bto16b(buf + 6);
  }
  if (D_block) {
    *D_block = Convert8bto16b(buf + 8);
  }
  if (dec_error) {
    *dec_error = Convert8bto16b(buf + 10);
  }
  return r;
}

bool devTEF_FM_Get_AGC(uint16_t *input_att, uint16_t *feedback_att) {
  uint8_t buf[4];
  uint8_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_AGC, buf, sizeof(buf));

  if (input_att) {
    *input_att = Convert8bto16b(buf) / 10;
  }
  if (feedback_att) {
    *feedback_att = Convert8bto16b(buf + 2) / 10;
  }
  return r;
}

bool devTEF_FM_Get_Signal_Status(uint16_t *status) {
  uint8_t buf[2];
  uint16_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_Signal_Status, buf, sizeof(buf));

  if (status) {
    *status = Convert8bto16b(buf);
  }
  return r;
}

bool devTEF_FM_Get_Processing_Status(uint16_t *softmute, uint16_t *highcut, uint16_t *stereo, uint16_t *sthiblend, uint8_t *stband_1, uint8_t *stband_2, uint8_t *stband_3, uint8_t *stband_4) {
  uint8_t buf[12];
  uint16_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_Processing_Status, buf, sizeof(buf));
  if (softmute) {
    *softmute = Convert8bto16b(buf) / 10;
  }
  if (highcut) {
    *highcut = Convert8bto16b(buf + 2) / 10;
  }
  if (stereo) {
    *stereo = Convert8bto16b(buf + 4) / 10;
  }
  if (sthiblend) {
    *sthiblend = Convert8bto16b(buf + 6) / 10;
  }
  uint16_t stband_1_2 = Convert8bto16b(buf + 8);
  uint16_t stband_3_4 = Convert8bto16b(buf + 10);
  if (stband_1) {
    *stband_1 = High_16bto8b(stband_1_2);
  }
  if (stband_2) {
    *stband_2 = Low_16bto8b(stband_1_2);
  }
  if (stband_3) {
    *stband_3 = High_16bto8b(stband_3_4);
  }
  if (stband_4) {
    *stband_4 = Low_16bto8b(stband_3_4);
  }
  return r;
}

bool devTEF_FM_Get_Interface_Status(uint16_t *samplerate) {
  uint8_t buf[2];
  uint16_t r = devTEF_Get_Cmd(TEF_FM, Cmd_Get_Interface_Status, buf, sizeof(buf));

  if (samplerate) {
    *samplerate = Convert8bto16b(buf);
  }
  return r;
}

bool devTEF_AM_Get_Quality_Status(int16_t *level, uint16_t *noise, uint16_t *co_channel, int16_t *offset, uint16_t *bandwidth, uint16_t *mod) {
  uint8_t buf[14];
  uint16_t r = devTEF_Get_Cmd(TEF_AM, Cmd_Get_Quality_Status, buf, sizeof(buf));

  if (level) {
    *level = Convert8bto16b(buf + 2) / 10;
  }
  if (noise) {
    *noise = Convert8bto16b(buf + 4) / 10;
  }
  if (co_channel) {
    *co_channel = Convert8bto16b(buf + 6);
  }
  if (offset) {
    *offset = Convert8bto16b(buf + 8) / 10;
  }
  if (bandwidth) {
    *bandwidth = Convert8bto16b(buf + 10) / 10;
  }
  if (mod) {
    *mod = Convert8bto16b(buf + 12) / 10;
  }
  return r;
}

bool devTEF_AM_Get_Interface_Status(uint16_t *samplerate) {
  uint8_t buf[2];
  uint16_t r = devTEF_Get_Cmd(TEF_AM, Cmd_Get_Interface_Status, buf, sizeof(buf));

  if (samplerate) {
    *samplerate = Convert8bto16b(buf);
  }
  return r;
}

bool devTEF_APPL_Get_Operation_Status(uint8_t *bootstatus) {
  uint8_t buf[2];
  uint16_t r = devTEF_Get_Cmd(TEF_APPL, Cmd_Get_Operation_Status, buf, sizeof(buf));
  if (bootstatus) {
    *bootstatus = Convert8bto16b(buf);
  }
  return r;
}

bool devTEF_APPL_Get_Identification(uint16_t *device, uint16_t *hw_version, uint16_t *sw_version) {
  uint8_t buf[6];
  uint16_t r = devTEF_Get_Cmd(TEF_APPL, Cmd_Get_Identification, buf, sizeof(buf));

  if (device) {
    *device = Convert8bto16b(buf);
  }
  if (hw_version) {
    *hw_version = Convert8bto16b(buf + 2);
  }
  if (sw_version) {
    *sw_version = Convert8bto16b(buf + 4);
  }
  return r;
}

bool devTEF_APPL_Get_Interface_Status(uint16_t *samplerate) {
  uint8_t buf[2];
  uint16_t r = devTEF_Get_Cmd(TEF_APPL, Cmd_Get_Interface_Status, buf, sizeof(buf));

  if (samplerate) {
    *samplerate = Convert8bto16b(buf);
  }
  return r;
}