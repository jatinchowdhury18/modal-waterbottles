#ifndef BOTTOMCOMPONENT_H_INCLUDED
#define BOTTOMCOMPONENT_H_INCLUDED

#include "PluginProcessor.h"

class BottomComponent : public Component,
                        private Timer
{
public:
    BottomComponent (WaterbottleSynthAudioProcessor& p);
    ~BottomComponent() {}

    void paint (Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;

    MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomComponent)
};

#endif //  BOTTOMCOMPONENT_H_INCLUDED
