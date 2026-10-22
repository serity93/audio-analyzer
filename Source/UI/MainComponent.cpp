#include "UI/MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible (panelA);
    addAndMakeVisible (panelB);
    setSize (1100, 560);
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Divider between the two stream panels.
    g.setColour (juce::Colours::white.withAlpha (0.15f));
    g.drawVerticalLine (getWidth() / 2, 8.0f, (float) getHeight() - 8.0f);
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    panelA.setBounds (area.removeFromLeft (area.getWidth() / 2));
    panelB.setBounds (area);
}

MainComponent::StreamPanel::StreamPanel (const juce::String& name, AudioStream& stream)
    : deviceSelector (stream.getDeviceManager(),
                      1, LevelAnalyzer::maxChannels,   // input channels: min, max
                      0, 0,                            // output channels: none
                      false, false,                    // no MIDI
                      false,                           // pick channels individually, not as stereo pairs
                      true),                           // hide sample rate/buffer size behind a button
      meter (stream.getLevels())
{
    title.setText (name, juce::dontSendNotification);
    title.setFont (juce::FontOptions (20.0f, juce::Font::bold));

    addAndMakeVisible (title);
    addAndMakeVisible (deviceSelector);
    addAndMakeVisible (meter);
}

void MainComponent::StreamPanel::resized()
{
    auto area = getLocalBounds().reduced (10);
    title.setBounds (area.removeFromTop (30));
    area.removeFromTop (6);

    meter.setBounds (area.removeFromRight (140));
    area.removeFromRight (10);

    // The selector sets its own height to fit its contents, so only its width really matters here.
    deviceSelector.setBounds (area);
}
