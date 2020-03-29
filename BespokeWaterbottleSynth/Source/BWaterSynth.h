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

    void setWater (float water)
    {
        if (water == waterLevel) // no updating needed
            return;

        waterLevel = water;

        for (auto* voice : voices)
        {
            if (auto* voiceCast = dynamic_cast<BModalVoice*> (voice))
                voiceCast->setWater (waterLevel);
        }
    }

private:
    float waterLevel = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BWaterSynth)
};

#endif // BWATERSYNTH_H_INCLUDED
