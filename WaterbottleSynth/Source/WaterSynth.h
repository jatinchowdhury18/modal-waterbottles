#ifndef WATERSYNTH_H_INCLUDED
#define WATERSYNTH_H_INCLUDED

#include "ModalVoice.h"

class WaterSynth : public Synthesiser
{
public:
    WaterSynth() {}

    void setParameters (float water, float stickers)
    {
        if (water == waterLevel && stickers == stickersAmt) // no need to update
            return;

        waterLevel = jlimit (0.0f, 1.0f, water);
        stickersAmt = jlimit (0.0f, 1.0f, stickers);

        for (auto* voice : voices)
        {
            if (auto* voiceCast = dynamic_cast<ModalVoice*> (voice))
                voiceCast->setParameters (waterLevel, stickersAmt);
        }
    }

private:
    float waterLevel = 0.0f;
    float stickersAmt = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterSynth)
};

#endif // WATERSYNTH_H_INCLUDED
