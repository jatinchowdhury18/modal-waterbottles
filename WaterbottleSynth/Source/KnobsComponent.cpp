#include "KnobsComponent.h"

KnobsComponent::KnobsComponent (WaterbottleSynthAudioProcessor& p)
{
    // Water amount Slider
    addAndMakeVisible (waterSlide);
    waterSlide.setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    waterAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.vts, "water", waterSlide));
    waterSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
    waterSlide.onValueChange = [=] { updateWaterAmount(); };

    // Strikers combobox
    addAndMakeVisible (strikerBox);
    strikerBox.addItemList (StrikerFilter::getChoices(), 1);
    strikerAttach.reset (new AudioProcessorValueTreeState::ComboBoxAttachment (p.vts, "striker", strikerBox));

    // Swing damping slider
    addAndMakeVisible (swingDampSlide);
    swingDampSlide.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    swingDampSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
    swingDampAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.vts, "swingdamp", swingDampSlide));

    // Swing modes slider
    addAndMakeVisible (swingModesSlide);
    swingModesSlide.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    swingModesSlide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
    swingModesAttach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.vts, "swingmodes", swingModesSlide));
}

void KnobsComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.drawFittedText ("Water", 0, 5, 100, 10, Justification::centred, 1);
    g.drawFittedText ("Swing Damp", 0, 155, 100, 10, Justification::centred, 1);
    g.drawFittedText ("Swing Modes", 0, 305, 100, 10, Justification::centred, 1);
}

void KnobsComponent::resized()
{
    waterSlide.setBounds  (10, 10, 80, 90);
    strikerBox.setBounds  (5, 115, 90, 20);
    
    swingDampSlide.setBounds  (10, 160, 80, 90);
    swingModesSlide.setBounds (10, 310, 80, 90);
}
