
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ma_engine engine;
ma_sound sound;

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  if (pInput == NULL || pOutput == NULL)
  {
    return; // 無音
  }

  /* 入力をそのまま出力に渡す（ただし、ここでピッチが処理される） */
  MA_COPY_MEMORY(pOutput, pInput, frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));
}

int main(int argc, char **argv)
{
  ma_result result;
  ma_device_config deviceConfig;
  ma_device device;

  /* エンジンの初期化 */
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS)
  {
    printf("Failed to initialize audio engine.\n");
    return -1;
  }

  /* デバイスの設定 */
  deviceConfig = ma_device_config_init(ma_device_type_duplex);
  deviceConfig.capture.format = ma_format_f32; // フロート形式で処理
  deviceConfig.capture.channels = 2;
  deviceConfig.playback.format = ma_format_f32;
  deviceConfig.playback.channels = 2;
  deviceConfig.sampleRate = 48000;           // 固定サンプリングレート
  deviceConfig.dataCallback = data_callback; // コールバック関数を設定

  result = ma_device_init(NULL, &deviceConfig, &device);
  if (result != MA_SUCCESS)
  {
    printf("Failed to initialize audio device.\n");
    ma_engine_uninit(&engine);
    return -1;
  }

  /* サウンドオブジェクトの作成 */
  result = ma_sound_init(&engine, NULL, MA_SOUND_FLAG_STREAM, NULL, &sound);
  if (result != MA_SUCCESS)
  {
    printf("Failed to initialize sound.\n");
    ma_device_uninit(&device);
    ma_engine_uninit(&engine);
    return -1;
  }

  /* ピッチを設定: 0.5は一オクターブ下 */
  ma_sound_set_pitch(&sound, 0.5f);

  /* デバイス開始 */
  result = ma_device_start(&device);
  if (result != MA_SUCCESS)
  {
    printf("Failed to start audio device.\n");
    ma_sound_uninit(&sound);
    ma_device_uninit(&device);
    ma_engine_uninit(&engine);
    return -1;
  }

  printf("Audio device started. Speak into the microphone...\n");
  printf("Press Enter to quit...\n");
  getchar();

  /* 後処理 */
  ma_sound_uninit(&sound);
  ma_device_uninit(&device);
  ma_engine_uninit(&engine);

  return 0;
}