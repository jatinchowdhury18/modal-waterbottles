#ifndef MODE_H_INCLUDED
#define MODE_H_INCLUDED

#include "ModeOscillator.h"

class Mode
{
public:
    Mode();

    // @TODO: decay time is currently the only parameter that will not modulate smoothly
    inline void setDecayTime (float t60, float damping = 1.0f) noexcept { osc.setDecay (t60, damping); }
    inline void setModeFrequency (float freq) noexcept { oscFreq = freq; osc.setFreq (freq); };
    inline void setModeAmp (std::complex<float> amp) noexcept { osc.setAmp (amp); }

    float getFreq() const noexcept { return oscFreq; }

    void startNote (float velocity = 1.0f) { osc.start (velocity); }
    inline void reset() noexcept { osc.reset(); }
    inline bool isPlaying() const noexcept { return osc.isPlaying(); }

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void process (AudioBuffer<float>& buffer, const int numSamples);
    void processVibrato (AudioBuffer<float>& buffer, const AudioBuffer<float>& freqBuffer, const int numSamples);

private:
    ModeOscillator osc;

    AudioBuffer<float> renderBuffer;

    float fs = 44100.0f;
    float oscFreq = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mode)
};

#endif // MODE_H_INCLUDED
