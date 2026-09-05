#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../audio/AudioEngine.h"

// Phase 1 is deliberately thin: prove the audio device opens and produces
// sound, driven by a UI toggle. Channel rack / piano roll / mixer views
// get their own files in later phases rather than being stubbed here.
class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AudioEngine audioEngine;
    juce::TextButton toneToggleButton { "Play Test Tone" };
    juce::Label statusLabel;

    void toggleTone();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
