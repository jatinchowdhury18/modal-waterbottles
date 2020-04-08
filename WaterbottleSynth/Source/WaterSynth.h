#ifndef WATERSYNTH_H_INCLUDED
#define WATERSYNTH_H_INCLUDED

#include "ModalVoice.h"

class WaterSynth : public Synthesiser
{
public:
    WaterSynth() {}

    void setParameters (float water, float stickers, float newSwingDamp, int newSwingModes)
    {
        if (water == waterLevel && stickers == stickersAmt
         && swingDamp == newSwingDamp && swingModes == newSwingModes) // no need to update
            return;

        waterLevel = jlimit (0.0f, 1.0f, water);
        stickersAmt = jlimit (0.0f, 1.0f, stickers);
        swingDamp = newSwingDamp;
        swingModes = newSwingModes;

        for (auto* voice : voices)
        {
            if (auto* voiceCast = dynamic_cast<ModalVoice*> (voice))
                voiceCast->setParameters (waterLevel, stickersAmt, swingDamp, swingModes);
        }
    }

private:
    float waterLevel = 0.0f;
    float stickersAmt = 0.0f;
    float swingDamp = 0.0f;
    int swingModes = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterSynth)
};

#endif // WATERSYNTH_H_INCLUDED
