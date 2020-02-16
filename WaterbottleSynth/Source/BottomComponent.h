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

    struct Listener
    {
    public:
        virtual ~Listener() {}
        virtual void waterAmountChanged (float /*newAmt*/) {}
    };

    void addListener (Listener* l) { listeners.add (l); }
    void removeListener (Listener* l) { listeners.remove (l); }

private:
    void timerCallback() override;

    ListenerList<Listener> listeners;

    Slider waterSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> waterAttach;

    MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomComponent)
};

#endif //  BOTTOMCOMPONENT_H_INCLUDED
