#ifndef BOTTLECOMPONENT_H_INCLUDED
#define BOTTLECOMPONENT_H_INCLUDED

#include "Stickers.h"
#include "BottomComponent.h"
#include "PluginProcessor.h"

class BottleComponent : public Component,
                        public BottomComponent::Listener,
                        private WaterbottleSynthAudioProcessor::StickerListener
{
public:
    BottleComponent (WaterbottleSynthAudioProcessor& p);
    ~BottleComponent();

    void paint (Graphics&) override;
    void resized() override;

    void waterAmountChanged (float newAmt) override;
    void stickersUpdate() override;

    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;

private:
    WaterbottleSynthAudioProcessor& processor;

    std::unique_ptr<Drawable> bottlePic;
    std::unique_ptr<Drawable> water;
    std::unique_ptr<Drawable> draw;

    StickerLasso lasso;

    float waterAmt = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottleComponent)
};

#endif // BOTTLECOMPONENT_H_INCLUDED
