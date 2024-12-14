import 'package:flutter/material.dart';
import 'package:flutter_c_miniaudio/src/native_audio.dart';

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
    if (initAudio() != 0) {
      debugPrint("Failed to initialize audio");
    } else {
      debugPrint("Audio initialized successfully");
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
