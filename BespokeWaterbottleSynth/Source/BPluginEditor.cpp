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
    setSize (600, 200);

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

    auto setupSlider = [=, &p] (Slider& slide, std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>& attach,
        String paramID, std::function<void()> onClick = {})
    {
        addAndMakeVisible (slide);
        slide.setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        attach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.getVTS(), paramID, slide));
        slide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
        slide.setName (p.getVTS().getParameter (paramID)->name);
        slide.onValueChange = onClick;
    };

    setupSlider (waterSlide, waterAttach, "water");
    setupSlider (swingDampSlide, swingDampAttach, "swingdamp");
    setupSlider (swingModesSlide, swingModesAttach, "swingmodes");
    setupSlider (gainSlide, gainAttach, "gain");
    setupSlider (numSlide, numAttach, "nummodes");

    // Striker combobox
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

    g.setColour (Colours::white);
    auto makeName = [&g] (Component& comp)
    {
        g.drawFittedText (comp.getName(), comp.getX(), comp.getY() - 5, comp.getWidth(), 10, Justification::centred, 1);
    };

    makeName (waterSlide);
    makeName (swingDampSlide);
    makeName (swingModesSlide);
    makeName (gainSlide);
    makeName (numSlide);
}

void BespokeWaterbottleSynthAudioProcessorEditor::resized()
{
    reloadButton.setBounds (10, 140, 80, 20);
    strikerBox.setBounds   (10,  40, 90, 20);
    waterSlide.setBounds       (110, 10, 90, 85);
    swingDampSlide.setBounds   (210, 10, 90, 85);
    swingModesSlide.setBounds  (310, 10, 90, 85);
    gainSlide.setBounds        (410, 10, 90, 85);
    numSlide.setBounds         (510, 10, 90, 85);

    keyboard.setBounds (getBounds().removeFromRight (500).removeFromBottom (100));
}
