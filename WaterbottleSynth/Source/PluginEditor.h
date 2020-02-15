#pragma once

#include "PluginProcessor.h"
// #include "BottleComponent.h"

class WaterbottleSynthAudioProcessorEditor  : public AudioProcessorEditor,
                                              private Timer
{
public:
    WaterbottleSynthAudioProcessorEditor (WaterbottleSynthAudioProcessor&);
    ~WaterbottleSynthAudioProcessorEditor();

    void paint (Graphics&) override;
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

    WaterbottleSynthAudioProcessor& processor;
    ListenerList<Listener> listeners;

    std::unique_ptr<Listener> bottle;
    // BottleComponent bottle;

    Slider waterSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> waterAttach;

    MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterbottleSynthAudioProcessorEditor)
};
