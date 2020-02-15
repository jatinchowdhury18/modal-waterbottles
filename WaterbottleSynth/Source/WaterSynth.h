#ifndef WATERSYNTH_H_INCLUDED
#define WATERSYNTH_H_INCLUDED

#include "ModalVoice.h"

class WaterSynth : public Synthesiser
{
public:
    WaterSynth() {}

    void setWaterLevel (float water)
    {
        waterLevel = jlimit (0.0f, 1.0f, water);
    }

    void noteOn (const int midiChannel, const int midiNoteNumber, const float velocity) override
    {
        for (auto* voice : voices)
        {
            if (auto* voiceCast = dynamic_cast<ModalVoice*> (voice))
                voiceCast->setWaterLevel (waterLevel);
        }

        Synthesiser::noteOn (midiChannel, midiNoteNumber, velocity);
    }

private:
    float waterLevel = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterSynth)
};

#endif // WATERSYNTH_H_INCLUDED
