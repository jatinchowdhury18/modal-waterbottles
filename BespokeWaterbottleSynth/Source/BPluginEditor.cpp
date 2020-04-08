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
    setSize (560, 200);

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

    // Water amount slider
    addAndMakeVisible (waterSlide);
    waterSlide.setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    waterAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.getVTS(), "water", waterSlide));
    waterSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);

    // Striker combobox
    addAndMakeVisible (strikerBox);
    strikerBox.addItemList (StrikerFilter::getChoices(), 1);
    strikerAttach.reset (new AudioProcessorValueTreeState::ComboBoxAttachment (p.getVTS(), "striker", strikerBox));

    // Swing damping slider
    addAndMakeVisible (swingDampSlide);
    swingDampSlide.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    swingDampSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
    swingDampAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.getVTS(), "swingdamp", swingDampSlide));

    // Swing modes slider
    addAndMakeVisible (swingModesSlide);
    swingModesSlide.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    swingModesSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
    swingModesAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.getVTS(), "swingmodes", swingModesSlide));

    // Gain slider
    addAndMakeVisible (gainSlide);
    gainSlide.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    gainSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
    gainAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.getVTS(), "gain", gainSlide));

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

    g.setColour (Colours::white);
    g.drawFittedText ("Water",       110, 3, 90, 10, Justification::centred, 1);
    g.drawFittedText ("Swing Damp",  210, 3, 90, 10, Justification::centred, 1);
    g.drawFittedText ("Swing Modes", 310, 3, 90, 10, Justification::centred, 1);
    g.drawFittedText ("Gain [dB]",   410, 3, 90, 10, Justification::centred, 1);
}

void BespokeWaterbottleSynthAudioProcessorEditor::resized()
{
    reloadButton.setBounds (10, 140, 80, 20);
    strikerBox.setBounds   (10,  40, 90, 20);
    waterSlide.setBounds       (110,  10, 90, 85);
    swingDampSlide.setBounds   (210,  10, 90, 85);
    swingModesSlide.setBounds  (310,  10, 90, 85);
    gainSlide.setBounds        (410,  10, 90, 85);

    keyboard.setBounds (getBounds().removeFromRight (460).removeFromBottom (100));
}
