#include "Analysis/LevelAnalyzer.h"

#include <algorithm>
#include <cmath>

#include <juce_audio_basics/juce_audio_basics.h>

void LevelAnalyzer::prepare (double sampleRate, int newNumChannels)
{
    numChannels.store (newNumChannels, std::memory_order_relaxed);

    // time constant: tau = window / 2
    const auto timeConstantMs = rmsWindowMs / 2.0;

    // smoothing coefficient: a = 1 - e^(-dt/(tau * sampleRate)), dt = 1/sampleRate (s) or 1000/sampleRate (ms)
    rmsAlpha = 1.0 - std::exp(-1000.0 / (timeConstantMs * sampleRate));

    // gain = 10^(dB / 20)
    // dBPerSample = -dBPerSecond / sampleRate
    // gainPerSample    = 10^((dBPerSample) / 20)
    //                  = 10^((-dBPerSecond / sampleRate) / 20)
    rmsFallPerSample = std::pow(10.0, -rmsFallRateDbPerSecond / sampleRate / 20.0);

    reset();
}

void LevelAnalyzer::reset()
{
    meanSquare.fill(0.0);
    calculatedRmsValue.fill(0.0);

    for (auto& value : rms)
        value.store(0.0f, std::memory_order_relaxed);

    for (auto& value : peak)
        value.store(0.0f, std::memory_order_relaxed);
}

void LevelAnalyzer::process (const float* const* channelData, int numInputChannels, int numSamples)
{
    juce::ScopedNoDenormals noDenormals;

    const auto blockFall = std::pow(rmsFallPerSample, (double)numSamples);

    for (int ch = 0; ch < numInputChannels; ++ch)
    {
        const auto* samples = channelData[ch];

        if (samples == nullptr)
            continue;

        // get mean square from last block
        auto blockMeanSquare = meanSquare[(size_t)ch];

        // calculate new mean square value
        for (int i = 0; i < numSamples; ++i)
        {
            const auto sample = (double)samples[i];
            blockMeanSquare += rmsAlpha * (sample * sample - blockMeanSquare);
        }

        // store new mean square
        meanSquare[(size_t)ch] = blockMeanSquare;

        // get RMS of last block
        auto& blockRms = calculatedRmsValue[(size_t)ch];

        // calculate max of RMS and RMS with fall factor
        blockRms = std::max(std::sqrt(blockMeanSquare), blockRms * blockFall);

        // store new RMS value
        rms[(size_t)ch].store((float)blockRms, std::memory_order_relaxed);
    }
}
