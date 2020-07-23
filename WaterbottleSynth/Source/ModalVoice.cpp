#include "ModalVoice.h"

namespace
{
    constexpr float FREQS[] = { 0.0f, 735.0f, 927.3f, 977.0f, 1371.7f, 
                             1739.3f, 1782.3f, 1928.7f, 2151.0f, 2182.7f,
                             2206.0f, 2591.3f, 2733.3f, 3359.3f, 3461.7f,
                             3678.0f, 3751.0f, 3977.0f, 4192.0f, 4278.7f,
                             4601.0f, 4796.0f, 5216.0f, 5319.0f, 5653.3f,
                             5962.3f, 6056.7f, 6955.3f, 7344.7f, 7429.0f,
                             7599.0f, 7761.3f, 7945.0f, 8384.3f,  8522.7f,
                             8772.0f, 9128.3f, 9833.7f, 10376.f, 11209.f, };

    constexpr float TAUS[] = { 0.0f, 0.0f, 21474.8f, 21694.8f, 14806.6f, 13126.2f,
                           13010.7f, 12464.7f, 11472.7f, 11345.0f, 11250.4f, 8989.90f,
                           8293.51f, 6026.65f, 5731.55f, 5402.86f, 5340.44f, 5162.93f,
                           5026.59f, 4957.85f, 4804.45f, 4752.26f, 4595.37f, 4567.85f,
                           4616.46f, 4422.07f, 4375.13f, 3496.75f, 3281.70f, 3239.40f,
                           3170.67f, 3117.73f, 3065.84f, 2912.81f, 2880.41f, 2808.99f,
                           2726.79f, 2531.38f, 2357.52f, 2091.17f, };

    constexpr float STICKER_FACTORS[] = { -0.0001141f, -0.0001210f, -0.0001877f, -0.0001502f, -0.0001466f,
                                          -0.0001050f, -0.0004610f, -0.0004138f, -0.0004081f, -0.0003864f,
                                          -0.0003577f, -0.0003365f, -0.0003537f, -0.0003846f, -0.0004152f,
                                          -0.0007438f, -0.0010839f, -0.0013383f, -0.0014889f, -0.0015063f,
                                          -0.0015118f, -0.0015224f, -0.0015332f, -0.0015526f, -0.0015753f,
                                          -0.0015711f, -0.0016101f, -0.0016789f, -0.0017116f, -0.0017111f,
                                          -0.0017095f, -0.0016950f, -0.0016814f, -0.0016423f, -0.0016122f,
                                          -0.0007624f, -0.0008890f, -0.0008736f, -0.0008004f, -0.0008868f, };
    
    constexpr std::complex<float> AMPS[] = { {1.6375e-4, 8.2776e-4}, {2.4356e-3, -1.492e-3}, {1.4482e-3, 1.9481e-3}, {5.4375e-4, 2.6515e-3},
                                             {-9.101e-4, -1.718e-3}, {3.3461e-3, 2.8032e-3}, {9.2404e-4, 1.6469e-3}, {-9.274e-4, 9.0368e-4},
                                             {-1.429e-3, -2.004e-3}, {-6.974e-4, -9.022e-4}, {-8.597e-4, -1.253e-3}, {3.2111e-4, 1.0081e-3},
                                             {-1.467e-3, 1.4958e-3}, {1.1023e-3, 1.1865e-3}, {-3.712e-4, 3.3302e-3}, {-1.659e-3, 1.7872e-4},
                                             {-8.134e-4, -2.297e-4}, {9.9300e-4, -1.626e-3}, {1.2352e-3, 8.9491e-4}, {1.0866e-3, 3.1236e-3},
                                             {-1.961e-3, -1.558e-3}, {2.4649e-3, -2.387e-3}, {-1.754e-3, 7.7755e-4}, {-3.785e-3, 2.7729e-4},
                                             {-1.370e-3, 6.7944e-4}, {-2.485e-4, 1.2892e-3}, {3.7174e-5, 1.3199e-3}, {8.5018e-4, 1.0479e-3},
                                             {-3.269e-3, 8.0851e-4}, {1.0449e-3, 8.6837e-4}, {-1.951e-4, 3.7306e-3}, {9.5586e-4, 3.3230e-5},
                                             {-1.841e-3, -1.429e-3}, {4.1932e-4, 2.4528e-3}, {2.4407e-3, -2.483e-3}, {-1.047e-3, 5.7032e-4},
                                             {-1.937e-3, 1.3581e-3}, {1.7622e-4, 1.6890e-3}, {1.9720e-3, 1.2535e-4}, {2.3114e-3, 2.8051e-5}, };
}


ModalVoice::ModalVoice (AudioProcessorValueTreeState& vts)
{
    waterParam = vts.getRawParameterValue ("water");
    swingDampParam = vts.getRawParameterValue ("swingdamp");
    swingModesParam = vts.getRawParameterValue ("swingmodes");
    numModesParam = vts.getRawParameterValue ("nummodes");

    for (int i = 0; i < numModes; ++i)
        modes[i].setModeAmp (AMPS[i] / std::abs (AMPS[0]));
}

bool ModalVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<ModalSound*> (sound) != nullptr;
}

void ModalVoice::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (auto& mode : modes)
        mode.prepareToPlay (sampleRate, samplesPerBlock);

    for (auto& mode : swingModes)
        mode.prepareToPlay (sampleRate, samplesPerBlock);

    renderBuffer.setSize (2, samplesPerBlock);
    swingBuffer.setSize (1, samplesPerBlock);
}

void ModalVoice::updateParams()
{
    updateModeFreqs();
    updateDecays();
}

void ModalVoice::setStickers (float stickers)
{
    if (stickers == stickersAmt)
        return;

    stickersAmt = stickers;
}

void ModalVoice::updateModeFreqs()
{
    const auto baseFreq = freq0 (*waterParam);

    for (int i = 0; i < (int) *numModesParam; i++)
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

void ModalVoice::updateDecays()
{

    for (int i = 0; i < (int) *numModesParam; i++)
    {
        auto tau = TAUS[i];
        if (i == 0)
            tau = decay0 (*waterParam);
        else if (i == 1)
            tau = decay1 (*waterParam);

        modes[i].setDecayTime (tau2t60 (tau), std::exp (stickersAmt * STICKER_FACTORS[i]));
    }
}

void ModalVoice::triggerSwing (float velocity)
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

void ModalVoice::pitchWheelMoved (int pitchValue)
{
    auto cents = 200.0f * float (pitchValue - 8192) / 8192.0f;
    fundamental = (float) MidiMessage::getMidiNoteInHertz (midiNote) * pow (2.0f, cents / 1200.0f);
}

void ModalVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    midiNote = midiNoteNumber;
    fundamental = (float) MidiMessage::getMidiNoteInHertz (midiNote);

    updateModeFreqs();
    for (int i = 0; i < (int) *numModesParam; ++i)
    {
        modes[i].reset();
        modes[i].startNote (velocity);
    }

    triggerSwing (velocity);
}

void ModalVoice::stopNote (float /*velocity*/, bool  allowTailOff)
{
    if (! allowTailOff)
        clearCurrentNote();
}

bool ModalVoice::isNotePlaying()
{
    bool result = false;
    for (int i = 0; i < (int) *numModesParam; ++i)
        result = modes[i].isPlaying() || result;

    return result;
}

void ModalVoice::renderNextBlock (AudioSampleBuffer& buffer, int startSample, int numSamples)
{
    updateParams();
    if (! isNotePlaying())
        return;

    renderBuffer.clear();

    int modeIdx = 0;
    int modesToSwing = (int) jmin (*swingModesParam, *numModesParam);
    for (; modeIdx < modesToSwing; ++modeIdx)
    {
        swingBuffer.clear();
        swingModes[modeIdx].process (swingBuffer, numSamples);
        modes[modeIdx].processVibrato (renderBuffer, swingBuffer, numSamples);
    }

    for (; modeIdx < (int) *numModesParam; ++modeIdx)
        modes[modeIdx].process (renderBuffer, numSamples);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.addFrom (ch, startSample, renderBuffer, ch, 0, numSamples);

    if (! isNotePlaying())
        clearCurrentNote();
}
