#ifndef BASEMODE_H_INCLUDED
#define BASEMODE_H_INCLUDED

#include "JuceHeader.h"
#include <complex>

class BaseMode
{
public:
    BaseMode (std::function<float(float)> freqLambda, std::function<float(float)> tauLambda, std::complex<float> amp);
    BaseMode (float freq, float tau, std::complex<float> amp);

    void prepare (double sampleRate);
    void calcCoefs();
    void triggerNote (float newFreqMult);
    void setWaterLevel (float water);
    void setStickerAmt (float stickers);
    float getBaseFreq() const noexcept { return freq; }

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

    std::function<float(float)> freqLambda;
    std::function<float(float)> tauLambda;

    float freq = 150.0f;
    float tau = 28340.8f;
    std::complex<float> amp = std::complex<float> (1.6375e-4, 8.2776e-4);
    float freqMult = 1.0f;

    float fs = 44100.0f;
    const std::complex<float> jImag = std::complex<float> (0, 1);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseMode)
};

#endif // BASEMODE_H_INCLUDED

