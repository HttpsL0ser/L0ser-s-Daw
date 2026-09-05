# MyDAW — Phase 1

Skeleton + audio engine, per the project brief. Opens a window, initializes
the default audio device, and plays a 440 Hz test tone on demand via a
button — proving the audio callback is live before any sequencing/MIDI work
starts in Phase 2.

## Build

Requires CMake 3.22+, a C++20 compiler, and network access (CMake's
`FetchContent` pulls JUCE from GitHub on first configure — that's the one
step that couldn't be run in the sandbox this was written in).

```bash
cmake -B build -S .
cmake --build build --config Release
```

The JUCE fetch is pinned to tag `7.0.12` in the top-level `CMakeLists.txt`;
bump that if you want a newer JUCE release.

## What's here

```
CMakeLists.txt
src/
  audio/AudioEngine.h/.cpp   -- AudioDeviceManager wrapper, real-time-safe sine callback
  app/MainComponent.h/.cpp   -- window contents: status label + tone toggle
  app/Main.cpp               -- JUCEApplication entry point
```

Nothing else is scaffolded yet — per the brief, later-phase folders
(sequencer, midi, mixer, plugins, project) get created when we get there.

## Design decisions flagged for you

- **JUCE license**: dual GPLv3 / commercial. Worth deciding your licensing
  model before Phase 6 (third-party plugin hosting), since that's usually
  where "is this commercial software" stops being hypothetical.
- **Thread safety in `AudioEngine`**: the only cross-thread communication is
  a single `std::atomic<bool>` (`testToneEnabled`). That pattern (atomics in,
  never anything back out except via a lock-free queue) is what should carry
  forward into Phase 2's step sequencer — flag it again then if a queue
  becomes necessary for note events.
