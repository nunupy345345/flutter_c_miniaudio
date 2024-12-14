#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdlib.h>

ma_device *device = NULL;
float volume = 1.0f;

void audio_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  MA_ASSERT(pDevice->capture.format == pDevice->playback.format);
  MA_ASSERT(pDevice->capture.channels == pDevice->playback.channels);

  // 音量調整を行う
  ma_int16 *output = (ma_int16 *)pOutput;
  const ma_int16 *input = (const ma_int16 *)pInput;
  for (ma_uint32 i = 0; i < frameCount * pDevice->capture.channels; i++)
  {
    output[i] = (ma_int16)(input[i] * volume);
  }
}

// FFIから呼び出す関数群
void *initialize_audio()
{
  if (device != NULL)
    return device;

  device = (ma_device *)malloc(sizeof(ma_device));
  ma_device_config config = ma_device_config_init(ma_device_type_duplex);

  config.capture.format = ma_format_s16;
  config.capture.channels = 1;
  config.playback.format = ma_format_s16;
  config.playback.channels = 1;
  config.dataCallback = audio_callback;

  if (ma_device_init(NULL, &config, device) != MA_SUCCESS)
  {
    free(device);
    device = NULL;
    return NULL;
  }

  return device;
}

int start_audio()
{
  if (device == NULL)
    return -1;
  return ma_device_start(device);
}

int stop_audio()
{
  if (device == NULL)
    return -1;
  return ma_device_stop(device);
}

void set_volume(float vol)
{
  volume = vol;
}

void cleanup_audio()
{
  if (device != NULL)
  {
    ma_device_uninit(device);
    free(device);
    device = NULL;
  }
}