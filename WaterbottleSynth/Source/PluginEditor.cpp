#include "PluginEditor.h"
#include "BottleComponent.h"

WaterbottleSynthAudioProcessorEditor::WaterbottleSynthAudioProcessorEditor (WaterbottleSynthAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p),
    keyboard (p.keyBoardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    startTimer (400);
    
    bottle = std::make_unique<BottleComponent>();
    addAndMakeVisible (dynamic_cast<BottleComponent*> (bottle.get()));
    addListener (bottle.get());

    addAndMakeVisible (waterSlide);
    waterSlide.setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    waterAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.vts, "water", waterSlide));
    waterSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
    waterSlide.onValueChange = [=] { listeners.call (&WaterbottleSynthAudioProcessorEditor::Listener::waterAmountChanged, (float) waterSlide.getValue()); };
    listeners.call (&WaterbottleSynthAudioProcessorEditor::Listener::waterAmountChanged, (float) waterSlide.getValue());

    addAndMakeVisible (keyboard);

    setSize (500, 500);
}

WaterbottleSynthAudioProcessorEditor::~WaterbottleSynthAudioProcessorEditor()
{
    removeListener (bottle.get());
}

void WaterbottleSynthAudioProcessorEditor::timerCallback()
{
    keyboard.grabKeyboardFocus();
    stopTimer();
}

void WaterbottleSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.drawFittedText ("Water", 0, 403, 100, 10, Justification::centred, 1);
}

void WaterbottleSynthAudioProcessorEditor::resized()
{
    dynamic_cast<BottleComponent*> (bottle.get())->setBounds (0, 0, 500, 400);
    waterSlide.setBounds (10, 410, 80, 90);
    keyboard.setBounds (getLocalBounds().removeFromRight (400).removeFromBottom (100));
}
