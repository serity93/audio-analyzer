#pragma once

#include <array>
#include <atomic>

// Measures the peak and RMS levels of one audio stream, per channel.
//
// Threading: prepare(), reset() and process() are called on the audio thread (or while the
// device is stopped). getPeak(), getRms() and getNumChannels() are called on the message
// thread by the meters, about 30 times a second, so anything they read has to be safe to share
// between the two threads - which is why the results below are std::atomic.
class LevelAnalyzer
{
public:
    static constexpr int maxChannels = 2;

    // Called before audio starts (and again whenever the device, sample rate or channels change).
    void prepare (double sampleRate, int numChannels);

    // Called when the device stops. Should put the meters back to silence.
    void reset();

    // Called for every block of incoming audio. channelData holds one pointer per active input
    // channel (numInputChannels <= maxChannels), each pointing at numSamples samples in [-1, 1].
    void process (const float* const* channelData, int numInputChannels, int numSamples);

    // Results, as linear amplitude (1.0 = 0 dBFS). The meters convert these to dB for display.
    float getPeak (int channel) const   { return peak[(size_t) channel].load (std::memory_order_relaxed); }
    float getRms (int channel) const    { return rms[(size_t) channel].load (std::memory_order_relaxed); }
    int getNumChannels() const          { return numChannels.load (std::memory_order_relaxed); }

private:
    std::atomic<int> numChannels { 0 };
    std::array<std::atomic<float>, maxChannels> peak {};
    std::array<std::atomic<float>, maxChannels> rms {};

    // TODO: add whatever state your peak/RMS calculations need here.
};
