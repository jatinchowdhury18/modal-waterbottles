#ifndef BMODALVOICE_H_INCLUDED
#define BMODALVOICE_H_INCLUDED

#include "BaseMode.h"

struct BModalSound : public SynthesiserSound
{
    BModalSound() {}

    bool appliesToNote    (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

class BModalVoice : public SynthesiserVoice
{
public:
    BModalVoice();

    bool canPlaySound (SynthesiserSound* sound) override;

    void setCurrentPlaybackSampleRate (double sampleRate) override;
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*pitchWheelPos*/) override;
    void stopNote (float /*velocity*/, bool allowTailOff) override;

    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    void setWater (float water);

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

    void reload (File& bottleFile);

private:

    OwnedArray<BaseMode> modes[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BModalVoice)
};

#endif // BMODALVOICE_H_INCLUDED
