#ifndef BASEMODE_H_INCLUDED
#define BASEMODE_H_INCLUDED

#include "JuceHeader.h"
#include <complex>

class BaseMode
{
public:
    BaseMode (std::function<float(float)> freqLambda, std::function<float(float)> tauLambda, std::complex<float> amp, float stickerFactor=0.0f);
    BaseMode (float freq, float tau, std::complex<float> amp, float stickerFactor=0.0f);

    void prepare (double sampleRate);
    void triggerNote (float newFreqMult, float velocity);
    void setParameters (float water, float stickers);

    void setFrequency (float newFreqMult);
    float getBaseFreq() const noexcept { return freq; }

    void calcCoefs();
    void calcOscCoef();
    void calcDecayCoef();

    inline float getNextSample() noexcept
    {
        auto y = ampCoef * x + oscCoef * decayCoef * y1;

        y1 = y;
        x = 0.0f;
        return std::imag (y);   
    }

private:
    std::complex<float> y1 = 0.0f;
    float x = 0.0f;

    std::complex<float> decayCoef = 0.0f;
    std::complex<float> oscCoef = 0.0f;
    std::complex<float> ampCoef = 0.0f;

    const float stickerFactor;
    float stickerCoef = 1.0f;

    std::function<float(float)> freqLambda;
    std::function<float(float)> tauLambda;

    float freq = 150.0f;
    float tau = 28340.8f;
    std::complex<float> amp = std::complex<float> (1.6375e-4, 8.2776e-4);
    float freqMult = 1.0f;

    float fs = 44100.0f;
    const std::complex<float> jImag = std::complex<float> (0, 1);

    const float fsMeasure = 48000.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseMode)
};

#endif // BASEMODE_H_INCLUDED

