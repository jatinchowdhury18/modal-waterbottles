#include "PluginEditor.h"
#include "BottleComponent.h"

WaterbottleSynthAudioProcessorEditor::WaterbottleSynthAudioProcessorEditor (WaterbottleSynthAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p),
    bottle (p),
    knobs (p),
    bottom (p)
{
    addAndMakeVisible (bottle);
    addAndMakeVisible (knobs);
    addAndMakeVisible (bottom);

    knobs.addListener (&bottle);
    knobs.updateWaterAmount();

    setSize (700, 700);
}

WaterbottleSynthAudioProcessorEditor::~WaterbottleSynthAudioProcessorEditor()
{
    knobs.removeListener (&bottle);
}

void WaterbottleSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void WaterbottleSynthAudioProcessorEditor::resized()
{
    bottle.setBounds (0, 0, getWidth(), getHeight() - 100);
    knobs.setBounds (0, 0, 100, getHeight() - 100);
    bottom.setBounds (0, bottle.getBottom(), getWidth(), 100);
}
