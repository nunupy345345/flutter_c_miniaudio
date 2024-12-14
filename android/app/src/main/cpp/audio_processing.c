#include <string.h>
#define MINIAUDIO_IMPLEMENTATION // Miniaudio の関数実装を有効にする
#include "miniaudio.h"

ma_device device;
ma_device_config deviceConfig;

// コールバック関数
void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  if (pInput && pOutput)
  {
    memcpy(pOutput, pInput, frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));
  }
}

// 初期化関数
int init_audio()
{
  deviceConfig = ma_device_config_init(ma_device_type_duplex);
  deviceConfig.capture.format = ma_format_s16;
  deviceConfig.capture.channels = 1;
  deviceConfig.playback.format = ma_format_s16;
  deviceConfig.playback.channels = 1;
  deviceConfig.sampleRate = 48000;
  deviceConfig.dataCallback = data_callback;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
  {
    return -1;
  }
  return 0;
}

int start_audio()
{
  if (ma_device_start(&device) != MA_SUCCESS)
  {
    return -1;
  }
  return 0;
}

void stop_audio()
{
  ma_device_uninit(&device);
}
