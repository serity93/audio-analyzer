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

    // Peaks at or above this count as clipping. It's just under 0 dBFS because integer input
    // converted to float tops out slightly below 1.0 on the positive side (32767/32768 for 16-bit).
    static constexpr float clipDb = -0.01f;

    // How long the peak-hold marker stays at a new high before following the peak back down.
    static constexpr double peakHoldMs = 1500.0;

    const LevelAnalyzer& analyzer;

    int numChannels = 0;
    std::array<float, LevelAnalyzer::maxChannels> peakDb {}, rmsDb {};
    std::array<float, LevelAnalyzer::maxChannels> peakHoldDb {};
    std::array<double, LevelAnalyzer::maxChannels> peakHoldStartMs {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
