#include "BModalVoice.h"

BModalVoice::BModalVoice (AudioProcessorValueTreeState& vts)
{
    waterParam = vts.getRawParameterValue ("water");
    swingDampParam = vts.getRawParameterValue ("swingdamp");
    swingModesParam = vts.getRawParameterValue ("swingmodes");
    numModesParam = vts.getRawParameterValue ("nummodes");

    // initial synthesized sound
    curMaxModes = 4;
    const std::complex<float> ampComplex (1.4482e-4, 1.9481e-4);
    const float ampAbs = std::abs (ampComplex);
    for (int i = 0; i < curMaxModes; ++i)
    {
        modes[i].setModeAmp (std::pow (ampComplex, i+1) / ampAbs);
        modes[i].setDecayTime (1.0f * std::pow (0.9f, i));
        FREQS[i] = 100.0f * (i + 1);
    }

    swingFreq = 10.0f;
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
    curMaxModes = jmin (maxNumModes, (int) line0[1].getDoubleValue());
    std::complex<float> baseAmp;

    // clear existing modes    
    for (int i = 0; i < curMaxModes; ++i)
    {
        StringArray line;
        line.addTokens (lines[i+1], ",", "");
    
        float freq = (float) line[0].getDoubleValue();
        float tau  = (float) line[1].getDoubleValue();
        float real = (float) line[2].getDoubleValue();
        float imag = (float) line[3].getDoubleValue();
    
        if (i == 0)
            baseAmp = std::complex<float> (real, imag);

        if (i == 1) // use freq of second mode to calc waterbottle length
        {
            const float bottleHeight = 343.0f / (2 * freq); // meters
            swingFreq = 1.0f / (MathConstants<float>::twoPi * sqrtf(bottleHeight / 9.8f));
        }
    
        modes[i].setDecayTime (tau2t60 (tau));
        modes[i].setModeAmp (std::complex<float> (real, imag) / std::abs (baseAmp));
        FREQS[i] = freq;
    }
}

void BModalVoice::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (auto& mode : modes)
        mode.prepareToPlay (sampleRate, samplesPerBlock);

    for (auto& mode : swingModes)
        mode.prepareToPlay (sampleRate, samplesPerBlock);

    renderBuffer.setSize (2, samplesPerBlock);
    swingBuffer.setSize (1, samplesPerBlock);
}

void BModalVoice::updateModeFreqs()
{
    const auto baseFreq = FREQS[0] * (1.0f + *waterParam);

    for (int i = 0; i < curNumModes(); i++)
    {
        auto ratio = FREQS[i] / baseFreq;
        if (i == 0)
            ratio = 1.0f;

        float f = fundamental * ratio;
        if( f < maxFreq())
            modes[i].setModeFrequency (f);
        else
            modes[i].setModeFrequency (0.0f);
    }
}

void BModalVoice::triggerSwing (float velocity)
{
    float swingDamp = 0.0f;
    if (*waterParam > 1.0f / 64.0f)
        swingDamp = powf ((1.0f - *swingDampParam), (float) 1.0e-4) * (1.0f - powf (*waterParam, 7.0f));

    for (int i = 0; i < maxSwingModes; ++i)
    {
        swingModes[i].reset();
        swingModes[i].setModeFrequency (swingFreq);
        swingModes[i].setModeAmp (velocity * modes[i].getFreq() / 10.0f);
        swingModes[i].setDecayTime (100.0f, swingDamp);
        swingModes[i].startNote();
    }
}

void BModalVoice::pitchWheelMoved (int pitchValue)
{
    auto cents = 200.0f * float (pitchValue - 8192) / 8192.0f;
    fundamental = (float) MidiMessage::getMidiNoteInHertz (midiNote) * pow (2.0f, cents / 1200.0f);
}

void BModalVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    midiNote = midiNoteNumber;
    fundamental = (float) MidiMessage::getMidiNoteInHertz (midiNote);

    updateModeFreqs();
    for (int i = 0; i < curNumModes(); ++i)
    {
        modes[i].reset();
        modes[i].startNote (velocity);
    }

    triggerSwing (velocity);
}

void BModalVoice::stopNote (float /*velocity*/, bool  allowTailOff)
{
    if (! allowTailOff)
        clearCurrentNote();
}

bool BModalVoice::isNotePlaying()
{
    bool result = false;
    for (int i = 0; i < curNumModes(); ++i)
        result = modes[i].isPlaying() || result;

    return result;
}

void BModalVoice::renderNextBlock (AudioSampleBuffer& buffer, int startSample, int numSamples)
{
    updateModeFreqs();
    if (! isNotePlaying())
        return;

    renderBuffer.clear();

    int modeIdx = 0;
    int modesToSwing = jmin ((int) *swingModesParam, curNumModes());
    for (; modeIdx < modesToSwing; ++modeIdx)
    {
        swingBuffer.clear();
        swingModes[modeIdx].process (swingBuffer, numSamples);
        modes[modeIdx].processVibrato (renderBuffer, swingBuffer, numSamples);
    }

    for (; modeIdx < curNumModes(); ++modeIdx)
        modes[modeIdx].process (renderBuffer, numSamples);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.addFrom (ch, startSample, renderBuffer, ch, 0, numSamples);

    if (! isNotePlaying())
        clearCurrentNote();
}
