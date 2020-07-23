#ifndef MODEOSCILLATOR_H_INCLUDED
#define MODEOSCILLATOR_H_INCLUDED

#include "JuceHeader.h"
#include <complex>

class ModeOscillator
{
public:
    ModeOscillator() {}

    void prepare (double sampleRate)
    {
        fs = (float) sampleRate;

        decayFactor = pow (0.001f, 1.0f / (t60 * fs));
        oscCoef = exp (jImag * juce::MathConstants<float>::twoPi * (freq / fs));
        updateParams();
        maxSamples = juce::int64 (t60 * fs);

        reset();
    }

    inline void reset() noexcept
    {
        y1 = 0.0f;
    }

    inline void setAmp (std::complex<float> amp) noexcept
    {
        A = amp;
    }

    inline void setDecay (float newT60, float dampFactor = 1.0f) noexcept
    {
        // if (t60 == newT60)
        //     return;

        t60 = newT60;
        decayFactor = pow (0.001f, 1.0f / (t60 * fs)) * dampFactor;
        updateParams();
        maxSamples = int (1.5f * t60 * fs);
    }

    inline void setFreq (float newFreq) noexcept
    {
        if (freq == newFreq)
            return;

        freq = newFreq;
        oscCoef = exp (jImag * juce::MathConstants<float>::twoPi * (freq / fs));
        updateParams();
    }

    inline void updateParams() noexcept { filtCoef = decayFactor * oscCoef; }

    inline bool isPlaying() const noexcept
    {
        return sampleCnt < maxSamples;
    }

    void start (float amp = 1.0f)
    {
        y1 += A * amp;
        sampleCnt = 0;
    }

    inline float getNextSample() noexcept
    {
        sampleCnt++;

        auto y = filtCoef * y1;

        y1 = y;
        return std::imag (y);
    }

private:
    std::complex<float> filtCoef = 0.0f;
    float decayFactor = 0.0f;
    std::complex<float> oscCoef = 0.0f;

    std::complex<float> y1 = 0.0f;

    float freq = 100.0f;
    float t60 = 1.0f;
    std::complex<float> A;
    juce::int64 maxSamples = 0;
    juce::int64 sampleCnt = 0;

    float fs = 44100.0f;
    const std::complex<float> jImag = std::complex<float> (0, 1);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModeOscillator)
};

#endif // MODEOSCILLATOR_H_INCLUDED
