/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "BPluginProcessor.h"
#include "BPluginEditor.h"

//==============================================================================
BespokeWaterbottleSynthAudioProcessorEditor::BespokeWaterbottleSynthAudioProcessorEditor (BespokeWaterbottleSynthAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p),
    keyboard (p.keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    setSize (500, 100);

    startTimer (400);

    addAndMakeVisible (reloadButton);
    reloadButton.setButtonText ("Reload");
    reloadButton.onClick = [&p]
    {
        FileChooser fileChooser (String ("Open waterbottle file"), File(), "*.waterbottle");

        if (fileChooser.browseForFileToOpen())
        {
            auto file = fileChooser.getResult();
            p.reloadSynth (file);
        }
    };

    addAndMakeVisible (keyboard);
    keyboard.setKeyPressBaseOctave (3);
    keyboard.setLowestVisibleKey (24);
}

BespokeWaterbottleSynthAudioProcessorEditor::~BespokeWaterbottleSynthAudioProcessorEditor()
{
}

void BespokeWaterbottleSynthAudioProcessorEditor::timerCallback()
{
    keyboard.grabKeyboardFocus();
    stopTimer();
}

//==============================================================================
void BespokeWaterbottleSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void BespokeWaterbottleSynthAudioProcessorEditor::resized()
{
    reloadButton.setBounds (10, 40, 80, 20);

    keyboard.setBounds (getBounds().removeFromRight (400));
}
