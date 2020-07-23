#ifndef MODALVOICE_H_INCLUDED
#define MODALVOICE_H_INCLUDED

#include "Mode.h"

struct ModalSound : public SynthesiserSound
{
    ModalSound() {}

    bool appliesToNote    (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

class ModalVoice :  public SynthesiserVoice
{
public:
    ModalVoice (AudioProcessorValueTreeState& vts);

    bool canPlaySound (SynthesiserSound* sound) override;

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*pitchWheelPos*/) override;
    void stopNote (float velocity, bool allowTailOff) override;

    void setStickers (float stickers);

    void pitchWheelMoved (int) override;
    void controllerMoved (int, int) override {}

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

private:
    void updateParams();
    void updateModeFreqs();
    void updateDecays();
    bool isNotePlaying();
    void triggerSwing (float velocity);

    inline float freq0 (float x) const noexcept { return 70.911258f * (1.0f - (1.0f / (1.0f + exp (-8.3632f * (x - 0.5404f))))) + 147.7403f; }
    inline float decay0 (float x) const noexcept { return (float) -1.471e6*x*x*x*x + (float) 3.345e6*x*x*x - (float) 2.392e6*x*x + (float) 5.137e5*x + (float) 2.49e4; };
    inline float decay1 (float x) const noexcept { return (float) -1.471e6*x*x*x*x + (float) 3.345e6*x*x*x - (float) 2.392e6*x*x + (float) 5.137e5*x + (float) 2.49e4; };
    
    const float log1000 = std::log (1000.0f);
    inline float tau2t60 (float tau) const noexcept { return 1.0f / (std::log (std::exp (48000.0f / tau)) / log1000); }
    inline double maxFreq() const noexcept { return getSampleRate() * 0.45; }

    // parameters
    float stickersAmt = 0.0f;
    float* waterParam = nullptr;
    float* swingDampParam = nullptr;
    float* swingModesParam = nullptr;
    float* numModesParam = nullptr;

    float fundamental = 0.0f;
    int midiNote = 0;

    static const int numModes = 40;
    Mode modes[numModes];
    AudioBuffer<float> renderBuffer;

    static const int maxSwingModes = 10;
    Mode swingModes[maxSwingModes];
    AudioBuffer<float> swingBuffer;

    const float bottleHeight = 0.239f; // meters
    const float swingFreq = 1.0f / (MathConstants<float>::twoPi * sqrtf(bottleHeight / 9.8f));

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModalVoice)
};

#endif // MODALVOICE_H_INCLUDED
