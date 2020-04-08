#pragma once

#include "PluginProcessor.h"
#include "BottleComponent.h"
#include "BottomComponent.h"
#include "KnobsComponent.h"

class WaterbottleSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    WaterbottleSynthAudioProcessorEditor (WaterbottleSynthAudioProcessor&);
    ~WaterbottleSynthAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    WaterbottleSynthAudioProcessor& processor;

    BottleComponent bottle;
    KnobsComponent knobs;
    BottomComponent bottom;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterbottleSynthAudioProcessorEditor)
};
