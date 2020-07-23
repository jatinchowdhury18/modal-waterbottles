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

    static void addParameters (std::vector<std::unique_ptr<RangedAudioParameter>>& params)
    {
        params.push_back (std::make_unique<AudioParameterFloat> ("water", "Water", 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterFloat> ("swingdamp", "Swing Damp", 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterInt> ("swingmodes", "Swing Modes", 0, 10, 0));
        params.push_back (std::make_unique<AudioParameterInt> ("nummodes", "# Modes", 1, 50, 50));
    }

    void prepareToPlay (double fs, int samplesPerBlock)
    {
        setCurrentPlaybackSampleRate (fs);

        for (auto voice : voices)
        {
            if (auto voiceCast = dynamic_cast<BModalVoice*> (voice))
                voiceCast->prepareToPlay (fs, samplesPerBlock);
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BWaterSynth)
};

#endif // BWATERSYNTH_H_INCLUDED
