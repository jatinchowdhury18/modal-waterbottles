#ifndef BMODALVOICE_H_INCLUDED
#define BMODALVOICE_H_INCLUDED

#include "Mode.h"

struct BModalSound : public SynthesiserSound
{
    BModalSound() {}

    bool appliesToNote    (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

class BModalVoice : public SynthesiserVoice
{
public:
    BModalVoice (AudioProcessorValueTreeState& vts);

    bool canPlaySound (SynthesiserSound* sound) override;

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*pitchWheelPos*/) override;
    void stopNote (float velocity, bool allowTailOff) override;

    void pitchWheelMoved (int) override;
    void controllerMoved (int, int) override {}

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

    void reload (File& bottleFile);

private:
    void updateModeFreqs();
    bool isNotePlaying();
    void triggerSwing (float velocity);

    const float log1000 = std::log (1000.0f);
    inline float tau2t60 (float tau) const noexcept { return 1.0f / (std::log (std::exp (48000.0f / tau)) / log1000); }
    inline double maxFreq() const noexcept { return currentSampleRate * 0.45; }
    inline int curNumModes() const noexcept { return jmin (curMaxModes, (int) *numModesParam); }

    // parameters
    float* waterParam = nullptr;
    float* swingDampParam = nullptr;
    float* swingModesParam = nullptr;
    float* numModesParam = nullptr;

    float fundamental = 0.0f;
    int midiNote = 0;

    static const int maxNumModes = 50;
    int curMaxModes = 0;
    Mode modes[maxNumModes];
    AudioBuffer<float> renderBuffer;

    static const int maxSwingModes = 10;
    Mode swingModes[maxSwingModes];
    AudioBuffer<float> swingBuffer;
    float swingFreq = 0.0f;

    float FREQS[maxNumModes];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BModalVoice)
};

#endif // BMODALVOICE_H_INCLUDED
