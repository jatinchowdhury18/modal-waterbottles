#include "BaseMode.h"

BaseMode::BaseMode (std::function<float(float)> freqLambda, std::function<float(float)> tauLambda, std::complex<float> amp, float stickerFactor) :
    freqLambda (freqLambda),
    tauLambda (tauLambda),
    freq (freqLambda (0.0f)),
    tau (tauLambda (0.0f)),
    amp (amp),
    stickerFactor (stickerFactor)
{
}

BaseMode::BaseMode (float freq, float tau, std::complex<float> amp, float stickerFactor) :
    freqLambda ([freq] (float) { return freq; }),
    tauLambda ([tau] (float) { return tau; }),
    freq (freq),
    tau (tau),
    amp (amp),
    stickerFactor (stickerFactor)
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

void BaseMode::triggerNote (float newFreqMult, float velocity)
{
    freqMult = newFreqMult;
    calcCoefs();

    x = powf (velocity, 1.0f);
}

void BaseMode::calcCoefs()
{
    calcOscCoef();
    calcDecayCoef();
    ampCoef = 1000.0f*amp;
}

void BaseMode::calcOscCoef()
{
    oscCoef = exp (jImag * MathConstants<float>::twoPi * freq * freqMult / fs);
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
