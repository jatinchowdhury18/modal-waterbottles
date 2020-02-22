#include "PluginEditor.h"
#include "BottleComponent.h"

WaterbottleSynthAudioProcessorEditor::WaterbottleSynthAudioProcessorEditor (WaterbottleSynthAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p),
    bottle (p),
    bottom (p)
{
    addAndMakeVisible (bottle);
    addAndMakeVisible (bottom);

    bottom.addListener (&bottle);
    bottom.updateWaterAmount();

    setSize (500, 500);
}

WaterbottleSynthAudioProcessorEditor::~WaterbottleSynthAudioProcessorEditor()
{
    bottom.removeListener (&bottle);
}

void WaterbottleSynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void WaterbottleSynthAudioProcessorEditor::resized()
{
    bottle.setBounds (0, 0, 500, 400);
    bottom.setBounds (0, 400, getWidth(), 100);
}
