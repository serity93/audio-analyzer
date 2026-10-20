#pragma once

#include <juce_audio_devices/juce_audio_devices.h>

#include "LevelAnalyzer.h"

// One analyzed input stream: its own audio device (chosen by the user) feeding a LevelAnalyzer.
// Each stream has its own AudioDeviceManager so the two streams can come from different devices.
class AudioStream final : private juce::AudioIODeviceCallback
{
public:
    AudioStream();
    ~AudioStream() override;

    juce::AudioDeviceManager& getDeviceManager()   { return deviceManager; }
    const LevelAnalyzer& getLevels() const         { return levels; }

private:
    void audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                           int numInputChannels,
                                           float* const* outputChannelData,
                                           int numOutputChannels,
                                           int numSamples,
                                           const juce::AudioIODeviceCallbackContext&) override;
    void audioDeviceAboutToStart (juce::AudioIODevice*) override;
    void audioDeviceStopped() override;

    juce::AudioDeviceManager deviceManager;
    LevelAnalyzer levels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioStream)
};
