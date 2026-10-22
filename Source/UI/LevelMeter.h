#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Analysis/LevelAnalyzer.h"

// A vertical peak/RMS meter for each channel of a LevelAnalyzer, with a dBFS scale and
// numeric readouts. It polls the analyzer on a timer; all the measuring happens in the analyzer.
class LevelMeter final : public juce::Component,
                         private juce::Timer
{
public:
    explicit LevelMeter (const LevelAnalyzer&);

    void paint (juce::Graphics&) override;

private:
    void timerCallback() override;

    static constexpr float minDb = -60.0f;
    static constexpr float maxDb = 0.0f;

    const LevelAnalyzer& analyzer;

    int numChannels = 0;
    std::array<float, LevelAnalyzer::maxChannels> peakDb {}, rmsDb {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
