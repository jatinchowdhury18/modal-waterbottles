#ifndef BOTTLECOMPONENT_H_INCLUDED
#define BOTTLECOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include"PluginEditor.h"

class BottleComponent : public Component,
                        public WaterbottleSynthAudioProcessorEditor::Listener
{
public:
    BottleComponent();
    ~BottleComponent();

    void paint (Graphics&) override;
    void resized() override;

    void waterAmountChanged (float newAmt) override;

private:
    std::unique_ptr<Drawable> bottlePic;
    std::unique_ptr<Drawable> water;
    std::unique_ptr<Drawable> draw;

    float waterAmt = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottleComponent)
};

#endif // BOTTLECOMPONENT_H_INCLUDED
