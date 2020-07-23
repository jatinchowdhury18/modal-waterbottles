#include "Mode.h"

using namespace juce;

Mode::Mode()
{
}

void Mode::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fs = (float) sampleRate;

    osc.prepare (sampleRate);

    renderBuffer.setSize (1, samplesPerBlock);
}

void Mode::process (AudioBuffer<float>& buffer, const int numSamples)
{
    auto* data = renderBuffer.getWritePointer (0);

    for (int n = 0; n < numSamples; ++n)
        data[n] = osc.getNextSample();

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.addFrom (ch, 0, renderBuffer, 0, 0, numSamples);
}

void Mode::processVibrato (AudioBuffer<float>& buffer, const AudioBuffer<float>& freqBuffer, const int numSamples)
{
    auto* data = renderBuffer.getWritePointer (0);
    auto* freq = freqBuffer.getReadPointer (0);

    for (int n = 0; n < numSamples; ++n)
    {
        osc.setFreq (oscFreq + freq[n]);
        data[n] = osc.getNextSample();
    }

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.addFrom (ch, 0, renderBuffer, 0, 0, numSamples);
}
