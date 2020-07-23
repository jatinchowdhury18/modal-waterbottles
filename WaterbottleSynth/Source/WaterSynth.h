#ifndef WATERSYNTH_H_INCLUDED
#define WATERSYNTH_H_INCLUDED

#include "ModalVoice.h"

class WaterSynth : public Synthesiser
{
public:
    WaterSynth() {}

    static void addParameters (std::vector<std::unique_ptr<RangedAudioParameter>>& params)
    {
        params.push_back (std::make_unique<AudioParameterFloat> ("water", "Water", 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterFloat> ("swingdamp", "Swing Damp", 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterInt> ("swingmodes", "Swing Modes", 0, 10, 0));
        params.push_back (std::make_unique<AudioParameterInt> ("nummodes", "# Modes", 1, 40, 40));
    }

    void prepareToPlay (double fs, int samplesPerBlock)
    {
        setCurrentPlaybackSampleRate (fs);

        for (auto voice : voices)
        {
            if (auto voiceCast = dynamic_cast<ModalVoice*> (voice))
                voiceCast->prepareToPlay (fs, samplesPerBlock);
        }
    }

    void setStickers (float stickers)
    {
        for (auto* voice : voices)
        {
            if (auto* voiceCast = dynamic_cast<ModalVoice*> (voice))
                voiceCast->setStickers (stickers);
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterSynth)
};

#endif // WATERSYNTH_H_INCLUDED
