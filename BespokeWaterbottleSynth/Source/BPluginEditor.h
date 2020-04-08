/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "BPluginProcessor.h"

//==============================================================================
/**
*/
class BespokeWaterbottleSynthAudioProcessorEditor :
    public AudioProcessorEditor,
    private Timer
{
public:
    BespokeWaterbottleSynthAudioProcessorEditor (BespokeWaterbottleSynthAudioProcessor&);
    ~BespokeWaterbottleSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    BespokeWaterbottleSynthAudioProcessor& processor;

    TextButton reloadButton;

    Slider waterSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> waterAttach;

    ComboBox strikerBox;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> strikerAttach;

    Slider swingDampSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> swingDampAttach;

    Slider swingModesSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> swingModesAttach;

    Slider gainSlide;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttach;

    MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BespokeWaterbottleSynthAudioProcessorEditor)
};
