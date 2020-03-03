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
    MidiKeyboardComponent keyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BespokeWaterbottleSynthAudioProcessorEditor)
};
