#ifndef BWATERSYNTH_H_INCLUDED
#define BWATERSYNTH_H_INCLUDED

#include "BModalVoice.h"

class BWaterSynth : public Synthesiser
{
public:
    BWaterSynth() {}

    void reload (File& bottleFile)
    {
        const ScopedLock sl (lock);

        for (auto* voice : voices)
            if (auto* voiceCast = dynamic_cast<BModalVoice*> (voice))
                voiceCast->reload (bottleFile);
    }

    void setParameters (float water, float newSwingDamp, int newSwingModes)
    {
        if (water == waterLevel && swingDamp == newSwingDamp
         && swingModes == newSwingModes) // no updating needed
            return;

        waterLevel = water;
        swingDamp = newSwingDamp;
        swingModes = newSwingModes;

        for (auto* voice : voices)
        {
            if (auto* voiceCast = dynamic_cast<BModalVoice*> (voice))
                voiceCast->setParameters (waterLevel, swingDamp, swingModes);
        }
    }

private:
    float waterLevel = 0.0f;
    float swingDamp = 0.0f;
    int swingModes = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BWaterSynth)
};

#endif // BWATERSYNTH_H_INCLUDED
