import 'dart:ffi';
import 'dart:io';

final DynamicLibrary nativeAudioLib = Platform.isAndroid
    ? DynamicLibrary.open("libaudio_processing.so")
    : DynamicLibrary.process();

// init_audio: 初期化
final int Function() initAudio = nativeAudioLib
    .lookup<NativeFunction<Int32 Function()>>("init_audio")
    .asFunction();

// start_audio: 再生開始
final int Function() startAudio = nativeAudioLib
    .lookup<NativeFunction<Int32 Function()>>("start_audio")
    .asFunction();

// stop_audio: 再生停止
final void Function() stopAudio = nativeAudioLib
    .lookup<NativeFunction<Void Function()>>("stop_audio")
    .asFunction();
