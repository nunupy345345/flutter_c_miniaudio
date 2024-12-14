import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart';

// C側の構造体を表現するクラス
final class AudioContext extends ffi.Struct {
  external ffi.Pointer<ffi.Void> device;
  @ffi.Bool()
  external bool is_running;
  @ffi.Float()
  external double volume;
}

// C関数のタイプ定義
typedef CreateAudioContextNative = ffi.Pointer<AudioContext> Function();
typedef CreateAudioContext = ffi.Pointer<AudioContext> Function();

typedef StartAudioNative = ffi.Void Function(ffi.Pointer<AudioContext>);
typedef StartAudio = void Function(ffi.Pointer<AudioContext>);

typedef StopAudioNative = ffi.Void Function(ffi.Pointer<AudioContext>);
typedef StopAudio = void Function(ffi.Pointer<AudioContext>);

typedef SetVolumeNative = ffi.Void Function(
    ffi.Pointer<AudioContext>, ffi.Float);
typedef SetVolume = void Function(ffi.Pointer<AudioContext>, double);

typedef DestroyAudioContextNative = ffi.Void Function(
    ffi.Pointer<AudioContext>);
typedef DestroyAudioContext = void Function(ffi.Pointer<AudioContext>);

// AudioProcessorクラスの実装
class AudioProcessor {
  late final ffi.DynamicLibrary _lib;
  late final CreateAudioContext _createAudioContext;
  late final StartAudio _startAudio;
  late final StopAudio _stopAudio;
  late final SetVolume _setVolume;
  late final DestroyAudioContext _destroyAudioContext;
  late final ffi.Pointer<AudioContext> _context;
  bool _isInitialized = false;

  AudioProcessor() {
    _lib = ffi.DynamicLibrary.open('libaudio_processing.so');
    _loadFunctions();
  }

  void _loadFunctions() {
    _createAudioContext =
        _lib.lookupFunction<CreateAudioContextNative, CreateAudioContext>(
            'create_audio_context');
    _startAudio =
        _lib.lookupFunction<StartAudioNative, StartAudio>('start_audio');
    _stopAudio = _lib.lookupFunction<StopAudioNative, StopAudio>('stop_audio');
    _setVolume = _lib.lookupFunction<SetVolumeNative, SetVolume>('set_volume');
    _destroyAudioContext =
        _lib.lookupFunction<DestroyAudioContextNative, DestroyAudioContext>(
            'destroy_audio_context');
  }

  bool initialize() {
    if (_isInitialized) return true;

    _context = _createAudioContext();
    if (_context == ffi.nullptr) return false;

    _isInitialized = true;
    return true;
  }

  void start() {
    if (!_isInitialized) return;
    _startAudio(_context);
  }

  void stop() {
    if (!_isInitialized) return;
    _stopAudio(_context);
  }

  void setVolume(double volume) {
    if (!_isInitialized) return;
    _setVolume(_context, volume);
  }

  void dispose() {
    if (_isInitialized) {
      _destroyAudioContext(_context);
      _isInitialized = false;
    }
  }

  bool get isRunning => _isInitialized && _context.ref.is_running;
}
