#ifndef KNOBSCOMPONENT_H_INCLUDED
#define KNOBSCOMPONENT_H_INCLUDED

#include "PluginProcessor.h"

class KnobsComponent : public Component
{
public:
    KnobsComponent (WaterbottleSynthAudioProcessor& p);
    ~KnobsComponent() {}

    void paint (Graphics& g) override;
    void resized() override;

    struct Listener
    {
    public:
        virtual ~Listener() {}
        virtual void waterAmountChanged (float /*newAmt*/) {}
    };

    void updateWaterAmount() { listeners.call (&Listener::waterAmountChanged, (float) waterSlide.getValue()); }

    void addListener (Listener* l) { listeners.add (l); }
    void removeListener (Listener* l) { listeners.remove (l); }

private:
    ListenerList<Listener> listeners;

    Slider waterSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> waterAttach;

    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> strikerAttach;
    ComboBox strikerBox;

    Slider swingDampSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> swingDampAttach;

    Slider swingModesSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> swingModesAttach;

    Slider gainSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttach;

    Slider numSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> numAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobsComponent)
};

#endif // KNOBSCOMPONENT_H_INCLUDED
