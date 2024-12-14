import 'package:flutter/material.dart';
import 'package:flutter_c_miniaudio/src/native_audio.dart';
import 'package:permission_handler/permission_handler.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: AudioApp(),
    );
  }
}

class AudioApp extends StatefulWidget {
  @override
  _AudioAppState createState() => _AudioAppState();
}

class _AudioAppState extends State<AudioApp> {
  bool isRecording = false;

  @override
  void initState() {
    super.initState();
    _requestPermissions();
    if (initAudio() != 0) {
      debugPrint("Failed to initialize audio");
    } else {
      debugPrint("Audio initialized successfully");
    }
  }

  Future<void> _requestPermissions() async {
    final status = await Permission.microphone.request();
    if (status.isGranted) {
      if (initAudio() != 0) {
        debugPrint("Failed to initialize audio");
      } else {
        debugPrint("Audio initialized successfully");
      }
    } else {
      debugPrint("Microphone permission denied");
    }
  }

  void toggleRecording() {
    if (isRecording) {
      stopAudio();
      initAudio();
    } else {
      if (startAudio() != 0) {
        debugPrint("Failed to start audio");
        return;
      }
    }
    setState(() {
      isRecording = !isRecording;
    });
  }

  @override
  void dispose() {
    stopAudio();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Miniaudio Example'),
      ),
      body: Center(
        child: ElevatedButton(
          onPressed: toggleRecording,
          child: Text(isRecording ? 'Stop' : 'Start'),
        ),
      ),
    );
  }
}
