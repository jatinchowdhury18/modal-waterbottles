#include "BModalVoice.h"

BModalVoice::BModalVoice()
{
    for (int ch = 0; ch < 2; ++ch)
    {
        for (int i = 0; i < 4; ++i)
            modes[ch].add (std::make_unique<BaseMode> (100.0f*(i+1), 20000.0f, std::complex<float> (1.4482e-4, 1.9481e-4)));
    }
}

bool BModalVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<BModalSound*> (sound) != nullptr;
}

void BModalVoice::reload (File& bottleFile)
{
    StringArray lines;
    bottleFile.readLines (lines);

    // read metadata from first line
    StringArray line0;
    line0.addTokens (lines[0], ",", "");

    const double fsMeasure = line0[0].getDoubleValue();
    const int numModes = (int) line0[1].getDoubleValue();

    // clear existing modes
    for (int ch = 0; ch < 2; ++ch)
        modes[ch].clear();

    for (int i = 0; i < numModes; ++i)
    {
        StringArray line;
        line.addTokens (lines[i+1], ",", "");

        float freq = (float) line[0].getDoubleValue();
        float tau  = (float) line[1].getDoubleValue();
        float real = (float) line[2].getDoubleValue();
        float imag = (float) line[3].getDoubleValue();

        if (i == 1) // use freq of second mode to calc waterbottle length
        {
            const float bottleHeight = 343.0f / (2 * freq); // meters
            swingFreq = 1.0f / (MathConstants<float>::twoPi * sqrtf(bottleHeight / 9.8f));
        }

        for (int ch = 0; ch < 2; ++ch)
            modes[ch].add (std::make_unique<BaseMode> (freq, tau, std::complex<float> (real, imag), 0.0f, fsMeasure));
    }
}

void BModalVoice::setCurrentPlaybackSampleRate (double sampleRate)
{
    SynthesiserVoice::setCurrentPlaybackSampleRate (sampleRate);

    for (int ch = 0; ch < 2; ++ch)
    {
        for (auto* m : modes[ch])
            m->prepare (sampleRate);
    }
}

void BModalVoice::setWater (float water)
{
    if (modes[0].isEmpty())
        return;

    waterLevel = water;

    auto baseFreq = modes[0][0]->getBaseFreq();
    auto newBaseFreq = baseFreq * (1.0f + water);
    auto freqMult = newBaseFreq / modes[0][0]->getBaseFreq();

    for (int ch = 0; ch < 2; ++ch)
    {
        for (auto* m : modes[ch])
            m->setFrequency (freqMult);
    }
}

void BModalVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    if (modes[0].isEmpty())
        return;

    auto freq = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    auto freqMult = freq / modes[0][0]->getBaseFreq();

    auto swingDamp = 0.0f;
    if (waterLevel > 1.0f / 64.0f)
        swingDamp = 1.0f - powf (waterLevel, 7.0f);

    const int swingCutoff = jmin (modes[0].size(), 5);

    for (int ch = 0; ch < 2; ++ch)
    {
        for (int m = 0; m < swingCutoff; ++m)
            modes[ch][m]->triggerNote (freqMult, velocity, swingDamp, swingFreq);

        for (int m = swingCutoff; m < modes[0].size(); ++m)
            modes[ch][m]->triggerNote (freqMult, velocity);
    }
}

void BModalVoice::stopNote (float /*velocity*/, bool  allowTailOff)
{
    clearCurrentNote();
}

void BModalVoice::renderNextBlock (AudioSampleBuffer& buffer, int startSample, int numSamples)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            for (auto* m : modes[ch])
                x[n] += m->getNextSample();
        }
    }

    // buffer.applyGain (0.5f);
}
