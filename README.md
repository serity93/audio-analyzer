# Audio Analyzer

A Windows desktop app for analyzing two audio streams side by side: peak/RMS (dBFS),
loudness (LUFS), waveform and spectrogram, with per-stream gain for level matching.

Built with C++20 and [JUCE](https://juce.com) 9, using CMake.

## Prerequisites

- Visual Studio 2026 (Community is free) or Build Tools for Visual Studio 2026 with the
  **Desktop development with C++** workload (includes the MSVC compiler, Windows SDK and CMake)
- Git (CMake uses it to download JUCE on the first configure)

## Build

From a **Developer PowerShell for VS 2026** (so `cmake` is on PATH), in the repo root:

```powershell
cmake --preset default          # configure (first run downloads JUCE into build/_deps)
cmake --build --preset debug    # or: --preset release
```

The app is written to:

```
build/AudioAnalyzer_artefacts/Debug/Audio Analyzer.exe
```

To build, run and debug in an IDE, either:

- **Visual Studio 2026:** open `build/AudioAnalyzer.slnx`, or use File → Open → Folder on the repo
  root. Older versions (e.g. Visual Studio 2022) can't open the `.slnx` solution or use the 2026
  (v145) compiler toolset.
- **VS Code:** install the C/C++ and CMake Tools extensions, open the repo folder, and select the
  `default` preset with the Visual Studio Build Tools 2026 kit.

## Project layout

```
CMakeLists.txt       build definition (fetches JUCE, defines the app target)
CMakePresets.json    configure/build presets
Source/
  Main.cpp           application entry point and main window
  MainComponent.*    the window's content: a device selector and meter per stream
  AudioStream.*      one input stream: its own audio device, feeding a LevelAnalyzer
  LevelAnalyzer.*    peak/RMS measurement (runs on the audio thread)
  LevelMeter.*       peak/RMS meter display (polls a LevelAnalyzer)
```

## Licence note

JUCE is dual-licensed under the AGPLv3 or the JUCE licence (which has a free tier). If this
repository is public and open source, AGPLv3-compatible licensing is the simplest route.
