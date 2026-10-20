#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "AudioStream.h"
#include "LevelMeter.h"

// The main content of the window: a device selector and peak/RMS meter for each of the two streams.
class MainComponent final : public juce::Component
{
public:
    MainComponent();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // The UI for one stream: a title, its audio device selector and its meter.
    class StreamPanel final : public juce::Component
    {
    public:
        StreamPanel (const juce::String& name, AudioStream&);

        void resized() override;

    private:
        juce::Label title;
        juce::AudioDeviceSelectorComponent deviceSelector;
        LevelMeter meter;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StreamPanel)
    };

    // The streams must outlive the panels that refer to them, so they're declared first.
    AudioStream streamA, streamB;
    StreamPanel panelA { "Stream A", streamA };
    StreamPanel panelB { "Stream B", streamB };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
