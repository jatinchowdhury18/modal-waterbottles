#include "BaseMode.h"

BaseMode::BaseMode (std::function<float(float)> freqLambda, std::function<float(float)> tauLambda, std::complex<float> amp, float stickerFactor) :
    freqLambda (freqLambda),
    tauLambda (tauLambda),
    freq (freqLambda (0.0f)),
    tau (tauLambda (0.0f)),
    amp (amp),
    stickerFactor (stickerFactor),
    fsMeasure (48000.0f)
{
}

BaseMode::BaseMode (float freq, float tau, std::complex<float> amp, float stickerFactor, float fsMeasure) :
    freqLambda ([freq] (float) { return freq; }),
    tauLambda ([tau] (float) { return tau; }),
    freq (freq),
    tau (tau),
    amp (amp),
    stickerFactor (stickerFactor),
    fsMeasure (fsMeasure)
{
}

void BaseMode::prepare (double sampleRate)
{
    fs = (float) sampleRate;
    y1 = 0.0f;

    calcCoefs();
}

void BaseMode::setFrequency (float newFreqMult)
{
    freqMult = newFreqMult;
    calcOscCoef();
}

void BaseMode::setParameters (float water, float stickers)
{
    freq = freqLambda (water);
    tau = tauLambda (water);

    stickerCoef = expf (stickers * stickerFactor);

    calcCoefs();
}

void BaseMode::triggerNote (float newFreqMult, float velocity, float newSwingDamp, float newSwingFreq)
{
    swingDamp = newSwingDamp;
    freqOff = velocity * freq / 10.0f;
    swingCoef = newSwingDamp * exp (jImag * MathConstants<float>::twoPi * newSwingFreq / fs);

    freqMult = newFreqMult;
    calcCoefs();

    if (freq * freqMult > fs / 2.0f) // no aliasing
        velocity = 0.0f;

    x = ampCoef * powf (velocity, 1.0f);
}

void BaseMode::calcCoefs()
{
    calcDecayCoef();
    calcOscCoef();
    ampCoef = 1000.0f*amp;
}

void BaseMode::calcOscCoef()
{
    oscCoef = exp (jImag * MathConstants<float>::twoPi * (freq + std::imag (freqOff)) * freqMult / fs);
    totCoef = oscCoef * decayCoef;
}

void BaseMode::calcDecayCoef()
{
    decayCoef = expf (-1.0f / (tau * fs / fsMeasure)) * stickerCoef;
}

// void BaseMode::processBlock (float* buffer, const int numSamples)
// {
//     for (int n = 0; n < numSamples; ++n)
//     {
//         auto y = ampCoef * buffer[n] + oscCoef * ampCoef * y1;
//         y1 = y;
//         buffer[n] = std::real (y);
//     }
// }
