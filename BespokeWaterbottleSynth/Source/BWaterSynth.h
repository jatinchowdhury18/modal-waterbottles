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

        Synthesiser::renderVoices (buffer, startSample, numSamples);
    }

private:
    std::atomic_bool reloading;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BWaterSynth)
};

#endif // BWATERSYNTH_H_INCLUDED
