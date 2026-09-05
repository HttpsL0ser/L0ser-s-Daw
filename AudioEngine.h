#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <atomic>

// Owns the juce::AudioDeviceManager and produces audio. This is the only
// class in Phase 1 that touches the real-time audio callback, and it is
// intentionally small: everything it needs from the UI thread arrives
// through std::atomic fields, never through locks or heap allocation.
class AudioEngine : public juce::AudioIODeviceCallback
{
public:
    AudioEngine();
    ~AudioEngine() override;

    void start();
    void stop();

    // Safe to call from the UI thread: this is a plain atomic store, so
    // there's no risk of the audio thread reading a half-written value.
    void setTestToneEnabled(bool shouldPlay) { testToneEnabled.store(shouldPlay, std::memory_order_relaxed); }
    bool isTestToneEnabled() const { return testToneEnabled.load(std::memory_order_relaxed); }

    juce::AudioDeviceManager& getDeviceManager() { return deviceManager; }

    // --- juce::AudioIODeviceCallback ---------------------------------
    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
                                           int numInputChannels,
                                           float* const* outputChannelData,
                                           int numOutputChannels,
                                           int numSamples,
                                           const juce::AudioIODeviceCallbackContext& context) override;
    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;

private:
    juce::AudioDeviceManager deviceManager;
    std::atomic<bool> testToneEnabled { false };

    // These are only ever touched on the audio thread (set once in
    // audioDeviceAboutToStart, read/updated in the callback), so they're
    // plain doubles rather than atomics.
    double currentSampleRate = 44100.0;
    double phase = 0.0;
    double phaseIncrement = 0.0;
    static constexpr double toneFrequencyHz = 440.0;

    void updatePhaseIncrement();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};
