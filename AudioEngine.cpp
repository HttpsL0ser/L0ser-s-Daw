#include "AudioEngine.h"
#include <cmath>

AudioEngine::AudioEngine()
{
    // 0 inputs, 2 outputs: Phase 1 only needs to prove sound comes out.
    // MIDI/audio input arrives in Phase 2.
    deviceManager.initialiseWithDefaultDevices(0, 2);
}

AudioEngine::~AudioEngine()
{
    stop();
}

void AudioEngine::start()
{
    deviceManager.addAudioCallback(this);
}

void AudioEngine::stop()
{
    deviceManager.removeAudioCallback(this);
}

void AudioEngine::updatePhaseIncrement()
{
    phaseIncrement = juce::MathConstants<double>::twoPi * toneFrequencyHz / currentSampleRate;
}

void AudioEngine::audioDeviceAboutToStart(juce::AudioIODevice* device)
{
    // Called on a non-realtime thread before the stream starts, so it's
    // fine to do normal work here - just not inside the callback below.
    currentSampleRate = device->getCurrentSampleRate();
    phase = 0.0;
    updatePhaseIncrement();
}

void AudioEngine::audioDeviceStopped()
{
    // No dynamically allocated audio resources to release in Phase 1.
}

void AudioEngine::audioDeviceIOCallbackWithContext(const float* const* /*inputChannelData*/,
                                                    int /*numInputChannels*/,
                                                    float* const* outputChannelData,
                                                    int numOutputChannels,
                                                    int numSamples,
                                                    const juce::AudioIODeviceCallbackContext& /*context*/)
{
    // REAL-TIME THREAD. No allocation, no locks, no logging, no JUCE
    // Component calls. The atomic load below is the one and only piece
    // of cross-thread state this callback touches.
    const bool playTone = testToneEnabled.load(std::memory_order_relaxed);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float value = 0.0f;

        if (playTone)
        {
            // 0.2 gain (~ -14 dB) so the test tone doesn't blast the speakers.
            value = static_cast<float>(std::sin(phase) * 0.2);

            phase += phaseIncrement;
            if (phase >= juce::MathConstants<double>::twoPi)
                phase -= juce::MathConstants<double>::twoPi;
        }

        for (int channel = 0; channel < numOutputChannels; ++channel)
            outputChannelData[channel][sample] = value;
    }
}
