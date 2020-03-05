#ifndef STRIKERFILTER_H_INCLUDED
#define STRIKERFILTER_H_INCLUDED

#include "JuceHeader.h"

enum Striker
{
    Toe,
    Shoulder,
    Palm,
    OneKnuckle,
    Nail,
    Knuckles,
    Knee,
    Heel,
    Head,
    GongMallet,
    Elbow,
    DAFx,
    Brush,
    None
};

/////////////////////
class StrikerFilter
{
public:
    StrikerFilter();

    static String getNameForStriker (Striker s);
    static StringArray getChoices();

    void loadIRForStriker (Striker s, dsp::Convolution* filter);
    void setStriker (int striker) { currStriker = static_cast<Striker> (striker); }

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void processBlock (AudioBuffer<float>& buffer);

private:
    OwnedArray<dsp::Convolution> filters;

    Striker currStriker = Striker::Head;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StrikerFilter)
};

#endif // STRIKERFILTER_H_INCLUDED
