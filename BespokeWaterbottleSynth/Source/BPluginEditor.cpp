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

    addAndMakeVisible (strikerBox);
    strikerBox.addItemList (StrikerFilter::getChoices(), 1);
    strikerAttach.reset (new AudioProcessorValueTreeState::ComboBoxAttachment (p.getVTS(), "striker", strikerBox));

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
    strikerBox.setBounds (100, 40, 90, 20);

    keyboard.setBounds (getBounds().removeFromRight (300));
}
