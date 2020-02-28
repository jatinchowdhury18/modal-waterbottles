#include "BottomComponent.h"

BottomComponent::BottomComponent (WaterbottleSynthAudioProcessor& p) :
    keyboard (p.keyBoardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    startTimer (400);

    addAndMakeVisible (waterSlide);
    waterSlide.setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    waterAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.vts, "water", waterSlide));
    waterSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
    waterSlide.onValueChange = [=] { updateWaterAmount(); };
    

    addAndMakeVisible (keyboard);
    keyboard.setLowestVisibleKey (24);
    keyboard.setKeyPressBaseOctave (3);
}

void BottomComponent::timerCallback()
{
    keyboard.grabKeyboardFocus();
    stopTimer();
}

void BottomComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.drawFittedText ("Water", 0, 3, 100, 10, Justification::centred, 1);
}

void BottomComponent::resized()
{
    waterSlide.setBounds (10, 10, 80, 90);
    keyboard.setBounds (getLocalBounds().removeFromRight (getWidth() - 100));
}
