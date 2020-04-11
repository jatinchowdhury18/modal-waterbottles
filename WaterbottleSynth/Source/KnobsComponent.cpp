#include "KnobsComponent.h"

KnobsComponent::KnobsComponent (WaterbottleSynthAudioProcessor& p)
{
    auto setupSlider = [=, &p] (Slider& slide, std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>& attach,
        String paramID, std::function<void()> onClick = {})
    {
        addAndMakeVisible (slide);
        slide.setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        attach.reset (new AudioProcessorValueTreeState::SliderAttachment (p.vts, paramID, slide));
        slide.setTextBoxStyle (Slider::TextBoxBelow, false, 60, 15);
        slide.setName (p.vts.getParameter (paramID)->name);
        slide.onValueChange = onClick;
    };

    setupSlider (waterSlide, waterAttach, "water", [=] { updateWaterAmount(); });
    setupSlider (swingDampSlide, swingDampAttach, "swingdamp");
    setupSlider (swingModesSlide, swingModesAttach, "swingmodes");
    setupSlider (gainSlide, gainAttach, "gain");
    setupSlider (numSlide, numAttach, "nummodes");

    // Strikers combobox
    addAndMakeVisible (strikerBox);
    strikerBox.addItemList (StrikerFilter::getChoices(), 1);
    strikerAttach.reset (new AudioProcessorValueTreeState::ComboBoxAttachment (p.vts, "striker", strikerBox));
}

void KnobsComponent::paint (Graphics& g)
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

void KnobsComponent::resized()
{
    waterSlide.setBounds  (10, 10, 80, 90);
    strikerBox.setBounds  (5, 115, 90, 20);
    
    swingDampSlide.setBounds  (10, 160, 80, 90);
    swingModesSlide.setBounds (10, 275, 80, 90);

    gainSlide.setBounds (10, 390, 80, 90);
    numSlide.setBounds  (10, 505, 80, 90);
}
