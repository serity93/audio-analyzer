#include "AudioStream.h"

AudioStream::AudioStream()
{
    // Input only: open the default input device with up to maxChannels channels, no outputs.
    const auto error = deviceManager.initialiseWithDefaultDevices (LevelAnalyzer::maxChannels, 0);

    if (error.isNotEmpty())
        DBG ("Couldn't open the default audio device: " << error);

    deviceManager.addAudioCallback (this);
}

AudioStream::~AudioStream()
{
    deviceManager.removeAudioCallback (this);
    deviceManager.closeAudioDevice();
}

void AudioStream::audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                                    int numInputChannels,
                                                    float* const* outputChannelData,
                                                    int numOutputChannels,
                                                    int numSamples,
                                                    const juce::AudioIODeviceCallbackContext&)
{
    levels.process (inputChannelData, juce::jmin (numInputChannels, LevelAnalyzer::maxChannels), numSamples);

    // We never play anything, but any output channels we're handed must still be silenced.
    for (int ch = 0; ch < numOutputChannels; ++ch)
        if (outputChannelData[ch] != nullptr)
            juce::FloatVectorOperations::clear (outputChannelData[ch], numSamples);
}

void AudioStream::audioDeviceAboutToStart (juce::AudioIODevice* device)
{
    const auto numInputs = device->getActiveInputChannels().countNumberOfSetBits();
    levels.prepare (device->getCurrentSampleRate(), juce::jmin (numInputs, LevelAnalyzer::maxChannels));
}

void AudioStream::audioDeviceStopped()
{
    levels.reset();
}
