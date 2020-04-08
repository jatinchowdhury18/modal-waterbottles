#include "BottomComponent.h"

BottomComponent::BottomComponent (WaterbottleSynthAudioProcessor& p) :
    keyboard (p.keyBoardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    startTimer (400);

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
}

void BottomComponent::resized()
{
    keyboard.setBounds (getLocalBounds());
}
