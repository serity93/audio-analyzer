#include "UI/LevelMeter.h"

namespace
{
    constexpr int scaleWidth = 34;
    constexpr int readoutHeight = 36;
    constexpr int barGap = 6;

    juce::String formatDb (float db, float floorDb)
    {
        return db <= floorDb ? juce::String ("-inf") : juce::String (db, 1);
    }
}

LevelMeter::LevelMeter (const LevelAnalyzer& analyzerToShow)
    : analyzer (analyzerToShow)
{
    peakDb.fill (minDb);
    rmsDb.fill (minDb);
    peakHoldDb.fill (minDb);
    startTimerHz (30);
}

void LevelMeter::timerCallback()
{
    numChannels = analyzer.getNumChannels();
    const auto now = juce::Time::getMillisecondCounterHiRes();

    // Decibels below minDb are shown as -inf (an empty bar).
    for (int ch = 0; ch < numChannels; ++ch)
    {
        peakDb[(size_t) ch] = juce::Decibels::gainToDecibels (analyzer.getPeak (ch), minDb);
        rmsDb[(size_t) ch]  = juce::Decibels::gainToDecibels (analyzer.getRms (ch), minDb);

        // Peak hold: catch new highs straight away, keep them for peakHoldMs, then follow the peak down.
        auto& held = peakHoldDb[(size_t) ch];

        if (peakDb[(size_t) ch] >= held)
        {
            held = peakDb[(size_t) ch];
            peakHoldStartMs[(size_t) ch] = now;
        }
        else if (now - peakHoldStartMs[(size_t) ch] > peakHoldMs)
        {
            held = peakDb[(size_t) ch];
        }
    }

    repaint();
}

void LevelMeter::paint (juce::Graphics& g)
{
    auto area = getLocalBounds().reduced (4);
    auto readouts = area.removeFromBottom (readoutHeight);
    auto scale = area.removeFromLeft (scaleWidth);
    auto bars = area.reduced (0, 6);   // leave room for the top/bottom scale labels

    const auto dbToY = [&bars] (float db)
    {
        return juce::jmap (juce::jlimit (minDb, maxDb, db), minDb, maxDb,
                           (float) bars.getBottom(), (float) bars.getY());
    };

    // Scale: tick labels on the left, faint lines across the bars.
    g.setFont (juce::FontOptions (11.0f));

    for (auto db : { 0.0f, -6.0f, -12.0f, -18.0f, -24.0f, -36.0f, -48.0f, -60.0f })
    {
        const auto y = dbToY (db);
        g.setColour (juce::Colours::grey);
        g.drawText (juce::String ((int) db), scale.getX(), (int) y - 6, scale.getWidth() - 4, 12,
                    juce::Justification::centredRight);
        g.setColour (juce::Colours::white.withAlpha (0.08f));
        g.drawHorizontalLine ((int) y, (float) bars.getX(), (float) bars.getRight());
    }

    // Row labels for the readouts.
    g.setColour (juce::Colours::grey);
    auto labels = readouts.withX (scale.getX()).withWidth (scale.getWidth() - 4);
    g.drawText ("Pk", labels.removeFromTop (readoutHeight / 2), juce::Justification::centredRight);
    g.drawText ("RMS", labels, juce::Justification::centredRight);

    if (numChannels == 0)
    {
        g.drawText ("No input", bars, juce::Justification::centred);
        return;
    }

    const auto barWidth = (bars.getWidth() - barGap * (numChannels - 1)) / numChannels;

    for (int ch = 0; ch < numChannels; ++ch)
    {
        const auto x = bars.getX() + ch * (barWidth + barGap);
        const auto bar = juce::Rectangle<int> (x, bars.getY(), barWidth, bars.getHeight()).toFloat();
        const auto peak = peakDb[(size_t) ch];
        const auto held = peakHoldDb[(size_t) ch];
        const auto rms = rmsDb[(size_t) ch];

        g.setColour (juce::Colours::black.withAlpha (0.4f));
        g.fillRect (bar);

        // RMS as a solid bar, peak as a lighter bar behind it, and the held peak as a line
        // (red once it has clipped).
        g.setColour (juce::Colours::limegreen.withAlpha (0.35f));
        g.fillRect (bar.withTop (dbToY (peak)));
        g.setColour (juce::Colours::limegreen);
        g.fillRect (bar.withTop (dbToY (rms)));

        if (held > minDb)
        {
            g.setColour (held >= clipDb ? juce::Colours::red : juce::Colours::white);
            g.fillRect (bar.withTop (dbToY (held)).withHeight (2.0f));
        }

        // Numeric readouts under the bar. The peak readout shows the held peak so it's readable.
        auto readout = juce::Rectangle<int> (x, readouts.getY(), barWidth, readoutHeight);
        g.setColour (juce::Colours::white);
        g.drawText (formatDb (held, minDb), readout.removeFromTop (readoutHeight / 2), juce::Justification::centred);
        g.drawText (formatDb (rms, minDb), readout, juce::Justification::centred);
    }
}
