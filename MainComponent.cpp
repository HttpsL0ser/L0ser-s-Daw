#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(toneToggleButton);
    addAndMakeVisible(statusLabel);

    statusLabel.setText("Audio device ready.", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);

    toneToggleButton.onClick = [this] { toggleTone(); };

    setSize(500, 300);

    // Start the callback as soon as the window exists. Real engines would
    // surface device-open failures to the UI; that's a Phase-1-plus concern
    // once we have somewhere to show an error.
    audioEngine.start();
}

MainComponent::~MainComponent()
{
    audioEngine.stop();
}

void MainComponent::toggleTone()
{
    const bool nowPlaying = !audioEngine.isTestToneEnabled();
    audioEngine.setTestToneEnabled(nowPlaying);
    toneToggleButton.setButtonText(nowPlaying ? "Stop Test Tone" : "Play Test Tone");
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    statusLabel.setBounds(area.removeFromTop(40));
    area.removeFromTop(20);
    toneToggleButton.setBounds(area.removeFromTop(40).reduced(100, 0));
}
