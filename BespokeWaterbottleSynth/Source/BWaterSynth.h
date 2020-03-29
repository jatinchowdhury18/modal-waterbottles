#ifndef BWATERSYNTH_H_INCLUDED
#define BWATERSYNTH_H_INCLUDED

#include "BModalVoice.h"

class BWaterSynth : public Synthesiser
{
public:
    BWaterSynth()
    {
        reloading = false;
    }

    void reload (File& bottleFile)
    {
        reloading = true;

        for (auto* voice : voices)
            if (auto* voiceCast = dynamic_cast<BModalVoice*> (voice))
                voiceCast->reload (bottleFile);

        reloading = false;
    }

    void renderVoices (AudioBuffer<float>& buffer, int startSample, int numSamples) override
    {
        if (reloading) // watching out for threads...
            return;

        MessageManagerLock mml;
        Synthesiser::renderVoices (buffer, startSample, numSamples);
    }

    void setWater (float water)
    {
        if (water == waterLevel || reloading) // no updating needed
            return;

        waterLevel = water;

        for (auto* voice : voices)
        {
            if (auto* voiceCast = dynamic_cast<BModalVoice*> (voice))
                voiceCast->setWater (waterLevel);
        }
    }

private:
    std::atomic_bool reloading;

    float waterLevel = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BWaterSynth)
};

#endif // BWATERSYNTH_H_INCLUDED
