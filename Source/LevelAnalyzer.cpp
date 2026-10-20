#include "LevelAnalyzer.h"

void LevelAnalyzer::prepare ([[maybe_unused]] double sampleRate, int newNumChannels)
{
    numChannels.store (newNumChannels, std::memory_order_relaxed);

    // TODO: set up your peak/RMS state for this sample rate and channel count.
}

void LevelAnalyzer::reset()
{
    // TODO: clear your state and store 0 into peak and rms so the meters drop to silence.
}

void LevelAnalyzer::process ([[maybe_unused]] const float* const* channelData,
                             [[maybe_unused]] int numInputChannels,
                             [[maybe_unused]] int numSamples)
{
    // TODO: calculate the peak and RMS level of each channel and store them into peak[ch] and
    // rms[ch].
    //
    // Things to think about:
    //  - Blocks arrive far more often (e.g. every 10 ms) than the meters read them (every ~33 ms),
    //    so if you only store each block's own peak, the meters can miss short peaks.
    //  - Over what length of time should RMS be measured? A single block is quite short and
    //    its length depends on the device's buffer size.
    //  - Should the meters fall back gradually after a peak (ballistics), and if so, where?
    //  - This runs on the real-time audio thread: no allocating, locking or logging in here.
}
