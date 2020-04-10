#ifndef MODALVOICE_H_INCLUDED
#define MODALVOICE_H_INCLUDED

#include "BaseMode.h"

struct ModalSound : public SynthesiserSound
{
    ModalSound() {}

    bool appliesToNote    (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

class ModalVoice :  public SynthesiserVoice
{
public:
    ModalVoice();

    bool canPlaySound (SynthesiserSound* sound) override;

    void setCurrentPlaybackSampleRate (double sampleRate) override;
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*pitchWheelPos*/) override;
    void stopNote (float /*velocity*/, bool allowTailOff) override;

    void setParameters (float water, float stickers, float newSwingDamp, int newSwingModes);

    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

private:
    float waterLevel = 0.0f;
    float stickersAmt = 0.0f;
    float swingDamp = 0.0f;
    float swingDampFactor = 0.0f;
    int swingModes = 0;

    const static int numModes = 40;
    std::unique_ptr<BaseMode> mode[numModes][2];

    const float bottleHeight = 0.239f; // meters
    const float swingFreq = 1.0f / (MathConstants<float>::twoPi * sqrtf(bottleHeight / 9.8f));

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModalVoice)
};

#endif // MODALVOICE_H_INCLUDED
