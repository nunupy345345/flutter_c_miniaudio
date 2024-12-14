#include <string.h>
#define MINIAUDIO_IMPLEMENTATION // Miniaudio の関数実装を有効にする
#include "miniaudio.h"

ma_device device;
ma_device_config deviceConfig;

// リングバッファを追加
#define BUFFER_SIZE 4800 // 100ms at 48kHz
ma_int16 inputBuffer[BUFFER_SIZE];
int writePos = 0;
int readPos = 0;

// コールバック関数
void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  ma_int16 *output = (ma_int16 *)pOutput;
  const ma_int16 *input = (const ma_int16 *)pInput;

  if (pInput && pOutput)
  {
    for (ma_uint32 i = 0; i < frameCount; i++)
    {
      // 入力サンプルをバッファに書き込む
      inputBuffer[writePos] = input[i];
      writePos = (writePos + 1) % BUFFER_SIZE;

      // ピッチを下げるために2サンプルおきに読み出す
      output[i] = inputBuffer[readPos];
      readPos = (readPos + 2) % BUFFER_SIZE;
    }
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

  // バッファを初期化
  memset(inputBuffer, 0, sizeof(inputBuffer));
  writePos = 0;
  readPos = 0;

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
