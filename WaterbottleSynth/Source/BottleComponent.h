#ifndef BOTTLECOMPONENT_H_INCLUDED
#define BOTTLECOMPONENT_H_INCLUDED

#include "Stickers.h"
#include "KnobsComponent.h"
#include "PluginProcessor.h"

class BottleComponent : public Component,
                        public KnobsComponent::Listener,
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

    static int getBottleArea() { return bottleWidth * bottleHeight; }

private:
    WaterbottleSynthAudioProcessor& processor;

    std::unique_ptr<Drawable> bottlePic;
    std::unique_ptr<Drawable> water;
    std::unique_ptr<Drawable> draw;

    Rectangle<int> bottleRect;
    const static int bottleWidth = 175;
    const static int bottleHeight = 380;

    StickerLasso lasso;

    float waterAmt = 0.0f;

    // reverse iterator
    template<typename It>
    class Range
    {
    public:
        Range (It b, It e) : b (b), e (e) {}

        It begin() const { return b; }
        It end() const { return e; }

    private:
        It b, e;
    };

    template<typename ORange, typename OIt = decltype (std::begin (std::declval<ORange>())), typename It = std::reverse_iterator<OIt>>
    Range<It> reverse (ORange&& originalRange)
    {
        return Range<It> (It (std::end (originalRange)), It (std::begin (originalRange)));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottleComponent)
};

#endif // BOTTLECOMPONENT_H_INCLUDED
