#include "BaseMode.h"

BaseMode::BaseMode (std::function<float(float)> freqLambda, std::function<float(float)> tauLambda, std::complex<float> amp) :
    freqLambda (freqLambda),
    tauLambda (tauLambda),
    freq (freqLambda (0.0f)),
    tau (tauLambda (0.0f)),
    amp (amp)
{
}

BaseMode::BaseMode (float freq, float tau, std::complex<float> amp) :
    freqLambda ([freq] (float) { return freq; }),
    tauLambda ([tau] (float) { return tau; }),
    freq (freq),
    tau (tau),
    amp (amp)
{
}

void BaseMode::prepare (double sampleRate)
{
    fs = (float) sampleRate;
    y1 = 0.0f;

    calcCoefs();
}

void BaseMode::setParameters (float water, float stickers)
{
    freq = freqLambda (water);
    tau = tauLambda (water);

    auto normalTau = (float) (1.864e4 * 1.0f + 4.3896e3);
    auto stickerTau = (float) (1.864e4 *  exp(-4.333 * stickers) + 4.3896e3);

    tau = -1.0f / ((1.0f / normalTau) - (1.0f / tau) - (1.0f / stickerTau));

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
    decayCoef = expf (-1.0f / tau);
    oscCoef = exp (jImag * MathConstants<float>::twoPi * freq * freqMult / fs);
    ampCoef = 1000.0f*amp;
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
