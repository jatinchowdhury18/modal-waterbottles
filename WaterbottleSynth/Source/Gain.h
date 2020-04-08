#ifndef GAIN_H_INCLUDED
#define GAIN_H_INCLUDED

#include "JuceHeader.h"

class Gain
{
public:
    Gain() {}
    ~Gain() {}

    void setGain (float newGain)
    { 
        curGain = newGain;
    }

    void prepare()
    {
        prevGain = curGain;
    }

    void processBlock (AudioBuffer<float>& buffer)
    {
        if (curGain == prevGain)
        {
            buffer.applyGain (curGain);
        }
        else
        {
            buffer.applyGainRamp (0, buffer.getNumSamples(), prevGain, curGain);
            prevGain = curGain;
        }
    }

private:
    float curGain = 1.0f;
    float prevGain = curGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain)
};

#endif //GAIN_H_INCLUDED
